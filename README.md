# Final-Project
This project is an STM32F103C8-based robot car program. It combines infrared line tracking, ultrasonic distance measurement, servo scanning, and PWM motor control to allow the car to follow a line and avoid obstacles.

## Main Features

- Infrared line tracking using left and right tracking sensors
- Ultrasonic obstacle detection using a front-mounted ultrasonic module
- Servo-based left, front, and right distance scanning
- PWM motor control for forward movement, backward movement, turning, spinning, and braking
- Key-controlled startup
- Obstacle bypass logic that selects the side with more available space
- Return-to-line logic using the infrared tracking sensors after bypassing an obstacle

Hardware/
  robot.c / robot.h              Motor movement control
  PWM.c / PWM.h                  TIM4 PWM for motor control
  UltrasonicWave.c / .h          Ultrasonic distance measurement
  Servo.c / Servo.h              Servo angle control
  ServoPWM.c / ServoPWM.h        TIM3 PWM for servo control
  Irtracking.c / Irtracking.h    Infrared line tracking input
  Key.c / Key.h                  Startup key input
  timer.c / timer.h              TIM2 timing for ultrasonic measurement

  Main Logic
After power-on, the program initializes the key, timer, motor, servo, ultrasonic module, and infrared tracking sensors. The car waits until the key is pressed before starting.

During normal operation, the car continuously reads the left and right infrared sensors and adjusts its movement to follow the line. The ultrasonic sensor does not measure distance in every loop. Instead, a counter is used to scan the front distance periodically, which helps keep line tracking responsive.

When an obstacle is detected in front of the car, the car stops, moves backward slightly, and uses the servo to scan the left and right sides. The program compares the left and right distances and chooses the side with more available space. After bypassing the obstacle, the car uses the infrared tracking sensors to find the original line again.
