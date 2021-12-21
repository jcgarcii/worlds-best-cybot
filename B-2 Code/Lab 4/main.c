/*
 * main.c
 *
 *  Created on: Feb 9, 2021
 *      Author: jmhafele
 */

#include "open_interface.h"
#include "movement.h"
#include "resetSimulation.h"
#include "button.h"
#include "Timer.h"
#include "lcd.h"
#include "cyBot_uart.h"
#include "cyBot_FindObjects.h"
#include <stdio.h>

void main(){
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    cyBot_uart_init();

    cyBot_Object_t objects[6];

    bool is_front = true;
    int num_objects = 0;
    num_objects = cyBot_FindObjects(objects, num_objects);
    displayObjects(objects, num_objects);

    int i;
    float smallest_width = 10;
    int smallest_index = 0;
    for(i = 0; i < num_objects; i++){
        if(objects[i].width <= smallest_width){
            smallest_width = objects[i].width;
            smallest_index = i;
        }
    }

    turn_right(sensor_data, 180);

    num_objects = cyBot_FindObjects(objects, num_objects);
    displayObjects(objects, num_objects);

    for(i = 0; i < num_objects; i++){
        if(objects[i].width <= smallest_width){
            smallest_width = objects[i].width;
            smallest_index = i;
            is_front = false;
        }
    }

    if(is_front){
        num_objects = cyBot_FindObjects(objects, num_objects);
        displayObjects(objects, num_objects);
    }


    //Keeps robot looping until less than 10 cm away from tall object, where it stops
    while(objects[smallest_index].distance > 10){

        //Turns robot
        if(objects[smallest_index].angle < 90){
            turn_left(sensor_data, 90 - objects[smallest_index].angle);
        }
        else if(objects[smallest_index].angle > 90){
            turn_right(sensor_data, objects[smallest_index].angle - 90);
        }

        //Move robot
        move_forward(sensor_data, objects[smallest_index].distance * 10 - 100);

        //Rescans for tall object to update angle and distance
        num_objects = cyBot_FindObjects(objects, num_objects);
        displayObjects(objects, num_objects);
        smallest_width = 10;
        smallest_index = 0;
        for(i = 0; i < num_objects; i++){
            if(objects[i].width <= smallest_width && objects[i].width){
                smallest_width = objects[i].width;
                smallest_index = i;
            }
        }
    }


    oi_free(sensor_data);

    resetSimulationBoard();
}

void parsePutty(char message[]){
    int i;
    for(i = 0; i < strlen(message); i++){
        cyBot_sendByte(message[i]);
    }
}

void displayObjects(cyBot_Object_t objects[], int num_objects){
    char message[100];
    sprintf(message, "\n \n Object# \t Angle \t \t Distance \t Width");
    parsePutty(message);

    int i;
    for(i = 0; i < num_objects; i++){
        sprintf(message, "\n %d \t \t %d \t  \t %.1f \t \t %.1f", i + 1, objects[i].angle, objects[i].distance, objects[i].width);
        parsePutty(message);
    }
}
