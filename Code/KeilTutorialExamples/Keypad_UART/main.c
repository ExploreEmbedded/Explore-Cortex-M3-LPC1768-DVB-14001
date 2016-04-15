/***************************************************************************************************
                                   ExploreEmbedded	
****************************************************************************************************
 * File:   keypad.c
 * Version: 16.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: The files contains the program to read the key pressed on 4x4 hex keypad and send it on UART.

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
#include "uart.h"


int main() 
{
    uint8_t key;
	SystemInit();
    UART0_Init(9600);                                     // Initialize UART0 at 9600 baud rate
    KEYPAD_Init(P2_0,P2_1,P2_2,P2_3,P2_4,P2_5,P2_6,P2_7); // Keypad is connected to P2_0 - P2_7

    while (1) 
    {
        key = KEYPAD_GetKey();                            // Read the Ascii value of Key
        UART0_Printf("\n\r Key:%c", key);                 // Transmit the key pressed on UART
    }                      
}

