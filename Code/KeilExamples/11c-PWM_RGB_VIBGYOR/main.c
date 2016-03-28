
/***************************************************************************************************
                                   ExploreEmbedded
 ****************************************************************************************************
 * File:   main.c
 * Version: 16.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: This file contains the program to demonstrate Rainbow colour generation using RGB Leds.
                     Program gets the reqguired colour from user through UART and generates the colour using PWM.

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
#include "uart.h"

#define RED_LED    PWM_1
#define GREEN_LED  PWM_2
#define BLUE_LED   PWM_3


/* start the main program */
int main() 
{
    char colour;
    SystemInit();   //Clock and PLL configuration 
    UART0_Init(9600);
    PWM_Init(255);     /* Initialize the PWM module and the Cycle time(Ton+Toff) is set to 255(similar to arduino) */
    PWM_Start(RED_LED|GREEN_LED|BLUE_LED); 

    PWM_SetDutyCycle(RED_LED,255);
    PWM_SetDutyCycle(GREEN_LED,0);
    PWM_SetDutyCycle(BLUE_LED,0);   

    UART0_Printf("\n\r Choose a Colour from VIBGYOR");
    while(1)
    {
        colour = UART0_RxChar();

        switch(colour)
        {
        case 'v': /* Violet Color */
        case 'V':
            PWM_SetDutyCycle(RED_LED,148);
            PWM_SetDutyCycle(GREEN_LED,0);
            PWM_SetDutyCycle(BLUE_LED,211);
            break;

        case 'i': /* Indigo Color */
        case 'I':
            PWM_SetDutyCycle(RED_LED,75);
            PWM_SetDutyCycle(GREEN_LED,0);
            PWM_SetDutyCycle(BLUE_LED,130);
            break;


        case 'b': /* Blue Color */
        case 'B':
            PWM_SetDutyCycle(RED_LED,0);
            PWM_SetDutyCycle(GREEN_LED,0);
            PWM_SetDutyCycle(BLUE_LED,255);
            break;


        case 'g': /* Green Color */
        case 'G':
            PWM_SetDutyCycle(RED_LED,0);
            PWM_SetDutyCycle(GREEN_LED,255);
            PWM_SetDutyCycle(BLUE_LED,0);
            break;


        case 'y': /* Yellow Color */
        case 'Y':
            PWM_SetDutyCycle(RED_LED,255);
            PWM_SetDutyCycle(GREEN_LED,255);
            PWM_SetDutyCycle(BLUE_LED,0);
            break;


        case 'o': /* Orange Color */
        case 'O':
            PWM_SetDutyCycle(RED_LED,255);
            PWM_SetDutyCycle(GREEN_LED,127);
            PWM_SetDutyCycle(BLUE_LED,0);
            break;


        case 'r': /* red Color */
        case 'R':
            PWM_SetDutyCycle(RED_LED,255);
            PWM_SetDutyCycle(GREEN_LED,0);
            PWM_SetDutyCycle(BLUE_LED,0);
            break;

        default:  /* Do nothing */
            UART0_Printf("\n\rInvalid Colour ");
            break;

        }                 
    }                             
}
