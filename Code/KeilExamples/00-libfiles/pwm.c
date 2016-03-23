/***************************************************************************************************
                                   ExploreEmbedded    
****************************************************************************************************
 * File:   pwm.c
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
#include "lpc17xx.h"
#include "pwm.h"
#include "gpio.h"
#include "stdutils.h"


static uint32_t pwmInitDoneFlag_u32 = 0;

/***************************************************************************************************
                         void PWM_Init(void)
****************************************************************************************************
 * I/P Arguments: none.
 * Return value    : none

 * description :This function initializes the internal PWM1 module of lpc1768.
                By default the pwm  cycle(Ton+Toff) is set 255 to match the arduino style.
***************************************************************************************************/
void PWM_Init(void)
{

    LPC_PWM1->TCR = TCR_RESET;  /* Counter Reset */ 
    LPC_PWM1->PR = 0x0;         /* count frequency:Fpclk */
    LPC_PWM1->MCR = 02;         /* Reset TC whenever it matches MR0. MR0 hold the Cycle time(Ton+Toff) */                
    LPC_PWM1->MR0 = 255;        /* set PWM cycle(Ton+Toff) to 255 (similar to arduino) */
    LPC_PWM1->LER = LER0_EN; 
    PWM_Start();
}




/***************************************************************************************************
                         void PWM_SetDutyCycle( uint32_t pin, uint32_t dutyCycle )
****************************************************************************************************
 * I/P Arguments: uint32_t: Pin number at which PWM needs to be generated.
                  uint32_t : required dutyCycle at the pwm pin.
 * Return value    : none

 * description :This function initializes the internal PWM1 module of lpc1768.
                By default the pwm  cycle(Ton+Toff) is set 255 to match the arduino style.
***************************************************************************************************/
void PWM_SetDutyCycle( uint32_t pin, uint32_t dutyCycle )
{  

   if(pwmInitDoneFlag_u32 == 0)
   {
       PWM_Init();
       pwmInitDoneFlag_u32 = 1;
   }       
   switch(pin)
   {
    case P2_0:
            GPIO_PinFunction(P2_0,PINSEL_FUNC_1);
            LPC_PWM1->MR1 = dutyCycle;
            LPC_PWM1->LER|= LER1_EN; 
            break;
    
    case P2_1:
            GPIO_PinFunction(P2_1,PINSEL_FUNC_1);
            LPC_PWM1->MR2 = dutyCycle;
            LPC_PWM1->LER|= LER2_EN; 
            break;
            
    case P2_2:
            GPIO_PinFunction(P2_2,PINSEL_FUNC_1);
            LPC_PWM1->MR3 = dutyCycle;
            LPC_PWM1->LER|= LER3_EN; 
            break;

    case P2_3:
            GPIO_PinFunction(P2_3,PINSEL_FUNC_1);
            LPC_PWM1->MR4 = dutyCycle;
            LPC_PWM1->LER|= LER4_EN; 
            break;

    case P2_4:
            GPIO_PinFunction(P2_4,PINSEL_FUNC_1);
            LPC_PWM1->MR5 = dutyCycle;
            LPC_PWM1->LER|= LER5_EN; 
            break;

    case P2_5:
            GPIO_PinFunction(P2_5,PINSEL_FUNC_1);
            LPC_PWM1->MR6 = dutyCycle;
            LPC_PWM1->LER|= LER6_EN; 
            break;

    default :
            break;            
     }
}        




/***************************************************************************************************
                         void PWM_Start(void)
****************************************************************************************************
 * I/P Arguments: none.
 * Return value    : none

 * description :This function is used to start the pwm generation..
***************************************************************************************************/
void PWM_Start(void)
{
    /* All single edge, all enable */
    LPC_PWM1->PCR = PWMENA1 | PWMENA2 | PWMENA3 | PWMENA4 | PWMENA5 | PWMENA6;
    LPC_PWM1->TCR = TCR_CNT_EN | TCR_PWM_EN;    /* counter enable, PWM enable */
}




/***************************************************************************************************
                         void PWM_Stop(void)
****************************************************************************************************
 * I/P Arguments: none.
 * Return value    : none

 * description :This function is used to stop the pwm generation..
***************************************************************************************************/
void PWM_Stop(void)
{
    LPC_PWM1->PCR = 0;
    LPC_PWM1->TCR = 0x00;        /* Stop all PWMs */
}


