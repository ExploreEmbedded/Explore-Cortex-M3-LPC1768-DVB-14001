
/***************************************************************************************************
                                   ExploreEmbedded    
****************************************************************************************************
 * File:   extintr.h
 * Version: 16.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: File contains the function prototypes and configuration of LPC1768 External Interrupts.
                 

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


#ifndef _EXT_INTERRUPTS_H_
#define _EXT_INTERRUPTS_H_


#include <LPC17xx.h>
#include "stdutils.h"





/*************************************************************************************************
                         Costants, Structures and Typedefs for timers 							   
**************************************************************************************************/
#define EINT0          0
#define EINT1          1
#define EINT2          2
#define EINT3          3
#define EINT_MAX       4

//#define LOW     0 already defined in stdutils.h 
//#define HIGH    1
#define FALLING 2
#define RISING  3

typedef void (*extnIntrFunPtr)(void);

typedef struct
{
    extnIntrFunPtr userFunction; 
    IRQn_Type IrqNumber;
    uint8_t pinumber; 
}eintConfig_t;

/*************************************************************************************************/




/*************************************************************************************************
                                Function Prototypes 							   
*************************************************************************************************/
void EINT_AttachInterrupt(uint8_t intNumber_u8, extnIntrFunPtr funPtr, uint8_t intMode_u8);
void EINT_Enable(uint8_t timerNumber_u8);
void EINT_Disable(uint8_t timerNumber_u8);     
/*************************************************************************************************/    

#endif
