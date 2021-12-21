

/**
 * main.c
 */
#include <math.h>
#include <stdio.h>
#include "lcd.h"
#include "Timer.h"
#include "adc.h"
#include "uart.h"

//#include "uart-interupt.h"

int main(void)
{
   //resetSimulationBoard(); //used for resetting the simGUI

    timer_init(); // Must be called before lcd_init(), which uses timer functions
    lcd_init();
    adc_init();
    uart_init();

    double average = 0.0;

    while(1) {
        int i = 0;
        double sum = 0;

        for (i = 0; i < 16; i++) {
            int data = adc_read();
            char message[100];
            double centi = pow((double) data, -1.143)*99283;
            sum += centi;
            sprintf(message, "Raw Value: %d \nDistance: %0.1f cm \nAverage: %0.2f cm", data, centi, average);
                // sprintf(message, "%d", data);
            lcd_printf(message);
            uart_sendStr(message);
            int j = 0;
               for (j = 0; j < 100000; j++) {}  //delay
        }
        average = sum / 16.0;

    }
	return 0;
}



