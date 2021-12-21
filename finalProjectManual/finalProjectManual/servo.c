/*
 * servo.c
 *
 *  Created on: Mar 30, 2021
 *      Author: egabriel
 */


#include "servo.h"

int raw_180 = 0;
int raw_0 = 0;
bool not_calibrated = true;

void servo_init(void) {

    SYSCTL_RCGCTIMER_R |= 0x02; //Enables timer 1 on bit 1
    SYSCTL_RCGCGPIO_R |= 0x02; //Enables Port B by setting bit 1
    while ((SYSCTL_PRTIMER_R & 0x02) == 0) {}; //Busy-wait for Timer1 enable on bit 1
    while ((SYSCTL_PRGPIO_R & 0x02) == 0) {}; //Busy-wait for PortB enable on bit 1

    GPIO_PORTB_DEN_R |= 0x20; //Enables PB5
    //GPIO_PORTB_DIR_R &= 0xF7; //Sets PB3 as input by clearing bit 3
    GPIO_PORTB_AFSEL_R |= 0x20; //Enables alternate function for PB5
    GPIO_PORTB_PCTL_R &= 0xFF0FFFFF;
    GPIO_PORTB_PCTL_R |= 0x00700000; //Sets PB5 alternate function to Timer1B


    TIMER1_CTL_R &= ~0x0100; //disables Timer1B
    //TIMER1_CTL_R |= 0x0100; //set bit 14 for PWM inverted output?
    TIMER1_CFG_R = 4; //sets Timer1 to 16 bit
    TIMER1_TBMR_R = 0xA; //configures PWM mode
    TIMER1_TBILR_R = 0xE200; //sets value to 320,000
    TIMER1_TBPR_R = 0x04; //sets prescaler

    // Configure and enable the timer
    TIMER1_CTL_R |= 0x0100; //enables Timer1B
}

void servo_calibrate(void){
    raw_180 = 0; //resets raw values
    raw_0 = 0;
    char scan; //Char to scan from putty
    float degrees = 180; //Current angle servo moving, unoptimized
    servo_move(degrees);
    char message1[] = "\n\r l: turn left \n\r r: turn right \n\r d: Press if at 180 degrees \n\r Turn right for initial half turn\n\r";
    uart_sendStr(message1); //Sends instruction string
    //Find proper 180
    while(raw_180 == 0){
        scan = uart_receive(); //busy-wait for putty input
        if(scan == 'l'){ //Turn left condition
            degrees -= 3;
            servo_move(degrees);
        }
        else if(scan == 'r'){ //Turn right condition
            degrees += 3;
            servo_move(degrees);
        }
        else if(scan == 'd'){ //Done condition
            raw_180 = servo_move(degrees);
            lcd_printf("%d", raw_180);
        }
    }
    degrees = 0;
    servo_move(degrees);
    char message2[] = "\n\r l: turn left \n\r r: turn right \n\r f: Press if at 0 degrees \n\r Turn right for initial half turn\n\r";
    uart_sendStr(message2);
    //Find proper 0
    while(raw_0 == 0){
        scan = uart_receive(); //busy-wait for putty input
        if(scan == 'l'){ //Turn left condition
            degrees -= 3;
            servo_move(degrees);
        }
        else if(scan == 'r'){ //Turn right condition
            degrees += 3;
            servo_move(degrees);
        }
        else if(scan == 'f'){ //done condition
            raw_0 = servo_move(degrees);
            lcd_printf("%d", raw_0);
        }
    }
    not_calibrated = false;

}

int servo_move(float degrees) {
    int match;
    if(not_calibrated){ //Uncalibrated mode
        float high_pulse_ms = (degrees / 180) + 1;
        float low_pulse_ms = 20 - high_pulse_ms;
        float s = low_pulse_ms / 1000;
        match = 16000000 * s;
        TIMER1_TBMATCHR_R = match & 0xFFFF;
        TIMER1_TBPMR_R = match >> 16;
    }
    else { //Calibrated mode
        int match;
        match = (unsigned int) ((raw_180 - raw_0)/180)*degrees + raw_0;
        TIMER1_TBMATCHR_R = match & 0xFFFF;
        TIMER1_TBPMR_R = match >> 16;
    }
    return match;
}
