/***************************************************************************************************
                                   ExploreEmbedded    
 ****************************************************************************************************
 * File:   delay.c
 * Version: 15.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: File contains the delay routines for us/ms/sec.

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
#include "delay.h"



/*---------------------------------------------------------------------------------
                       void DELAY_us(unsigned int count)
 ----------------------------------------------------------------------------------
 * I/P Arguments: no of ticks (multiple of 1us)
 * Return value	: none

 * description  :
    This function suspends the tasks for specified ticks(in us).

-----------------------------------------------------------------------------------*/

void DELAY_us(unsigned int count)
{
  unsigned int j=0,i=0;

  for(j=0;j<count;j++)
  {
    /* At 100Mhz, the below loop introduces
    DELAY of 1 us */
    for(i=0;i<23;i++);
  }
}



/*---------------------------------------------------------------------------------
                       void DELAY_ms(unsigned int count)
 ----------------------------------------------------------------------------------
 * I/P Arguments: no of ticks (multiple of 1ms)
 * Return value	: none

 * description  :
    This function suspends the tasks for specified ticks(in ms).

-----------------------------------------------------------------------------------*/

void DELAY_ms(unsigned int count)
{
	unsigned int i;
	for (i=0;i<count;i++)
	{
		DELAY_us(1000);
	}
}

 /*---------------------------------------------------------------------------------
                       void DELAY_ms(unsigned int count)
 ----------------------------------------------------------------------------------
 * I/P Arguments: no of ticks (multiple of 1s)
 * Return value	: none

 * description  :
    This function suspends the tasks for specified ticks(in sec).

-----------------------------------------------------------------------------------*/

void DELAY_sec(unsigned int count)
{
	unsigned int i;
	for (i=0;i<count;i++)
	{
		DELAY_ms(1000);
	}
}
