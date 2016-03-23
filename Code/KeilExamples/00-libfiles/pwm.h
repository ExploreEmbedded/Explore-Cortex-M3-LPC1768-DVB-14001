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
#ifdef __cplusplus
extern "C" {
#endif

#ifndef __PWM_H 
#define __PWM_H

#include "stdutils.h"


#define PWM_CYCLE		255
#define PWM_OFFSET		200

#define MR0_INT			(1 << 0)
#define MR1_INT			(1 << 1)
#define MR2_INT			(1 << 2)
#define MR3_INT			(1 << 3)
#define MR4_INT			(1 << 8)
#define MR5_INT			(1 << 9)
#define MR6_INT			(1 << 10)

#define TCR_CNT_EN		0x00000001
#define TCR_RESET		0x00000002
#define TCR_PWM_EN		0x00000008

#define PWMMR0I			(1 << 0)
#define PWMMR0R			(1 << 1)
#define PWMMR0S			(1 << 2)
#define PWMMR1I			(1 << 3)
#define PWMMR1R			(1 << 4)
#define PWMMR1S			(1 << 5)
#define PWMMR2I			(1 << 6)
#define PWMMR2R			(1 << 7)
#define PWMMR2S			(1 << 8)
#define PWMMR3I			(1 << 9)
#define PWMMR3R			(1 << 10)
#define PWMMR3S			(1 << 11)
#define PWMMR4I			(1 << 12)
#define PWMMR4R			(1 << 13)
#define PWMMR4S			(1 << 14)
#define PWMMR5I			(1 << 15)
#define PWMMR5R			(1 << 16)
#define PWMMR5S			(1 << 17)
#define PWMMR6I			(1 << 18)
#define PWMMR6R			(1 << 19)
#define PWMMR6S			(1 << 20)

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

void PWM_Init(void);
void PWM_SetDutyCycle( uint32_t pin, uint32_t dutyCycle );
void PWM_Start(void);
void PWM_Stop(void);

#endif /* end __PWM_H */

#ifdef __cplusplus
}
#endif
/****************************************************************************
**                            End Of File
****************************************************************************/
