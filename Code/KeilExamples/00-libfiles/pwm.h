/***************************************************************************************************
                                   ExploreEmbedded    
****************************************************************************************************
 * File:   pwm.h
 * Version: 15.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: Contains the library function for PWM generation for lpc1768
 
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
#ifndef __PWM_H 
#define __PWM_H
#include "stdutils.h"



/*************************************************************************************************
                           PWM Config Bit Positions
*************************************************************************************************/
#define PWM_1   1u   /* ((uint32_t)1<<0) */
#define PWM_2   2u   /* ((uint32_t)1<<1) */
#define PWM_3   4u   /* ((uint32_t)1<<2) */
#define PWM_4   8u   /* ((uint32_t)1<<5) */
#define PWM_5  16u   /* ((uint32_t)1<<4) */
#define PWM_6  32u   /* ((uint32_t)1<<5) */

#define PWM_CHANNEL_MASK  0x3Fu /* 00111111- All 6pwm channels selected */
/*************************************************************************************************/



/*************************************************************************************************
                           PWM Config Bit Positions
*************************************************************************************************/
#define TCR_CNT_EN  0x00000001
#define TCR_RESET	0x00000002
#define TCR_PWM_EN	0x00000008


#define PWMSEL2			(1 << 2)
#define PWMSEL3			(1 << 3)
#define PWMSEL4			(1 << 4)
#define PWMSEL5			(1 << 5)
#define PWMSEL6			(1 << 6)
#define PWMENA1			(1 << 9)
#define PWMENA2			(1 << 10)
#define PWMENA3			(1 << 11)
#define PWMENA4			(1 << 12)
#define PWMENA5			(1 << 13)
#define PWMENA6			(1 << 14)

#define LER0_EN			(1 << 0)
#define LER1_EN			(1 << 1)
#define LER2_EN			(1 << 2)
#define LER3_EN			(1 << 3)
#define LER4_EN			(1 << 4)
#define LER5_EN			(1 << 5)
#define LER6_EN			(1 << 6)
/*************************************************************************************************/
#define PWM_CYCLE		255






/***************************************************************************************************
                             Function prototypes
***************************************************************************************************/
void PWM_Init(uint32_t cycleTime);
void PWM_SetDutyCycle( uint32_t pin, uint32_t dutyCycle );
void PWM_Start(uint32_t pin);
void PWM_Stop(uint32_t pin);
/**************************************************************************************************/

#endif /* end __PWM_H */
