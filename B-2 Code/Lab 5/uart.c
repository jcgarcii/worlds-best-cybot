/*
 * uart.c
 *
 *  Created on: Mar 2, 2021
 *      Author: egabriel
 */

#include "button.h"
#include "timer.h"
#include "lcd.h"
#include "cyBot_uart.h"  // Functions for communicating between CyBot and Putty (via UART1)
                         // PuTTy: Baud=115200, 8 data bits, No Flow Control, No Parity, COM1

//#include "cyBot_FindObjects.h"  // Scan the CyBot sensors to find objects

#include <stdint.h>
#include <stdbool.h>
//#include "TM4C123GH6PM.h"

void uart_init(void) {

       cyBot_uart_init_clean();  // Clean UART initialization, before running your UART GPIO init code

       SYSCTL_RCGCGPIO_R |= 0x02;   //enable clock to GPIO_PORTB
       SYSCTL_RCGCUART_R = 0x02;     //enable clock to UART1
          while ((SYSCTL_PRGPIO_R & 0x02) == 0) {};

        GPIO_PORTB_DEN_R |= 0x03;
        GPIO_PORTB_AFSEL_R |= 0x03;
        GPIO_PORTB_PCTL_R &= 0xFFFFFF00;     // Force 0's in the desired locations
        GPIO_PORTB_PCTL_R |= 0x00000011;     // Force 1's in the desired locations



        UART1_CTL_R &= 0xFFFFFFFE;      //disables UART1
        UART1_IBRD_R = 0x8;               //set the integer baud rate period(IBRD = 8)
        UART1_FBRD_R = 0x2C;              //set fractional baud rate period (FBRD = 44)
        UART1_LCRH_R = 0x60;              //set baud rate = 115200, 8-bit data, 1 stop no parity
        UART1_CC_R = 0x0;                 //set UARTCC as 00 to select System Clock as source
        UART1_CTL_R |= 0x1;               //enable UART1

        //cyBot_uart_init_last_half();  // Complete the UART device configuration

}


void uart_sendChar(char sdata) {
    while((UART1_FR_R & 0x20)!= 0);
    UART1_DR_R = sdata;

}
    //do we need anymore than this?


char uart_receive(void) {
    int ret;
    char rdata;

    while((UART1_FR_R & 0x10)!= 0);
    ret = UART1_DR_R;
//    if(ret & 0xF00) {
//        GPIO_PORTF_DATA_R = 0xF;
//    }
    rdata = (char)(UART1_DR_R & 0xFF);

    return rdata;

}



void uart_sendStr(const char *data){
       int i;
       for(i = 0; i < strlen(data); i++){
           uart_sendChar(data[i]);
       }
}



