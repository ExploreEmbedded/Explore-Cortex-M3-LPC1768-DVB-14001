/***************************************************************************************************
                                   ExploreEmbedded
 ****************************************************************************************************
 * File:   main.c
 * Version: 16.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: This file contains the program to write a string at memory location 0x00 and read it back.
 *              The string read and written is transmitted on UART.  

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

#include "uart.h"
#include "eeprom.h"

/* start the main program */
int main() 
{
    unsigned char eeprom_address = 0x00, write_str[] = "Welcome to Pic programming by Explore Embedded";
    unsigned char read_str[50];

    UART_Init(9600);

    UART_Printf("\n\rEeprom Write String: %s", write_str); //Print the message on UART
    EEPROM_WriteString(eeprom_address, write_str); // Write the string at memoryLocation	0x00


    EEPROM_ReadString(eeprom_address, read_str); // Read the string from memoryLocation 0x00
    UART_Printf("\n\rEeprom Read String: %s", read_str); //Print the message on UART

    while (1);

    return 0;
}
