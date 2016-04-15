/***************************************************************************************************
                                   ExploreEmbedded    
****************************************************************************************************
 * File:   main.c
 * Version: 16.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: File contains the example to demonstarte the Edge and Level triggered interrupts.


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
#include <lpc17xx.h>
#include "stdutils.h"
#include "gpio.h"
#include "extintr.h"
#include "lcd.h"

volatile uint32_t cnt1=0,cnt2=0;

void myExtIntrIsr_0(void)
{
    cnt1++;               /* Increment cnt1 every time EINT0 is detected */ 
}

void myExtIntrIsr_1(void)
{
    cnt2++;               /* Increment cnt2 every time EINT1 is detected */ 
}


int main (void) 
{
    SystemInit();

    /*        RS   RW   EN   D0   D1   D2   D3   D4    D5    D6    D7      P_NC(Not connected)*/
    LCD_SetUp(P2_0,P2_1,P2_2,P_NC,P_NC,P_NC,P_NC,P1_24,P1_25,P1_26,P1_27); 
    LCD_Init(2,16);

    /* EINT0 is configured as FallingEdge interrupt and myExtIntrIsr_0 will be called by EINT0_IRQHandler */
    EINT_AttachInterrupt(EINT0,myExtIntrIsr_0,FALLING);  

    /* EINT1 is configured as Active Low interrupt and myExtIntrIsr_1 will be called by EINT1_IRQHandler */
    EINT_AttachInterrupt(EINT1,myExtIntrIsr_1,LOW);

    while(1)
    {
        LCD_GoToLine(0);
        LCD_Printf("EINT0=%8u EINT1:%8u",cnt1,cnt2); /* Display the occurrence of EINT0 and EINT1 */
    }
}
