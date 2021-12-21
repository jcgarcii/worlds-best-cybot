/**
 * lab5_template.c
 *
 * Template file for CprE 288 Lab 5
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 *
 * @author Phillip Jones, updated 6/4/2019
 * @author Diane Rover, updated 2/25/2021
 */

#include "button.h"
#include "timer.h"
#include "lcd.h"

#include "cyBot_uart.h"  // Functions for communicating between CyBot and Putty (via UART1)
                         // PuTTy: Baud=115200, 8 data bits, No Flow Control, No Parity, COM1

#include "cyBot_FindObjects.h"  // Scan the CyBot sensors to find objects

#include "uart.h"
#include <stdint.h>
#include <stdbool.h>


//#warning "Possible unimplemented functions"
#define REPLACEME 0



int main(void) {
	button_init();
	timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();
	uart_init();


//	uint8_t num = 0;
//	char s_data, chr[20];
//
//	uart_init();
//	for(s_data = 'A'; s_data <= 'Z'; s_data = s_data +1) {
//	    uart_sendChar(s_data);      //send data
//	    chr[num] = uart_receive();  //receive data
//	    num++;
//	}
//	while(1) {

	 int charReceived = 0;
	 char data;
	 while(charReceived < 20) {
	     data = uart_receive();
	     if (data == '\r') {
	         charReceived = 20;
	         uart_sendChar('\r');
	         uart_sendChar('\n');
	     }
	 lcd_putc(uart_receive());
	 uart_sendChar(data);
	 charReceived++;
	 }
	 char message[] = "cybottest";
	 uart_sendStr(message);


	    // YOUR CODE HERE
	//  char test[100];
	//  sprintf(test, "isu \n");
	//  parsePutty(test);
	//  while(1){}


}

void parsePutty(char message[]){
    int i;
    for(i = 0; i < strlen(message); i++){
        cyBot_sendByte(message[i]);
    }
}
