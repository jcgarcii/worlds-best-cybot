/*
 * object.h
 *
 *  Created on: Apr 8, 2021
 *      Author: jmhafele
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include "ping.h"
#include "adc.h"
#include "servo.h"
#include "uart-interrupt.h"
#define PI 3.1415926535897

typedef struct{
    int angle; //degrees
    double distance; //cm
    double width; //cm
} Object;

//Initializes adc, servo, and ping
void object_init(void);

//Scans all objects within 180 degrees. Will detect only tall objects
int object_scan(Object objects[], int delay);

//Prints array of objects to the screen, used in scan already but can be called anytime with last set of objects in array
void print_objects(Object objects[], int num_objects);

void width_calibrate(Object objects[]);

#endif /* OBJECT_H_ */
