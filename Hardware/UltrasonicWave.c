#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "timer.h"

#define TRIG_PORT      GPIOB        // TRIG port
#define ECHO_PORT      GPIOB        // ECHO port
#define TRIG_PIN       GPIO_Pin_15  // TRIG pin
#define ECHO_PIN       GPIO_Pin_14  // ECHO pin

float UltrasonicWave_Distance;      // Calculated distance

//Description: Initialize the ultrasonic module and configure the ECHO interrupt.

void UltrasonicWave_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin = TRIG_PIN;               // PB15 connects to TRIG
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      // Push-pull output
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TRIG_PORT, &GPIO_InitStructure);            // Initialize TRIG GPIO

    GPIO_InitStructure.GPIO_Pin = ECHO_PIN;               // PB14 connects to ECHO
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;         // Pull-down input
    GPIO_Init(ECHO_PORT, &GPIO_InitStructure);            // Initialize ECHO GPIO

    // Configure PB14 as EXTI line 14.
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);

    EXTI_InitStructure.EXTI_Line = EXTI_Line14;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// Handle ECHO rising and falling edges without blocking inside the interrupt.
void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line14) != RESET)
    {
        if(GPIO_ReadInputDataBit(ECHO_PORT, ECHO_PIN) == 1)
        {
            // Rising edge: start timing the ECHO high level.
            TIM_SetCounter(TIM2, 0);
            TIM_Cmd(TIM2, ENABLE);
        }
        else
        {
            // Falling edge: stop timing and calculate the distance.
            TIM_Cmd(TIM2, DISABLE);
            UltrasonicWave_Distance = TIM_GetCounter(TIM2) * 5 * 34 / 200.0;
        }

        EXTI_ClearITPendingBit(EXTI_Line14);
    }
}

//Send a trigger pulse and return the measured distance.

int UltrasonicWave_StartMeasure(void)
{
    int u_temp;

    GPIO_SetBits(TRIG_PORT, TRIG_PIN);
    Delay_us(20);
    GPIO_ResetBits(TRIG_PORT, TRIG_PIN);

    Delay_ms(30);  // Wait for this measurement to complete.

	
    u_temp = UltrasonicWave_Distance * 10;
    return u_temp;
}
