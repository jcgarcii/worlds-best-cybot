/*
 * servo.h
 *
 *  Created on: Mar 30, 2021
 *      Author: egabriel
 */

#ifndef SERVO_H_
#define SERVO_H_

#include "REF_tm4c123gh6pm.h"
#include "uart-interrupt.h"
#include <stdbool.h>
#include <stdint.h>
#include "lcd.h"


#endif /* SERVO_H_ */


void servo_init(void);
void servo_calibrate(void);
int servo_move(float degrees);
