/***************************************************************************************************
                                   ExploreEmbedded	
 ****************************************************************************************************
 * File:   keypad.h
 * Version: 16.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: Contains hex-keypad port configurations

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



/***************************************************************************************************
                             Revision History
 ****************************************************************************************************
15.0: Initial version 
16.0: KEYPAD_Init function is updated to specify the row/col connections as parameters.
      Included the gpio functions to read/write the individual row/col.
 ***************************************************************************************************/
#ifndef _KEYPAD_H
#define _KEYPAD_H

#include "gpio.h"
#include "stdutils.h"


/***************************************************************************************************
                                 Hex-Keypad Row/Col Configuration
 ***************************************************************************************************/
#define C_MaxRows_U8 (4)   
#define C_MaxCols_U8 (4)                 
#define C_DebounceTimeInMicroSecond_U16 (10)
#define C_DefaultKey_U8  ('x')
/**************************************************************************************************/




/***************************************************************************************************
                             Function Prototypes
 ***************************************************************************************************/
uint8_t KEYPAD_GetKey(void);
void KEYPAD_Init(
        gpioPins_et row_0, 
        gpioPins_et row_1, 
        gpioPins_et row_2,
        gpioPins_et row_3, 
        gpioPins_et col_0,
        gpioPins_et col_1, 
        gpioPins_et col_2,
        gpioPins_et col_3 );
/**************************************************************************************************/

#endif
