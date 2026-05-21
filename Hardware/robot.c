#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "Delay.h"

// Initialize the robot motor PWM module.
void robot_Init(void)
{
    PWM_Init();
}

// Set the four PWM channels used by the motor driver.
void robot_speed(uint8_t left1_speed, uint8_t left2_speed, uint8_t right1_speed, uint8_t right2_speed)
{
    TIM_SetCompare1(TIM4, left1_speed);
    TIM_SetCompare2(TIM4, left2_speed);
    TIM_SetCompare3(TIM4, right1_speed);
    TIM_SetCompare4(TIM4, right2_speed);
}

// Move forward. The right wheel is slightly reduced to compensate for motor speed difference.
void makerobo_run(int8_t speed, uint16_t time)
{
    if(speed > 100)
    {
        speed = 100;
    }
    if(speed < 0)
    {
        speed = 0;
    }

    robot_speed(speed, 0, speed - 7, 0);
    Delay_ms(time);
}

// Stop both motors.
void makerobo_brake(uint16_t time)
{
    robot_speed(0, 0, 0, 0);
    Delay_ms(time);
}

// Turn left by driving only the right wheel forward.
void makerobo_Left(int8_t speed, uint16_t time)
{
    if(speed > 100)
    {
        speed = 100;
    }
    if(speed < 0)
    {
        speed = 0;
    }

    robot_speed(0, 0, speed, 0);
    Delay_ms(time);
}

// Spin left in place.
void makerobo_Spin_Left(int8_t speed, uint16_t time)
{
    if(speed > 100)
    {
        speed = 100;
    }
    if(speed < 0)
    {
        speed = 0;
    }

    robot_speed(0, speed, speed, 0);
    Delay_ms(time);
}

// Turn right by driving only the left wheel forward.
void makerobo_Right(int8_t speed, uint16_t time)
{
    if(speed > 100)
    {
        speed = 100;
    }
    if(speed < 0)
    {
        speed = 0;
    }

    robot_speed(speed, 0, 0, 0);
    Delay_ms(time);
}

// Spin right in place.
void makerobo_Spin_Right(int8_t speed, uint16_t time)
{
    if(speed > 100)
    {
        speed = 100;
    }
    if(speed < 0)
    {
        speed = 0;
    }

    robot_speed(speed, 0, 0, speed);
    Delay_ms(time);
}

// Move backward.
void makerobo_back(int8_t speed, uint16_t time)
{
    if(speed > 100)
    {
        speed = 100;
    }
    if(speed < 0)
    {
        speed = 0;
    }

    robot_speed(0, speed, 0, speed);
    Delay_ms(time);
}
