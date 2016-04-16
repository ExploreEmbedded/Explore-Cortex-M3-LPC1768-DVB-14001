/***************************************************************************************************
                                   ExploreEmbedded    
****************************************************************************************************
 * File:   main.c
 * Version: 16.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: File contains the example program for configuring and using the timer functions.
                 

The libraries have been tested on ExploreEmbedded development boards. We strongly believe that the 
library works on any of development boards for respective controllers. However, ExploreEmbedded 
disclaims any kind of hardware failure resulting out of usage of libraries, directly or indirectly.
Files may be subject to change without prior notice. The revision history contains the information 
related to updates. 


GNU GENERAL PUBLIC LICENSE: 
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>. 

Errors and omissions should be reported to codelibraries@exploreembedded.com
**************************************************************************************************/

#include <lpc17xx.h>
#include "stdutils.h"
#include "gpio.h"
#include "timer.h"

#define LED1      P2_0
#define LED2      P2_1


void  myTimerIsr_0(void)
{
    GPIO_PinToggle(LED1); /* Toggle the LED1 (P2_0) */ 
}

void  myTimerIsr_1(void)
{
    GPIO_PinToggle(LED2); /* Toggle the LED1 (P2_1) */ 
}


int main (void) 
{
    SystemInit();

    GPIO_PinDirection(LED1,OUTPUT);        /* Configure the pins as Output to blink the Leds*/
    GPIO_PinDirection(LED2,OUTPUT);

    TIMER_Init(0,100000);                  /* Configure timer0 to generate 100ms(100000us) delay*/
    TIMER_Init(1,500000);                  /* Configure timer1 to generate 500ms(500000us) delay*/

    TIMER_AttachInterrupt(0,myTimerIsr_0);  /* myTimerIsr_0 will be called by TIMER0_IRQn */
    TIMER_AttachInterrupt(1,myTimerIsr_1);  /* myTimerIsr_1 will be called by TIMER1_IRQn */

    TIMER_Start(0);                         /* Start the Timers */
    TIMER_Start(1);

    while(1)
    {
        //do nothing
    }
}
