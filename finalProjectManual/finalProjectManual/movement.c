/*
 * movement.c
 *
 *  Created on: Feb 9, 2021
 *      Author: jmhafele
 */

#include "movement.h"

bool foundPit = false;
bool noPitStop = false;
bool hitPitCorner = false;
bool moveAroundIsle = false;
int pitTurnState = 0; //1 if turns left initially, 2 if turns right initially after wall bounce, default 1 if never hits wall
int pitHandleState = 0; //1 if incrementing above pit, 2 if started inside of pit
int shortLeftCount = 0;
int shortRightCount = 0;

double move_forward(oi_t *sensor_data, double distance_mm){
    char message[100];
    sprintf(message, "\n\rStarting to move %.2f cm forwards", distance_mm / 10);
    uart_sendStr(message);

    double sum = 0;
    oi_setWheels(250,250);
    while(sum < distance_mm){
        oi_update(sensor_data);
        sum += sensor_data -> distance;//use -> notation since pointer

        if(check_bump(sensor_data) || check_border(sensor_data) || check_pitfall(sensor_data)){ //Checks if robot had collision
            oi_setWheels(0,0); //Stop
//            move_backward(sensor_data, sum); //backs up, states specific checkpoint unreachable
            lcd_printf("distance = %.2f meters", sum/1000); //Prints distance
            sprintf(message, "\n\rMoved %.2f cm forwards", sum / 10);
            uart_sendStr(message);
            return sum;
        }
    }
    oi_setWheels(0,0); //Stop
    lcd_printf("distance = %.2f meters", sum/1000); //Prints distance
    sprintf(message, "\n\rMoved %.2f cm forwards", sum / 10);
    uart_sendStr(message);
    return sum;
}

double turn_right(oi_t *sensor, double degrees){
    double angle = 0;
    char message[100];
    sprintf(message, "\n\rStarting to turn %.2f degrees right", degrees);
    uart_sendStr(message);
    oi_setWheels(50,-50);
    while(angle < degrees - 1.4){
        oi_update(sensor);
        angle += sensor -> angle;
    }
    oi_setWheels(0,0); //Stop
    sprintf(message, "\n\rTurned %.2f degrees right", angle);
    uart_sendStr(message);
    return angle;
}

double turn_left(oi_t *sensor, double degrees){
    char message[100];
    sprintf(message, "\n\rStarting to turn %.2f degrees left", degrees);
    uart_sendStr(message);

    double angle = 0;
    oi_setWheels(-50,50);
    uart_sendStr(message);
    while(angle > -degrees + 1.4){
        oi_update(sensor);
        angle += sensor -> angle;
    }
    oi_setWheels(0,0); //Stop

    sprintf(message, "\n\rTurned %.2f degrees left", angle);
    uart_sendStr(message);

    return angle;
}

double move_backward(oi_t *sensor_data, double distance_mm){
    char message[100];
    sprintf(message, "\n\rStarting to move %.2f cm backwards", distance_mm / 10);
    uart_sendStr(message);

    double sum = 0;
    oi_setWheels(-250,-250);

    while(sum > -distance_mm){
        oi_update(sensor_data);
        sum += sensor_data -> distance;//use -> notation since pointer
    }

    oi_setWheels(0,0); //Stop

    lcd_printf("distance = %.2f meters", sum/1000); //Prints distance
    sprintf(message, "\n\rMoved %.2f cm backwards", sum / 10);
    uart_sendStr(message);

    return sum;
}

int check_bump(oi_t *sensor_data){
    char message[100];
    int fault = 0;
    if(sensor_data -> bumpRight){
        lcd_printf("bumpRight \ndetected");
        sprintf(message, "\n\rRight collision detected");
        uart_sendStr(message);
        fault = 1;
    }
    if(sensor_data -> bumpLeft){
        lcd_printf("bumpLeft \ndetected");
        sprintf(message, "\n\rLeft collision detected");
        uart_sendStr(message);
        fault = 1;
    }
    return fault;
}

int check_border(oi_t *sensor_data){
    char message[100];
    int fault = 0;
    if(sensor_data -> cliffRightSignal > 1000){
        lcd_printf("cliffRightSignal \nBorder \ndetected");
        sprintf(message, "\n\rWall on right detected");
        uart_sendStr(message);
        fault = 1;
    }
    if(sensor_data -> cliffLeftSignal > 1000){
        lcd_printf("cliffLeftSignal \nBorder \ndetected");
        sprintf(message, "\n\rWall on left detected");
        uart_sendStr(message);
        fault = 1;
    }
    if(sensor_data -> cliffFrontRightSignal > 1000){
        lcd_printf("cliffFrontRightSignal \nBorder \ndetected");
        sprintf(message, "\n\rWall on front right detected");
        uart_sendStr(message);
        fault = 1;
    }
    if(sensor_data -> cliffFrontLeftSignal > 1000){
        lcd_printf("cliffFrontLeftSignal \nBorder \ndetected");
        sprintf(message, "\n\rWall on front left detected");
        uart_sendStr(message);
        fault = 1;
    }
    return fault;
}

int check_pitfall(oi_t *sensor_data){
    int fault = 0;
    char message[100];
    if(sensor_data -> cliffRightSignal == 0){
        lcd_printf("cliffRightSignal \nPitfall \ndetected");
        sprintf(message, "\n\rPitfall on right detected");
        uart_sendStr(message);
        fault = 1;
    }
    if(sensor_data -> cliffLeftSignal == 0){
        lcd_printf("cliffLeftSignal \nPitfall \ndetected");
        sprintf(message, "\n\rPitfall on left detected");
        uart_sendStr(message);
        fault = 1;

    }
    if(sensor_data -> cliffFrontRightSignal == 0){
        lcd_printf("cliffFrontRightSignal \nPitfall \ndetected");
        sprintf(message, "\n\rPitfall on front right detected");
        uart_sendStr(message);
        fault = 1;
    }
    if(sensor_data -> cliffFrontLeftSignal == 0){
        lcd_printf("cliffFrontLeftSignal \nPitfall \ndetected");
        sprintf(message, "\n\rPitfall on front left detected");
        uart_sendStr(message);
        fault = 1;
    }
    return fault;
}
