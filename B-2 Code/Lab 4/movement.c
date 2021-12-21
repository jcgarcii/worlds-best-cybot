/*
 * movement.c
 *
 *  Created on: Feb 9, 2021
 *      Author: jmhafele
 */

#include "movement.h"
#include "open_interface.h"
#include "lcd.h"

double move_forward(oi_t *sensor_data, double distance_mm){

    timer_init();
    lcd_init();

    double sum = 0;
    oi_setWheels(250,250);
    while(sum < distance_mm){
        oi_update(sensor_data);
        sum += sensor_data -> distance;//use -> notation since pointer

        if(sensor_data -> bumpRight || sensor_data -> bumpLeft){ //Checks if robot had collision on either bumper
            avoid_collision(sensor_data, distance_mm - sum);
            return sum;
        }

    }
    oi_setWheels(0,0); //Stop
    lcd_printf("distance = %.2f meters", sum/1000); //Prints distance
    return sum;
}

double turn_right(oi_t *sensor, double degrees){
    double angle = 0;
    oi_setWheels(100,-100);
    while(angle < degrees - 8){
        oi_update(sensor);
        angle += sensor -> angle;
    }
    oi_setWheels(0,0); //Stop
    return angle;
}

double turn_left(oi_t *sensor, double degrees){
    double angle = 0;
    oi_setWheels(-100,100);
    while(angle > -degrees + 8){
        oi_update(sensor);
        angle += sensor -> angle;
    }
    oi_setWheels(0,0); //Stop
    return angle;
}

double move_backward(oi_t *sensor_data, double distance_mm){

    timer_init();
    lcd_init();

    double sum = 0;

    oi_setWheels(-250,-250);

    while(sum > -distance_mm){
        oi_update(sensor_data);
        sum += sensor_data -> distance;//use -> notation since pointer
    }

    oi_setWheels(0,0); //Stop
    lcd_printf("distance = %.2f meters", sum/1000); //Prints distance
    return sum;
}

void avoid_collision(oi_t *sensor_data, double distance_left){

    if(sensor_data -> bumpRight){
        move_backward(sensor_data, 150);
        turn_left(sensor_data, 90);
        move_forward(sensor_data, 250);
        turn_right(sensor_data, 90);
        move_forward(sensor_data, 400);
        turn_right(sensor_data, 45); //Added for lab 4. Makes it so robot turns back towards center some for better scan
    }

    else if(sensor_data -> bumpLeft){
        move_backward(sensor_data, 150);
        turn_right(sensor_data, 90);
        move_forward(sensor_data, 250);
        turn_left(sensor_data, 90);
        move_forward(sensor_data, 400);
        turn_left(sensor_data, 45); //Added for lab 4. Makes it so robot turns back towards center some for better scan
    }

//    move_forward(sensor_data, distance_left);
}
