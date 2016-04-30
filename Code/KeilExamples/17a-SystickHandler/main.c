/***************************************************************************************************
                                   ExploreEmbedded    
****************************************************************************************************
 * File:   main.c
 * Version: 16.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: File contains the example program for configuring and using the Systick timer functions.
                 

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
#include "systick.h"
#include "gpio.h"


void myIsr(void)
{
  GPIO_PinToggle(P2_0);              /* Toggle the P2_0 to measure the time */
}
int main()
{
    SystemInit();
    GPIO_PinDirection(P2_0,OUTPUT);  /* Configure the Led Pin as Output */
    SysTick_Init();                  /* Initialize SysTick for 1ms(default)*/
    SysTick_AttachInterrupt(myIsr);  /* myIsr will be called by SysTick_Handler every ms */
    SysTick_Start();                 /* Start SysTick Timer */

   
   while(1)
   {
         /* Do Nothing */
   }
}
