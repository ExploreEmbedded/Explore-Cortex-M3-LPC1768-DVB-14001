/***************************************************************************************************
                                   ExploreEmbedded  
 ****************************************************************************************************
 * File:   keypad.c
 * Version: 16.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: This files contains the program to read the key pressed on 4x4 hex keypad and display it on LCD.

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
#include "keypad.h"
#include "lcd.h"



int main() 
{
    uint8_t key;
    SystemInit();

    /*        RS   RW   EN   D0   D1   D2   D3   D4    D5    D6    D7      P_NC(Not connected)*/
    LCD_SetUp(P2_0,P2_1,P2_2,P_NC,P_NC,P_NC,P_NC,P1_24,P1_25,P1_26,P1_27); 
    LCD_Init(2,16);

    KEYPAD_Init(P0_0,P0_1,P0_2,P0_3,P0_4,P0_5,P0_6,P0_7);  // Keypad is connected to P2_0 - P2_7

    LCD_Printf("Key Pressed:");
    while (1) 
    {
        key = KEYPAD_GetKey();     // Read the Ascii value of Key
        LCD_GoToLine(1);
        LCD_DisplayChar(key);      // Display the Key pressed on LCD        
    }    
}

