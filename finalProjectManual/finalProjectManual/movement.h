/*
 * movement.h
 *
 *  Created on: Feb 9, 2021
 *      Author: jmhafele
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "open_interface.h"
#include "lcd.h"
#include "object.h"
#include "uart-interrupt.h"

//moves cybot forward and updates checkpoints
double move_forward(oi_t *sensor_data, double distance_mm);

//Turns cybot right x degrees
double turn_right(oi_t *sensor, double degrees);

//turns cybot left x degrees
double turn_left(oi_t *sensor, double degrees);

//moves cybot strictly backward, no fluff
double move_backward(oi_t *sensor_data, double distance_mm);

//checks bump sensors
int check_bump(oi_t *sensor_data);

//Checks border sensors
int check_border(oi_t *sensor_data);

//checks cliff sensors
int check_pitfall(oi_t *sensor_data);

#endif /* MOVEMENT_H_ */
