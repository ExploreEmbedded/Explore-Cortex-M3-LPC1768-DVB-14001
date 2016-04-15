/***************************************************************************************************
                                   ExploreEmbedded
 ****************************************************************************************************
 * File:   main.c
 * Version: 16.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: This file contains the program to write A-Z at memory location 0x00 and read it back.
 *              The data read and written is transmitted on UART.  

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
#include "uart.h"
#include "eeprom.h"
#include "gpio.h"

/* start the main program */
int main() 
{
    unsigned char eeprom_address = 0x00, write_char = 'A', read_char;
    SystemInit();
    UART0_Init(9600);

    /* Eeprom IC:AT24C16   SDA:P2_0    SCL:P2_1 */        
    EEPROM_Init(AT24C16,P2_0,P2_1);

    for(write_char='A';write_char<='Z';write_char++)
    {
        UART0_Printf("Eeprom Write: %c    ",write_char); //Print the message on UART
        EEPROM_WriteByte(eeprom_address, write_char);    // Write the data at memoryLocation    0x00  

        read_char = EEPROM_ReadByte(eeprom_address);     // Read the data from memoryLocation 0x00
        UART0_Printf("Eeprom Read: %c\n\r",read_char);   //Print the message on UART
    }

    while (1);    
}
