/***************************************************************************************************
                                   ExploreEmbedded    
****************************************************************************************************
 * File:   systickHandler.h
 * Version: 16.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: File contains the function declaration and the SFR bit number for configuring and using the LPC1768 timers.
                 

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
#ifndef _SYSTICK_TIMERS_H_
#define _SYSTICK_TIMERS_H_


#include "LPC17xx.h"
#include "stdutils.h"

/*************************************************************************************************
                             SysTick SFR's and its bits                               
**************************************************************************************************/
#define SBIT_ENABLE     0
#define SBIT_TICKINT    1
#define SBIT_CLKSOURCE  2

      /* Systick Register address, refer datasheet for more info */
#define STCTRL      (*( ( volatile unsigned long *) 0xE000E010 ))
#define STRELOAD    (*( ( volatile unsigned long *) 0xE000E014 ))
#define STCURR      (*( ( volatile unsigned long *) 0xE000E018 ))

/* 100000000Mhz * 1us = 100 */
#define COUNT_PER_US  100

/* 100000000Mhz * 1ms = 1000000 - 1 */
#define COUNT_PER_MS  99999

typedef void (*sysTickCallBackFunPtr)(void);
/*************************************************************************************************/





/*************************************************************************************************
                                Function Prototypes                                
*************************************************************************************************/
void SysTick_Init(void);
void SysTick_Start(void);
void SysTick_Stop(void);
void SysTick_AttachInterrupt(sysTickCallBackFunPtr funPtr);
uint32_t SysTick_GetMsTime(void);
uint32_t SysTick_GetUsTime(void);     
/*************************************************************************************************/


#endif
