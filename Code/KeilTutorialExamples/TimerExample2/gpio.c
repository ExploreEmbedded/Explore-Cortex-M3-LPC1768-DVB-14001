/***************************************************************************************************
                                   ExploreEmbedded    
 ****************************************************************************************************
 * File:   gpio.h
 * Version: 15.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: File contains the functions for accessing the GPIO pins of a controller.
                First the GPIO pins needs to configured(Input/Output) and later used accordingly.

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




/***************************************************************************************************
                void GPIO_PinFunction(uint8_t v_pinNumber_u8, uint8_t v_pinFunction_u8)
 ****************************************************************************************************
 * I/P Arguments: 
                uint8_t: pin number for which the pin function needs to be selected.
                             .
                v_pinFunction_u8: Any of the below four pins functions.                                  
                                 PINSEL_FUNC_0
                                 PINSEL_FUNC_1
                                 PINSEL_FUNC_2
                                 PINSEL_FUNC_3    
                                 Refer lpc1768 data sheet for more info. 


 * Return value    : none

 * description :This function sets the specified PinFunction for the selected pin.
                Refer the lpc1768 data sheet for complete pin functions.  
 ****************************************************************************************************/
void GPIO_PinFunction(uint8_t v_pinNumber_u8, uint8_t v_pinFunction_u8)
{
    uint8_t v_portNumber_u8;
    uint32_t *ptr_PINCON;

    if((v_pinNumber_u8 < P_MAX) && (v_pinFunction_u8 <= PINSEL_FUNC_3)) //Handle the request only if it is in range
    {
        v_portNumber_u8 =  (v_pinNumber_u8>>5);  //Divide the pin number by 32 go get the PORT number
        v_pinNumber_u8  =   v_pinNumber_u8 & 0x1f;  //lower 5-bits contains the bit number of a 32bit port  


        v_portNumber_u8 = v_portNumber_u8*2;
        if(v_pinNumber_u8>=16)
        {
            v_portNumber_u8++; 
            v_pinNumber_u8 = v_pinNumber_u8-16;
        }

        v_pinNumber_u8 = v_pinNumber_u8 * 2;

        ptr_PINCON    = ((uint32_t *)&LPC_PINCON->PINSEL0  + v_portNumber_u8);

        *(uint32_t *)(ptr_PINCON) &= ~(0x03UL << v_pinNumber_u8);
        *(uint32_t *)(ptr_PINCON) |= (((uint32_t)v_pinFunction_u8) << v_pinNumber_u8);
    }
}



/***************************************************************************************************
                  void GPIO_PinDirection(uint8_t v_pinNumber_u8, gpioDirection_et enm_pinDirn)
 ****************************************************************************************************
 * I/P Arguments: 
                uint8_t: pin number which needs to be updated.
                             Refer the gpio.h for pin numbering.
                gpioDirection_et: Direction to be configured ie. INPUT/OUTPUT.


 * Return value    : none

 * description :This function sets the specified direction as INPUT/OUTPUT.  
 ****************************************************************************************************/
void GPIO_PinDirection(uint8_t v_pinNumber_u8, uint8_t v_pinDirn_u8)
{

    uint8_t v_portNumber_u8;
    LPC_GPIO_TypeDef *LPC_GPIO_PORT;

    if(v_pinNumber_u8 < P_MAX)               //Handle the request only if it is in range
    {
        v_portNumber_u8 =  (v_pinNumber_u8>>5);  //Divide the pin number by 32 go get the PORT number
        v_pinNumber_u8  =   v_pinNumber_u8 & 0x1f;  //lower 5-bits contains the bit number of a 32bit port  

        /* Go to particular port after decoding from the pin number and 
        set the direction as specified*/

        LPC_GPIO_PORT = (LPC_GPIO_TypeDef*)(LPC_GPIO_BASE + ((v_portNumber_u8) << 5));
        util_UpdateBit(LPC_GPIO_PORT->FIODIR,v_pinNumber_u8,v_pinDirn_u8);   
    }
}









/***************************************************************************************************
            void GPIO_PinWrite(uint8_t v_pinNumber_u8, uint8_t v_pinValue_u8)
 ****************************************************************************************************
 * I/P Arguments: 
                uint8_t: pin number which needs to be updated.
                             Refer the gpio.h for pin numbering.
                uint8_t: Value to be updated(LOW/HIGH) on the selected pin.


 * Return value    : none

 * description :This function updates the specified value on the selected pin.  
                Before updating the pins status, the pin function should be selected and then
                the pin should be configured as OUTPUT 
 ****************************************************************************************************/
void GPIO_PinWrite(uint8_t v_pinNumber_u8, uint8_t v_pinValue_u8)
{

    uint8_t v_portNumber_u8;
    LPC_GPIO_TypeDef *LPC_GPIO_PORT;
    if(v_pinNumber_u8 < P_MAX)               //Handle the request only if it is in range
    {
        v_portNumber_u8 =  (v_pinNumber_u8>>5);  //Divide the pin number by 32 go get the PORT number
        v_pinNumber_u8  =   v_pinNumber_u8 & 0x1f;  //lower 5-bits contains the bit number of a 32bit port  

        /* Go to particular port after decoding from the pin number and 
        update the value of the specified pin*/

        LPC_GPIO_PORT = (LPC_GPIO_TypeDef*)(LPC_GPIO_BASE + ((v_portNumber_u8) << 5));

        if(v_pinValue_u8!=0)
        {
            util_BitSet(LPC_GPIO_PORT->FIOSET,v_pinNumber_u8);        
        }
        else
        {
            util_BitSet(LPC_GPIO_PORT->FIOCLR,v_pinNumber_u8); 
        }
    }
}




/***************************************************************************************************
            void GPIO_PinToggle(uint8_t v_pinNumber_u8)
 ****************************************************************************************************
 * I/P Arguments: 
                uint8_t: pin number which needs to be toggled.
                             Refer the gpio.h for pin numbering.


 * Return value    : none

 * description :This function toggles the selected pin.  
                Before updating the pins status, the pin function should be selected and then
                the pin should be configured as OUTPUT 
 ****************************************************************************************************/
void GPIO_PinToggle(uint8_t v_pinNumber_u8)
{

    uint8_t v_portNumber_u8;
    LPC_GPIO_TypeDef *LPC_GPIO_PORT;

    if(v_pinNumber_u8 < P_MAX)               //Handle the request only if it is in range
    {
        v_portNumber_u8 =  (v_pinNumber_u8>>5);     //Divide the pin number by 32 go get the PORT number
        v_pinNumber_u8  =   v_pinNumber_u8 & 0x1f;  //lower 5-bits contains the bit number of a 32bit port  

        /* Go to particular port after decoding from the pin number and 
        update the value of the specified pin*/

        LPC_GPIO_PORT = (LPC_GPIO_TypeDef*)(LPC_GPIO_BASE + ((v_portNumber_u8) << 5));

        util_BitToggle(LPC_GPIO_PORT->FIOPIN,v_pinNumber_u8);      
    }          
}





/***************************************************************************************************
                    uint8_t GPIO_PinRead(uint8_t v_pinNumber_u8)
 ****************************************************************************************************
 * I/P Arguments: 
                uint8_t: pin number which needs to be read.
                             Refer the gpio.h for pin numbering.

 * Return value    : 
                 uint8_t:    Status of the specified pin.

 * description :This function returns the status of the selected pin.
                Before reading the pins status, the pin function should be selected and accordingly
                the pin should be configured as INPUT 
 ****************************************************************************************************/
uint8_t GPIO_PinRead(uint8_t v_pinNumber_u8)
{
    uint8_t returnStatus = 0;
    uint8_t v_portNumber_u8;
    LPC_GPIO_TypeDef *LPC_GPIO_PORT; 

    if(v_pinNumber_u8 < P_MAX)               //Handle the request only if it is in range
    {
        v_portNumber_u8 =  (v_pinNumber_u8>>5);  //Divide the pin number by 32 go get the PORT number
        v_pinNumber_u8  =   v_pinNumber_u8 & 0x1f;  //lower 5-bits contains the bit number of a 32bit port  

        /* Go to particular port after decoding from the pin number and read the pins status */

        LPC_GPIO_PORT = (LPC_GPIO_TypeDef*)(LPC_GPIO_BASE + ((v_portNumber_u8) << 5));    
        returnStatus = util_IsBitSet(LPC_GPIO_PORT->FIOPIN,v_pinNumber_u8);
    }

    return returnStatus;
}
