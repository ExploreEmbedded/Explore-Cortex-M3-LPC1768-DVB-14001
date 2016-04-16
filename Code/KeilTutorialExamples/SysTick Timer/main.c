/***************************************************************************************************
                                   ExploreEmbedded    
****************************************************************************************************
 * File:   main.c
 * Version: 16.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: File contains the example program for configuring and using the Systick timer.
                 

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

#include <LPC17xx.h>    


/* Systick Register address, refer datasheet for more info */
#define STCTRL      (*( ( volatile unsigned long *) 0xE000E010 ))
#define STRELOAD    (*( ( volatile unsigned long *) 0xE000E014 ))
#define STCURR      (*( ( volatile unsigned long *) 0xE000E018 ))  

/*******STCTRL bits*******/
#define SBIT_ENABLE     0
#define SBIT_TICKINT    1
#define SBIT_CLKSOURCE  2


/* 100000000Mhz * 1ms = 1000000 - 1 */
#define RELOAD_VALUE  99999

#define LED   0 //P2_0


int main (void) 
{
    SystemInit();

    STRELOAD = RELOAD_VALUE;    // Reload value for 1ms tick

    /* Enable the Systick, Systick Interrup and select CPU Clock Source */
    STCTRL = (1<<SBIT_ENABLE) | (1<<SBIT_TICKINT) | (1<<SBIT_CLKSOURCE);


    LPC_GPIO2->FIODIR = (1<<LED);            /* Configure the Led Pin as Output */
  
    while(1)
    {
        //do nothing
    }
}

void SysTick_Handler(void)
{
    LPC_GPIO2->FIOPIN ^= (1<<LED); /* Toggle the LED1 (P2_0) */
}
