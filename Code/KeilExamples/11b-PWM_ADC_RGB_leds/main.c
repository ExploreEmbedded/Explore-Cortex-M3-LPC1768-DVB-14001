/***************************************************************************************************
                                   ExploreEmbedded
 ****************************************************************************************************
 * File:   main.c
 * Version: 16.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: This file contains the program to demonstrate the RBG color change depending on ADC values. 
                     By varying the ADC value Different colors can be genrated.

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
#include "adc.h"
#include "gpio.h"
#include "stdutils.h"
#include "delay.h"
#include "uart.h"

#define RED_LED    PWM_1
#define GREEN_LED  PWM_2
#define BLUE_LED   PWM_3


/* start the main program */
int main() 
{
    int red_value,green_value,blue_value;
    SystemInit();   //Clock and PLL configuration 
    ADC_Init();
    UART0_Init(9600);
    PWM_Init(255);     /* Initialize the PWM module and the Cycle time(Ton+Toff) is set to 255(similar to arduino) */
    PWM_Start(RED_LED|GREEN_LED|BLUE_LED); // Enable the PWM on P2_0,P2_1,P2_2


    while(1)
    {
        red_value   =  ADC_GetAdcValue(0)/16; // LPC1768 has 12 bit adc and the max value will be 4096  
        green_value =  ADC_GetAdcValue(1)/16; // Since PWM Cycle(Ton+Toff) is 255, the adc value is divide by 16 to get 0-255  
        blue_value  =  ADC_GetAdcValue(2)/16;

        PWM_SetDutyCycle(RED_LED,red_value);
        PWM_SetDutyCycle(GREEN_LED,green_value);
        PWM_SetDutyCycle(BLUE_LED,blue_value);  

        UART0_Printf("Red:%3d Green:%3d  Blue:%3d\n\r",red_value,green_value,blue_value);            
    }                             
}

