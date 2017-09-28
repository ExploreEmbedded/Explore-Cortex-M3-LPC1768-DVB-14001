/***************************************************************************************************
                                   ExploreEmbedded    
****************************************************************************************************
 * File:   rtc.c
 * Version: 15.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: Contains the library functions for RTC- date and time read write

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
***************************************************************************************************/



/***************************************************************************************************
                             Revision History
****************************************************************************************************
15.0: Initial version 
***************************************************************************************************/

#include "rtc.h"
#include "lpc17xx.h"






/***************************************************************************************************
                         void RTC_Init(void)
 ***************************************************************************************************
 * I/P Arguments: none.
 * Return value	: none

 * description :This function is used to Initialize the  RTC.
***************************************************************************************************/
void RTC_Init(void)
{
    LPC_SC->PCONP |=(1<<9); // Enable the power to RTC module 
	/* Disable RTC clock, reset clock, Enable RTC calibration */
	LPC_RTC->CCR = ((1<<SBIT_CTCRST) | (1<<SBIT_CCALEN));
	LPC_RTC->CALIBRATION = 0x00;
    LPC_RTC->CCR = (1<<SBIT_CLKEN); /* Enable the clock for RTC */
}











/***************************************************************************************************
                    void RTC_SetDateTime(rtc_t *rtc)
****************************************************************************************************
 * I/P Arguments: rtc_t *: Pointer to structure of type rtc_t. Structure contains hour,min,sec,day,date,month and year 
 * Return value    : none

 * description  :This function is used to set Date(dd,mm,yy) into the  RTC.
                 The new Date is updated into the non volatile memory of RTC .
		Note: The I/P arguments should of hex or decimal 
	      like 15,08,2047 for 15th day,8th month and 2047th year.				 
***************************************************************************************************/
void RTC_SetDateTime(rtc_t *rtc)
{

     LPC_RTC->SEC   =  rtc->sec;       // Update sec value
	 LPC_RTC->MIN   =  rtc->min;	   // Update min value
     LPC_RTC->HOUR	=  rtc->hour;	   // Update hour value 
	 LPC_RTC->DOW	=  rtc->weekDay;   // Update day value 
	 LPC_RTC->DOM	=  rtc->date;	   // Update date value 
	 LPC_RTC->MONTH =  rtc->month;	   // Update month value
	 LPC_RTC->YEAR  =  rtc->year;	   // Update year value

}







/***************************************************************************************************
                     void RTC_GetDateTime(rtc_t *rtc)
****************************************************************************************************
 * I/P Arguments: rtc_t *: Pointer to structure of type rtc_t. Structure contains hour,min,sec,day,date,month and year 
 * Return value    : none

 * description  :This function is used to get the Time(hh,mm,ss) And Date from  RTC.

	Note: The time read from  will be of hex or decimal, 
	      like 12,39,26 for 12hr,39min and 26sec.	
***************************************************************************************************/
void RTC_GetDateTime(rtc_t *rtc)
{
     rtc->sec     = LPC_RTC->SEC ;	   // Read sec value
	 rtc->min     = LPC_RTC->MIN ;	   // Read min value
     rtc->hour    = LPC_RTC->HOUR;	   // Read hour value 
	 rtc->weekDay = LPC_RTC->DOW;      // Read day value 
	 rtc->date    = LPC_RTC->DOM;	   // Read date value 
	 rtc->month   = LPC_RTC->MONTH;	   // Read month value
	 rtc->year    = LPC_RTC->YEAR;	   // Read year value

}







