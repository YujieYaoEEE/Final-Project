#ifndef __ROBOT_H
#define __ROBOT_H

void robot_Init(void);
void robot_speed(uint8_t left1_speed, uint8_t left2_speed, uint8_t right1_speed, uint8_t right2_speed);

// Basic movement functions.
void makerobo_run(uint8_t speed, uint16_t time);          // Move forward.
void makerobo_brake(uint16_t time);                       // Stop the robot.
void makerobo_Left(int8_t speed, uint16_t time);          // Turn left.
void makerobo_Spin_Left(int8_t speed, uint16_t time);     // Spin left in place.
void makerobo_Right(int8_t speed, uint16_t time);         // Turn right.
void makerobo_Spin_Right(int8_t speed, uint16_t time);    // Spin right in place.
void makerobo_back(int8_t speed, uint16_t time);          // Move backward.

#endif
