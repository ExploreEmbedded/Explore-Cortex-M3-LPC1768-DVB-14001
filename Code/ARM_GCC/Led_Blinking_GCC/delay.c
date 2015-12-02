/*---------------------------------------------------------------------------------
                        ARM DELAY library header 
Filename: DELAY.h
Controller: LPC1768
Oscillator: 12 MHz
Author: Explore Embedded
website: www.exploreembedded.com
----------------------------------------------------------------------------------*/

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
    /* At 60Mhz, the below loop introduces
    DELAY of 10 us */
    for(i=0;i<15;i++);
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
