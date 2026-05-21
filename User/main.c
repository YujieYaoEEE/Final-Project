#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "robot.h"
#include "UltrasonicWave.h"
#include "Key.h"
#include "timer.h"
#include "Servo.h"
#include "Irtracking.h"

#define OBSTACLE_DISTANCE      150
#define FRONT_SCAN_INTERVAL    30

#define TRACK_SPEED            90
#define TURN_SPEED             80
#define BYPASS_SPEED           80

#define BACK_TIME              500
#define BRAKE_TIME             150
#define SERVO_SCAN_WAIT        700
#define MEASURE_COUNT          3

#define BYPASS_TURN_TIME       800
#define BYPASS_SIDE_TIME       600
#define BYPASS_FORWARD_TIME    2000
#define BYPASS_RETURN_TIMEOUT  1800
#define LINE_CORRECTION_TIME   150

// Measure the distance directly in front of the car.
int front_detection(void)
{
    Servo_SetAngle(90);
    Delay_ms(100);
    return UltrasonicWave_StartMeasure();
}

// Take several ultrasonic measurements and return the average valid value.
int Measure_Average(void)
{
    int i;
    int distance;
    int sum = 0;
    int count = 0;

    for(i = 0; i < MEASURE_COUNT; i++)
    {
        distance = UltrasonicWave_StartMeasure();

        if(distance > 0)
        {
            sum += distance;
            count++;
        }

        Delay_ms(50);
    }

    if(count == 0)
    {
        return 0;
    }

    return sum / count;
}

// Turn the servo to the left and measure the left-side distance.
int left_detection(void)
{
    Servo_SetAngle(175);
    Delay_ms(SERVO_SCAN_WAIT);
    return Measure_Average();
}

// Turn the servo to the right and measure the right-side distance.
int right_detection(void)
{
    Servo_SetAngle(5);
    Delay_ms(SERVO_SCAN_WAIT);
    return Measure_Average();
}

// Read the two tracking sensors and adjust the car to follow the line.
void Tracking(void)
{
    uint8_t left = Left_Irtracking_Get();
    uint8_t right = Right_Irtracking_Get();

    if(left == 0 && right == 0)
    {
        makerobo_run(TRACK_SPEED, 0);
    }
    else if(left == 1 && right == 0)
    {
        makerobo_Left(60, 0);
    }
    else if(left == 0 && right == 1)
    {
        makerobo_Right(60, 0);
    }
    else
    {
        makerobo_brake(0);
    }
}

// Move forward until the line is detected, then correct toward the bypass direction.
void Forward_Until_Line_Correct(uint16_t timeout_ms, uint8_t correct_left)
{
    uint16_t elapsed_ms = 0;

    makerobo_run(BYPASS_SPEED, 0);

    while(elapsed_ms < timeout_ms)
    {
        if(Left_Irtracking_Get() == 1 || Right_Irtracking_Get() == 1)
        {
            if(correct_left)
            {
                makerobo_Left(TURN_SPEED, LINE_CORRECTION_TIME);
            }
            else
            {
                makerobo_Right(TURN_SPEED, LINE_CORRECTION_TIME);
            }

            makerobo_run(BYPASS_SPEED, 0);
            return;
        }

        Delay_ms(10);
        elapsed_ms += 10;
    }

    makerobo_brake(100);
}

// Bypass an obstacle from the right side and return as soon as the line is detected.
void Bypass_Right(void)
{
    makerobo_Spin_Right(TURN_SPEED, BYPASS_TURN_TIME);
    makerobo_brake(200);

    makerobo_run(BYPASS_SPEED, BYPASS_SIDE_TIME);
    makerobo_brake(200);

    makerobo_Spin_Left(TURN_SPEED, BYPASS_TURN_TIME);
    makerobo_brake(200);

    makerobo_run(BYPASS_SPEED, BYPASS_FORWARD_TIME);
    makerobo_brake(200);

    makerobo_Spin_Left(TURN_SPEED, BYPASS_TURN_TIME);
    makerobo_brake(200);

    Forward_Until_Line_Correct(BYPASS_RETURN_TIMEOUT, 0);
}

// Bypass an obstacle from the left side and return as soon as the line is detected.
void Bypass_Left(void)
{
    makerobo_Spin_Left(TURN_SPEED, BYPASS_TURN_TIME);
    makerobo_brake(200);

    makerobo_run(BYPASS_SPEED, BYPASS_SIDE_TIME);
    makerobo_brake(200);

    makerobo_Spin_Right(TURN_SPEED, BYPASS_TURN_TIME);
    makerobo_brake(200);

    makerobo_run(BYPASS_SPEED, BYPASS_FORWARD_TIME);
    makerobo_brake(200);

    makerobo_Spin_Right(TURN_SPEED, BYPASS_TURN_TIME);
    makerobo_brake(200);

    Forward_Until_Line_Correct(BYPASS_RETURN_TIMEOUT, 1);
}

int main(void)
{
    int Q_temp, L_temp, R_temp;
    uint16_t count = 0;

    Timerx_Init(5000,7199);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    Key_Init();
    UltrasonicWave_Init();
    robot_Init();
    Servo_Init();
    Irtracking_Init();

    Servo_SetAngle(90);

    while(Key_GetNum() == 0);

    while(1)
    {
        Tracking();
        Delay_ms(5);

        count++;
        if(count < FRONT_SCAN_INTERVAL)
        {
            continue;
        }
        count = 0;

        Q_temp = front_detection();
        if(Q_temp < OBSTACLE_DISTANCE && Q_temp > 0)
        {
            makerobo_brake(BRAKE_TIME);
            makerobo_back(TRACK_SPEED, BACK_TIME);
            makerobo_brake(BRAKE_TIME);

            L_temp = left_detection();
            R_temp = right_detection();
            Servo_SetAngle(90);
            Delay_ms(200);

            if(L_temp > R_temp)
            {
                Bypass_Left();
            }
            else
            {
                Bypass_Right();
            }
        }
    }
}

