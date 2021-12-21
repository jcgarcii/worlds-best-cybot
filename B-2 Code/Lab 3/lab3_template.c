/**
 * lab3_template.c
 * 
 * Template file for CprE 288 lab 3
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 */

#include "button.h"
#include "Timer.h"
#include "lcd.h"
#include "cyBot_uart.h"  // Functions for communicating between CyBot and Putty (via UART)
                         // PuTTy: Baud=115200, 8 data bits, No Flow Control, No Parity,  COM1

#define REPLACEME 0



int main(void) {
	button_init();
	timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();
	cyBot_uart_init(); // initialize the cyBot UART

	bool hasSent = false;
	
//	resetSimulationBoard(); //used for resetting the simGUI
	while(1)
	{
//	    if (button_getButton() == 0) {      //this is true when none of the buttons are being pressed
//	        lcd_printf("No Button Pressed");   //LCD screen indicates no buttons pressed
//	    }
//	    else {
//	    lcd_printf("b%d", button_getButton());  //prints the rightmost button to the screen
//	    }

	    if (button_getButton() == 1 && !hasSent) {
	        cyBot_sendByte('H');
	        cyBot_sendByte('E');
	        cyBot_sendByte('L');
	        cyBot_sendByte('L');
	        cyBot_sendByte('O');
	        cyBot_sendByte(' ');
	        hasSent = true;
	    }
	    else if (button_getButton() == 2 && !hasSent) {
	        cyBot_sendByte('C');
	        cyBot_sendByte('P');
	        cyBot_sendByte('R');
	        cyBot_sendByte('E');
	        cyBot_sendByte('2');
	        cyBot_sendByte('8');
	        cyBot_sendByte('8');
	        cyBot_sendByte(' ');
	        hasSent = true;
	    }
	    else if (button_getButton() == 3 && !hasSent) {
	        cyBot_sendByte('C');
	        cyBot_sendByte('Y');
	        cyBot_sendByte('B');
	        cyBot_sendByte('O');
	        cyBot_sendByte('T');
	        cyBot_sendByte(' ');
	        hasSent = true;
	    }
	    else if (button_getButton() == 4 && !hasSent) {
	        cyBot_sendByte('I');
	        cyBot_sendByte('S');
	        cyBot_sendByte('U');
	        cyBot_sendByte(' ');
	        hasSent = true;
	    }

	    if (hasSent) {
	    char recieve = cyBot_getByte();
	    lcd_printf("%c", recieve);
	    hasSent = false;
	    }

	}

}
