/***************************************************************************************************
                                   ExploreEmbedded
 ****************************************************************************************************
 * File:   main.c
 * Version: 16.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: This file contains the program to read the switch status and display on the Leds. 

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
#include "gpio.h"

#define LED1 P2_0
#define LED2 P2_1
#define LED3 P2_2

#define SWITCH1 P2_12
#define SWITCH2 P2_10
#define SWITCH3 P2_11

int main() 
{
    GPIO_PinDirection(LED1,OUTPUT);               /* Configure LED pins as output */
    GPIO_PinDirection(LED2,OUTPUT);
    GPIO_PinDirection(LED3,OUTPUT);

    GPIO_PinDirection(SWITCH1,INPUT);             /* Configure Switch pins as input */
    GPIO_PinDirection(SWITCH2,INPUT);
    GPIO_PinDirection(SWITCH3,INPUT);

    while(1)
    {
        GPIO_PinWrite(LED1,GPIO_PinRead(SWITCH1)); /* Read switch status and turn ON/OFF Leds accordingly */
        GPIO_PinWrite(LED2,GPIO_PinRead(SWITCH2));   
        GPIO_PinWrite(LED3,GPIO_PinRead(SWITCH3));            
    }
}

