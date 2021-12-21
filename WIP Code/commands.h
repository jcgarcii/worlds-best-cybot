/**
*  this .h file helps mend the code in Movement.c with UART
*/

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <math.h>
#include <inc/tm4c123gh6pm.h>
#define PI 3.1415926535897

struct obstacle{float width; float distance; int count; int location;};

/**
* 	Sweep with both the Ping sensor and IR distance sensor over the 180 degree range of the servo,
* 	taking distance measurements from both sensors every 2 degrees and transmitting the data to Putty.
*/
void botScan();

/*
*	Moves bot in accordance to key being used! 	
*/
void move_bot();

void move_forward(oi_t *sensor, int centimeters);

void move_backwards(oi_t *sensor, int centimeters);

void turn_countClockwise(oi_t *sensor, int degrees);

void turn_clockwise(oi_t *sensor, int degrees);

int bumper(oi_t *sensor);

int cliff(oi_t *sensor);

#endif /* COMMANDS_H_ */
