#ifndef __UltrasonicWave_H
#define __UltrasonicWave_H

void UltrasonicWave_Init(void);       // Initialize the ultrasonic module.
int UltrasonicWave_StartMeasure(void); // Start one ultrasonic measurement and return the distance.

#endif
