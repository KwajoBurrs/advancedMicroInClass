/*
 * Hezekiah Destinoble
 * Spring 2025
 * 03/28/2025
 * PRG Assignment 4
 *
 * Description:
*******************************************************************************/
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define PORT1 GPIO_PORT_P1

#define PIN0 GPIO_PIN0


uint16_t period;
const uint16_t NUMBEROFLOOPS = 100;

int main(void)
{

    uint16_t x = 0;

    /* Stop Watchdog  */
    WDTCTL = 0x5A80;

    //Configure PMW Output for TA0 CCR1 on P2.4
    P2DIR  |= 0b00010000;
    P2SEL0 |= 0b00010000;

    //Configure
    P7DIR  &= ~BIT7;
    P7SEL0 |= 0b10000000;

    //Configure Timer A0
    TA0CTL = 0b0000001000010110; // (up mode, SMCLK, divider: 1)
    TA0CCR0 = 64000; //PERIOD
    TA0CCR1 = 30000; //DUTY
    TA0CCTL1 = 0b0000100011000000; // (compare mode, toggle/set)

    //Configure Timer A1
    TA1CTL = 0b0000001000100100; // (continuous, SMCLK, divider: 1)
    TA1CCTL1 = 0b0100100100010000; // (capture mode)

    while(x != NUMBEROFLOOPS){


        if((TA1CCTL1 & BIT0) == 1)
        {
            x++;
            TA1CTL = 0b0000001000000100; // Stop TA1
            TA1CCTL1 = 0b00011100100010000; // Disconnect Capture, Input VCC, Clear Int
            TA0CTL &= ~BIT0;// Clear TA0 Int
            period = TA1CCR1;

            while((TA0CTL & BIT0) != 1); // Wait for next TA0 to start again

            TA1CTL = 0b0000001000100100; // Start TA1 with initial config
            TA1CCTL1 = 0b0100100100010000;


        }

    }

    puts("EEL4742/5769 -- Advanced Microprocessors\n");
    puts("Spring 2025");
    puts("Hezekiah Destinoble");
    puts("Date: 03/28/25");
    puts("TA0 Divider is: 1");
    puts("TA1 Divider is: 1");
    printf("The Period is %d\n", period);

    while(1);

} // END OF MAIN

