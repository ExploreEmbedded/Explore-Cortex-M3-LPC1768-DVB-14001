/***************************************************************************************************
                                   ExploreEmbedded
****************************************************************************************************
 * File:   main.c
 * Version: 16.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: This file contains the program to transmit and receive data using USB-Virtual COM

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
#include "lpc17xx.h"
#include "usbserial.h"
#include "gpio.h"
#include "delay.h"

#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"
#include "usbcore.h"
#include "cdc.h"
#include "cdcuser.h"


extern void SystemCoreClockUpdate(void);
int main() 
{
	  SystemInit();
	SystemCoreClockUpdate();
	  GPIO_PinDirection(P2_0,OUTPUT);
	GPIO_PinDirection(P2_1,OUTPUT);
	GPIO_PinDirection(P2_2,OUTPUT);
	GPIO_PinDirection(P2_3,OUTPUT);
	
	GPIO_PinWrite(P2_0,1);
    UsbSerial_Init(9600);
	GPIO_PinWrite(P2_1,1);
    
    while(1)
    {
        UsbSerial_TxString(" Welcome to ARM Serial Programming by ExploreEmbedded\n\r");
    }
    
}

