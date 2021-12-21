/*
 * main.c
 *
 *  Created on: Feb 9, 2021
 *      Author: jmhafele
 */

#include "open_interface.h"
#include "movement.h"
#include "resetSimulation.h"

void main(){
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

//    move_forward(sensor_data, 250);
//    turn_right(sensor_data, 90);
//    move_forward(sensor_data, 250);
//    turn_right(sensor_data, 90);
//    move_forward(sensor_data, 250);
//    turn_right(sensor_data, 90);
//    move_forward(sensor_data, 250);
//    turn_right(sensor_data, 90);

    move_forward(sensor_data, 2000);

    oi_free(sensor_data); //Do once at end of main

//    resetSimulationBoard();
}
