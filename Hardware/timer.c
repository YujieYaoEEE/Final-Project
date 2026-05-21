#include "stm32f10x.h"                  // Device header

// Initialize TIM2. In this project TIM2 is used as the ultrasonic ECHO timing counter.
void Timerx_Init(uint16_t arr, uint16_t psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // Configure TIM2 as an up-counting timer with a 10 kHz count frequency.
    TIM_TimeBaseStructure.TIM_Period = 5000;
    TIM_TimeBaseStructure.TIM_Prescaler = (7200 - 1);
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // Enable TIM2 update and trigger interrupts.
    TIM_ITConfig(
        TIM2,
        TIM_IT_Update |
        TIM_IT_Trigger,
        ENABLE
    );

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
