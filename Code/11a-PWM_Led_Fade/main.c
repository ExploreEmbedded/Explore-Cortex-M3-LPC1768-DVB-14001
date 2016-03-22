/***************************************************************************************************
                                   ExploreEmbedded
****************************************************************************************************
 * File:   main.c
 * Version: 16.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: This file contains the program to demonstrate the LED Fading depending on PWM duty Cycle. 

The libraries have been tested on ExploreEmbedded development boards. We strongly believe that the
library works on any of development boards for respective controllers. However, ExploreEmbedded
disclaims any kind of hardware failure resulting out of usage of libraries, directly or indirectly.
Files may be subject to change without prior notice. The revision history contains the information
related to updates.


GNU GENERAL PUBLIC LICENSE:
    Copyright (C) 2012  ExploreEmbedded

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
#include "pwm.h"
#include "gpio.h"
#include "stdutils.h"
#include "delay.h"



/* start the main program */
int main() 
{
	  int i;
    SystemInit();   //Clock and PLL configuration 
    PWM_Init();     /* Initialize the PWM module and the Cycle time(Ton+Toff) is set to 255(similar to arduino) */
	  PWM_Start(); 

  while(1)
    {

       for(i=0;i<255;i++) /* Increase the Brightness of the Leds */
			{
				PWM_SetDutyCycle(P2_0,i);
				PWM_SetDutyCycle(P2_1,i);
				PWM_SetDutyCycle(P2_2,i);
				PWM_SetDutyCycle(P2_3,i);
				DELAY_ms(5);
			}
			
			for(i=255;i>0;i--) /* Decrease the Brightness of the Leds */
			{
				PWM_SetDutyCycle(P2_0,i);
				PWM_SetDutyCycle(P2_1,i);
				PWM_SetDutyCycle(P2_2,i);
				PWM_SetDutyCycle(P2_3,i);
				DELAY_ms(5);
			}
    }							  
}

