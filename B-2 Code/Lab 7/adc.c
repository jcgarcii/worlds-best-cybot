/*
 * ACD.c
 *
 *  Created on: Mar 16, 2021
 *      Author: egabriel
 */
#include "adc.h"

void adc_init(void) {
    SYSCTL_RCGCADC_R |= 0x0001;      //activate ADC0
    SYSCTL_RCGCGPIO_R |= 0x02;      //enable GPIO port B, using AIN10 on PB4

    while((SYSCTL_PRGPIO_R & 0x02)  != 0x02){};     //wait for portB to stabilize

    GPIO_PORTB_DIR_R &= ~0x10;      //make PB4 input
    GPIO_PORTB_AFSEL_R |= 0x10;     //enable alternate function on PB4
    GPIO_PORTB_DEN_R &= ~0x10;      //disable digital I/O on PB4
    GPIO_PORTB_AMSEL_R |= 0x10;     //enable analog functionality on PB4

    while((SYSCTL_PRADC_R&0X0001) != 0x0001) {};   //wait for ADC to be ready

    ADC0_PC_R &= ~0xF;
    ADC0_PC_R |= 0x1;               //configure for 125K samples/sec, might not need this
    ADC0_SSPRI_R = 0x0123;          //sequencer 3 is highest priority
    ADC0_ACTSS_R &= ~0x0008;        //disable sample sequencer 3
    ADC0_EMUX_R &= ~0xF000;         //seq3 is software trigger
    ADC0_SSMUX3_R &= 0x000F;
    ADC0_SSMUX3_R += 10;            //10 used to represent AIN10 being used on PB4
    //might have to reevaluate the value above
    ADC0_SSCTL3_R = 0x0006;         //no TSO DO, yes IE0 NED0
    ADC0_IM_R &= ~0x0008;           //disable SS3 interupts
    ADC0_ACTSS_R |= 0x0008;         //enable sample sequencer 3
}


int adc_read(void) {
    ADC0_PSSI_R = 0x0008;               //initiate SS3
    while ((ADC0_RIS_R&0x08)==0) {};    //wait for conversion done
    int result = ADC0_SSFIFO3_R&0xFFF;      //read result
    ADC0_ISC_R = 0x0008;                //acknowledge completion
    return result;
}



