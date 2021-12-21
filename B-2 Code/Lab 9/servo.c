/*
 * servo.c
 *
 *  Created on: Mar 30, 2021
 *      Author: egabriel
 */

#include "REF_tm4c123gh6pm.h"


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
    TIMER1_TBMR_R = 0x0C; //sets periodic PWM mode
    TIMER1_TBILR_R = 0xE200; //sets value to 320,000
    TIMER1_TBPR_R = 0x04; //sets prescaler

    // Configure and enable the timer
    TIMER1_CTL_R |= 0x0100; //enables Timer1B
}

int servo_move(float degrees) {
    float ms = (degrees / 180) + 1;
    float s = ms / 1000;
    int match = 16000000 * s;
    TIMER1_TBMATCHR_R = match & 0xFFFF;
    TIMER1_TBPMR_R = match & 0xFF0000;
    return 1;
}
