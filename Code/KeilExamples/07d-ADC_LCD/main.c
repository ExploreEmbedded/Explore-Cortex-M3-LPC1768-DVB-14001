/***************************************************************************************************
                                   ExploreEmbedded
****************************************************************************************************
 * File:   main.c
 * Version: 16.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: This file contains the program to read 10bit ADC value from channel 0 and display the Raw and Equivalent volt on LCD.

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
#include "LPC17xx.h"
#include "adc.h"
#include "lcd.h"
#include "delay.h"		  


int main() 
{
    int adcValue;
    float volt;
    SystemInit();
    ADC_Init();       /* Initialize the ADC module, Note: Max ADC Input voltage 3.3v */
    
    /*Connect RS, RW, EN and data bus to PORT0.4 to PORT0.7*/
    LCD_SetUp(P2_0,P2_1,P2_2,P_NC,P_NC,P_NC,P_NC,P1_24,P1_25,P1_26,P1_27);
    LCD_Init(2,16);
    
    while(1)
    {
        adcValue = ADC_GetAdcValue(0); // Read the ADC value of channel zero, Max ADC voltage 3.3v
        volt = (adcValue*3.3)/4095;	   // ADC_REF Voltage=3.3v
        LCD_GoToLine(0);
        LCD_Printf("ADC0 Value:%4d\nVolt:%f",adcValue,volt);     // Display Raw adc value and Equivalent temp on LCD
    }
    
 
}


