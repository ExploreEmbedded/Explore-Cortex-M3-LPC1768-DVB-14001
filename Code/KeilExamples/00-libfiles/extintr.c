/***************************************************************************************************
                                   ExploreEmbedded    
*****************************************************************************************************
 * File:   extintr.c
 * Version: 16.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: File contains the functions for configuring and using the LPC1768 External Interrupts.


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
#include <lpc17xx.h>
#include "extintr.h"
#include "stdutils.h"
#include "gpio.h"



/**************************************************************************************************
                      EINTconfiguration table(Do not alter uncless required)
***************************************************************************************************/
eintConfig_t EintConfigTable[EINT_MAX] = 
{  /* userFunPtr  IRQ Name   EINT Pin */
        { NULL,      EINT0_IRQn,  P2_10, },
        { NULL,      EINT1_IRQn,  P2_11, },
        { NULL,      EINT2_IRQn,  P2_12, },
        { NULL,      EINT3_IRQn,  P2_13, },
};
/*************************************************************************************************/




/***************************************************************************************************
    void EINT_AttachInterrupt(uint8_t intNumber_u8, extnIntrFunPtr funPtr, uint8_t intMode_u8)
*****************************************************************************************************
 * I/P Arguments: 
                uint8_t: One of the below EINT number(EINT0-EINT3) that needs to be enabled.
                         * EINT0 
                         * EINT1 
                         * EINT2 
                         * EINT3 

                uint8_t: Type of the interrupt from the below options
                        * LOW 
                        * HIGH
                        * FALLING 
                        * RISING                  

               extnIntrFunPtr: Function name thats needs to be called by the ISR.
                              The function parameter and return type should be void as shown below.
                              void MyFunToBeCalledByISR(void);

 * Return value: none

 * description :
                This function enables the EINTx for Edge/level triggerd as per the request.
                After configuring the EINTx, it attaches/hookes the user callback function to ISR.
                Ever time the interrupt occurs the ISR will call the user CallBack function.                  
*****************************************************************************************************/
void EINT_AttachInterrupt(uint8_t intNumber_u8, extnIntrFunPtr funPtr, uint8_t intMode_u8)
{
    if(intNumber_u8<EINT_MAX)
    {
        GPIO_PinFunction(EintConfigTable[intNumber_u8].pinumber,PINSEL_FUNC_1);
        NVIC_EnableIRQ(EintConfigTable[intNumber_u8].IrqNumber);
        EintConfigTable[intNumber_u8].userFunction = funPtr; 
        switch(intMode_u8)
        {            
        case LOW:
            util_BitClear(LPC_SC->EXTMODE,intNumber_u8);
            util_BitClear(LPC_SC->EXTPOLAR,intNumber_u8);
            break; 
            
        case HIGH:
            util_BitClear(LPC_SC->EXTMODE,intNumber_u8);
            util_BitSet(LPC_SC->EXTPOLAR,intNumber_u8);
            break; 

        case FALLING:
            util_BitSet(LPC_SC->EXTMODE,intNumber_u8);
            util_BitClear(LPC_SC->EXTPOLAR,intNumber_u8);
            break; 
            
        case RISING:
            util_BitSet(LPC_SC->EXTMODE,intNumber_u8);
            util_BitSet(LPC_SC->EXTPOLAR,intNumber_u8);
            break; 
        }
    }
}









/***************************************************************************************************
     void EINT_DetachInterrupt(uint8_t intNumber_u8)
*****************************************************************************************************
 * I/P Arguments: 
                uint8_t: One of the below EINT number(EINT_INT0-EINT_INT3) that needs to be disabled. 
 * EINT_INT0 
 * EINT_INT1 
 * EINT_INT2 
 * EINT_INT3                             

 * Return value: none

 * description :
                This function disables the requested EINTx and configures the pin as GPIO.
                After disabling the interrupt, it removes/detaches the user call back function.                  
*****************************************************************************************************/
void EINT_DetachInterrupt(uint8_t intNumber_u8)
{
    if(intNumber_u8<EINT_MAX)
    {
        GPIO_PinFunction(EintConfigTable[intNumber_u8].pinumber,PINSEL_FUNC_0);
        EintConfigTable[intNumber_u8].userFunction = NULL; 
    }
}





/***************************************************************************************************
                            EINTx ISR's
*****************************************************************************************************
 desc: The four ISR's will be called independently whenever the interrupt is detected on EITx pins.

       The ISR will clear the Interrupt bit as it is being served.(Setting the bit will clear the Corresponding ISR Bit)
       If the user CallBack Function is configured then it will be called. 

*****************************************************************************************************/
void EINT0_IRQHandler(void)
{
    util_BitSet(LPC_SC->EXTINT, EINT0);  /* Clear Interrupt Flag */
    if(EintConfigTable[EINT0].userFunction != NULL)
    {
        EintConfigTable[EINT0].userFunction();
    }
}


void EINT1_IRQHandler(void)
{
    util_BitSet(LPC_SC->EXTINT, EINT1);  /* Clear Interrupt Flag */
    if(EintConfigTable[EINT1].userFunction != NULL)
    {
        EintConfigTable[EINT1].userFunction();
    }
}


void EINT2_IRQHandler(void)
{
    util_BitSet(LPC_SC->EXTINT, EINT2);  /* Clear Interrupt Flag */
    if(EintConfigTable[EINT2].userFunction != NULL)
    {
        EintConfigTable[EINT2].userFunction();
    }
}


void EINT3_IRQHandler(void)
{
    util_BitSet(LPC_SC->EXTINT, EINT3);  /* Clear Interrupt Flag */
    if(EintConfigTable[EINT3].userFunction != NULL)
    {
        EintConfigTable[EINT3].userFunction();
    }
}

/*************************************************************************************************
                                    END of  ISR's 
**************************************************************************************************/
