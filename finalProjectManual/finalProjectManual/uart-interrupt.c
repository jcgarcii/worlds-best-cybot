/*
 *
 *   uart-interrupt.c
 *
 *
 *
 *   @author
 *   @date
 */

// The "???" placeholders should be the same as in your uart.c file.
// The "?????" placeholders are new in this file and must be replaced.
#include <stdint.h>
#include "uart-interrupt.h"
#include "lcd.h"

// These variables are declared as examples for your use in the interrupt handler.
volatile char command_byte = 't'; // byte value for special character used as a command
volatile int command_flag = 0; // flag to tell the main program a special command was received

void uart_interrupt_init(void)
{

    SYSCTL_RCGCGPIO_R |= 0x02;   //enable clock to GPIO_PORTB
    SYSCTL_RCGCUART_R = 0x02;     //enable clock to UART1
    while ((SYSCTL_PRGPIO_R & 0x02) == 0)
    {
    };
    while ((SYSCTL_PRUART_R & 0x02) == 0)
    {
    }; //Checks to see if bit 1 ready

    GPIO_PORTB_DEN_R |= 0x03;
    GPIO_PORTB_AFSEL_R |= 0x03;
    GPIO_PORTB_PCTL_R &= 0xFFFFFF00;     // Force 0's in the desired locations
    GPIO_PORTB_PCTL_R |= 0x00000011;     // Force 1's in the desired locations

    UART1_CTL_R &= 0xFFFFFFFE;      //disables UART1
    UART1_IBRD_R = 0x8;             //set the integer baud rate period(IBRD = 8)
    UART1_FBRD_R = 0x2C;           //set fractional baud rate period (FBRD = 44)
    UART1_LCRH_R = 0x60;  //set baud rate = 115200, 8-bit data, 1 stop no parity
    UART1_CC_R = 0x0;        //set UARTCC as 00 to select System Clock as source
    UART1_CTL_R |= 0x1;               //enable UART1

    //////Enable interrupts

    //first clear RX interrupt flag (clear by writing 1 to ICR)
    UART1_ICR_R |= 0x10;

    //enable RX raw interrupts in interrupt mask register
    UART1_IM_R |= 0x10; //sets bit 4

    //NVIC setup: set priority of UART1 interrupt to 1 in bits 21-23
    NVIC_PRI1_R = (NVIC_PRI1_R & 0xFF0FFFFF) | 0x00200000;

    //NVIC setup: enable interrupt for UART1, IRQ #6, set bit 6
    NVIC_EN0_R |= 0x40; //sets bit 6

    //tell CPU to use ISR handler for UART1 (see interrupt.h file)
    //from system header file: #define INT_UART1 22
    IntRegister(INT_UART1, UART1_Handler);

    //globally allow CPU to service interrupts (see interrupt.h file)
    IntMasterEnable();

    //re-enable UART1 and also enable RX, TX (three bits)
    //note from the datasheet UARTCTL register description:
    //RX and TX are enabled by default on reset
    //Good to be explicit in your code
    UART1_CTL_R |= 0x301; //sets bits RX(bit 9), TX(bit 8), UART EN(bit 0)
}

void uart_sendChar(char data)
{
    while ((UART1_FR_R & 0x20) != 0)
        ;
    UART1_DR_R = data;
}

char uart_receive(void)
{
    //DO NOT USE this busy-wait function if using RX interrupt
    char rdata;

    while ((UART1_FR_R & 0x10) != 0)
        ;
    rdata = (char) (UART1_DR_R & 0xFF);

    return rdata;
}

void uart_sendStr(const char *data)
{
    int i;
    for (i = 0; i < strlen(data); i++)
    {
        uart_sendChar(data[i]);
    }
}

// Interrupt handler for receive interrupts
void UART1_Handler(void)
{
    char byte_received;
    //check if handler called due to RX event
    if (UART1_MIS_R & 0x10)
    {
        //byte was received in the UART data register
        //clear the RX trigger flag (clear by writing 1 to ICR)
        UART1_ICR_R |= 0x10;

        //read the byte received from UART1_DR_R and echo it back to PuTTY
        //ignore the error bits in UART1_DR_R
        byte_received = UART1_DR_R & 0xFF;
        uart_sendChar(byte_received);

        //if byte received is a carriage return
        if (byte_received == '\r')
        {
            //send a newline character back to PuTTY
            uart_sendChar('\n');
        }
        else
        {
            if (byte_received == command_byte)
            {
                command_flag = 1;
            }
        }
    }
}
