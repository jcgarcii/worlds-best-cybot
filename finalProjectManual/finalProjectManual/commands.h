/**
*  this .h file helps mend the code in Movement.c with UART
*/

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <math.h>
#include <inc/tm4c123gh6pm.h>
#include <stdio.h>
#include "uart-interrupt.h"
#include "timer.h"
#include "servo.h"
#include "ping.h"
#include "lcd.h"
#include "open_interface.h"
#include "movement.h"
#include "object.h"
#include "button.h"

/*
*	Moves bot in accordance to key being used! 	
*/
int move_bot(oi_t *sensor_data, Object objects[]);

double get_double_uart(void);

double get_double_button(void);

bool verify_double(double value);

void print_instructions(void);

#endif /* COMMANDS_H_ */
