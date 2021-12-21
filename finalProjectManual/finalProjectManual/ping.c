/**
 * Driver for ping sensor
 * @file ping.c
 * @author
 */

#include "ping.h"

volatile unsigned long START_TIME = 0;
volatile unsigned long END_TIME = 0;
volatile enum{LOW, HIGH, DONE} STATE = LOW; // State of ping echo pulse
volatile unsigned int risingEdge, fallingEdge = 0;
volatile unsigned int overflowCount = 0;
double clkStp = 62.5; //Time for each clock cycle, 62.5 ns

void ping_init (void){

  // YOUR CODE HERE

    SYSCTL_RCGCTIMER_R |= 0x08; //Enables timer 3 on bit 3
    SYSCTL_RCGCGPIO_R |= 0x02; //Enables Port B by setting bit 1
    while ((SYSCTL_PRTIMER_R & 0x08) == 0) {}; //Busy-wait for Timer3 enable on bit 3
    while ((SYSCTL_PRGPIO_R & 0x02) == 0) {}; //Busy-wait for PortB enable on bit 1

    GPIO_PORTB_DEN_R |= 0x08; //Enables PB3
    GPIO_PORTB_DIR_R &= 0xF7; //Sets PB3 as input by clearing bit 3
    GPIO_PORTB_AFSEL_R |= 0x08; //Enables alternate function for PB3
    GPIO_PORTB_PCTL_R &= 0xFFFF0FFF;
    GPIO_PORTB_PCTL_R |= 0x00007000; //Sets PB3 alternate function to Timer3B


    TIMER3_CTL_R &= ~0x0100; //disables Timer3B
    TIMER3_CFG_R = 4; //Sets Timer3 to 16 bit
    TIMER3_TBMR_R = 0x07; //Sets to capture mode, edge time mode
    TIMER3_CTL_R |= 0x0C00; //sets event type to both rising and falling edges by setting bits 10/11
    TIMER3_TBILR_R = 0xFFFF; //Loads max value so timer can start counting down
    TIMER3_TBPR_R = 0xFF; //activates prescale, making 24 bit

    NVIC_PRI9_R |= 0x20; //Set Timer3B interrupt to highest priority (0x2)
    NVIC_EN1_R |= 0x10; //sets bit to 1, enabling Timer3B interrupt

    TIMER3_ICR_R |= 0x400; //set bit 10 to clear Timer3B capture mode event interrupt
    TIMER3_IMR_R |= 0x400; //Enable Timer3B capture mode event interrupt

    IntRegister(INT_TIMER3B, TIMER3B_Handler);

    IntMasterEnable();

    // Configure and enable the timer
    TIMER3_CTL_R |= 0x0100; //enables Timer3B and sets event type to both rising and falling edge, commented out above
}

void ping_trigger (void){
    STATE = LOW;
    TIMER3_CTL_R &= ~0x0100; //Disable Timer3B
    TIMER3_IMR_R &= ~0x400; //Disable Timer3B capture mode event interrupt
    // Disable alternate function (disconnect timer from port pin)
    GPIO_PORTB_AFSEL_R &= ~0x08;

    // YOUR CODE HERE FOR PING TRIGGER/START PULSE
    GPIO_PORTB_DIR_R |= 0x08; //Sets PB3 as output by setting bit 3

    GPIO_PORTB_DATA_R &= 0xF7; //Sets data low
    GPIO_PORTB_DATA_R |= 0x08; //Sets PB3 bit 3 for data register for init pulse
    timer_waitMicros(6);
    GPIO_PORTB_DATA_R &= 0xF7; //Clears PB3 bit 3 for data register for end of init pulse

    GPIO_PORTB_DIR_R &= 0xF7; //Sets PB3 as input by clearing bit 3

    // Clear an interrupt that may have been erroneously triggered
    TIMER3_ICR_R |= 0x400;
    // Re-enable alternate function, timer interrupt, and timer
    GPIO_PORTB_AFSEL_R |= 0x08;
    TIMER3_IMR_R |= 0x400;
    TIMER3_CTL_R |= 0x0100;
}

void TIMER3B_Handler(void){

    TIMER3_ICR_R = 0x400; //set bit 10 to clear Timer3B capture mode event interrupt

    if(STATE == LOW){
        risingEdge = TIMER3_TBR_R; //Assigns time where rising edge takes place

        STATE = HIGH; //Sets state to high until falling edge detected
    }
    else if(STATE == HIGH){
        fallingEdge = TIMER3_TBR_R; //Assigns time where falling edge takes place

        STATE = DONE; //Sets state to done, since one loop is done to now can calculate it in ping_getDistance()
    }

}

double ping_getDistance (void){
    ping_trigger();
        while(STATE != DONE){}
        int pwClockCycles = risingEdge - fallingEdge; //Determines pulse width in clock cycles
        if(pwClockCycles < 0){
            overflowCount++;
            pwClockCycles = risingEdge + (0xFFFFFF - fallingEdge);
        }
        float pwMillis = pwClockCycles * (clkStp * 0.000001); //Determines pulse width in milliseconds
        float pwDistanceCm = (343)*((pwMillis * 0.001)/ 2) * 100; //(speed of light in m/s)*(pwMillis * 0.001 / 2)

        char message[100];
        sprintf(message, "PW: %.2d Cycles\nPW: %.2f ms\nDist: %.2f cm\nOverflow: %d", pwClockCycles, pwMillis, pwDistanceCm, overflowCount);
        lcd_printf(message);

        return (double) pwDistanceCm;
}
