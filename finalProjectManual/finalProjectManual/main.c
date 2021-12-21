/**
 * @file lab8_template.c
 * @author
 * Template file for CprE 288 Lab 8
 */

#include "ping.h"
#include "Timer.h"
#include "lcd.h"
#include "object.h"
#include "uart-interrupt.h"
#include "movement.h"
#include "open_interface.h"
#include "commands.h"
#include "button.h"


int main(void)
{
//    resetSimulationBoard();
    timer_init(); // Must be called before lcd_init(), which uses timer functions
    lcd_init(); //initializes lcd init
    uart_interrupt_init(); //initializes uart interrupt stuff
    button_init();
    object_init(); //Initializes ping, adc, and servo all at once

    oi_t *sensor_data = oi_alloc(); //Used for open interface sensors
    oi_init(sensor_data); //Initializes open interface

    Object objects[10];

//    width_calibrate(objects);

    int is_done = 0;

    while (!is_done)
    {
        is_done = move_bot(sensor_data, objects);
    }

}
