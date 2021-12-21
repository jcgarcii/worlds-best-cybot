/*
 * movement.h
 *
 *  Created on: Feb 9, 2021
 *      Author: jmhafele
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "open_interface.h"

double move_forward(oi_t *sensor_data, double distance_mm);

double turn_right(oi_t *sensor, double degrees);

double turn_left(oi_t *sensor, double degrees);

double move_backward(oi_t *sensor_data, double distance_mm);

void avoid_collision(oi_t *sensor, double distance_left);

#endif /* MOVEMENT_H_ */
