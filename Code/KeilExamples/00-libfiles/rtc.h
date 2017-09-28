/***************************************************************************************************
                                   ExploreEmbedded	
 ***************************************************************************************************
 * File:   rtc_internal.h
 * Version: 15.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: Contains the function prototypes for RTC-Ds1307 date and time read write

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
#ifndef _RTC_H_
#define _RTC_H_

#include"stdutils.h"


/***************************************************************************************************
                                 Struct/Enums used
***************************************************************************************************/
typedef struct
{
  uint8_t sec;
  uint8_t min;
  uint8_t hour;
  uint8_t weekDay;
  uint8_t date;
  uint8_t month;
  uint16_t year;  
}rtc_t;
/**************************************************************************************************/


/***************************************************************************************************
                             Commonly used RTC macros/Constants
***************************************************************************************************/

#define SBIT_CLKEN   	  0		/* RTC Clock Enable*/
#define SBIT_CTCRST	      1		/* RTC Clock Reset */
#define SBIT_CCALEN	      4		/* RTC Calibration counter enable */

#define SBIT_CALDIR    	 17
#define SBIT_PCRTC        9

/**************************************************************************************************/




/***************************************************************************************************
                             Function Prototypes
***************************************************************************************************/
void RTC_Init(void);
void RTC_SetDateTime(rtc_t *rtc);
void RTC_GetDateTime(rtc_t *rtc);
/**************************************************************************************************/

#endif
