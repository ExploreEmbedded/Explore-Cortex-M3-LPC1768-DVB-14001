/***************************************************************************************************
                                   ExploreEmbedded	
****************************************************************************************************
 * File:   adc.c
 * Version: 15.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: Contains the library function for ADC conversion
 
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
***************************************************************************************************/
#include"adc.h"
#include"delay.h"
#include "stdutils.h"
#include "gpio.h"





const adcChannelConfig_st AdcConfig[C_MaxAdcChannels_U8]=
{
  { P0_23, PINSEL_FUNC_1}, /* AD0[0] is on P0.23 second alternative function */
  { P0_24, PINSEL_FUNC_1}, /* AD0[1] is on P0.24 second alternative function */
  { P0_25, PINSEL_FUNC_1}, /* AD0[2] is on P0.25 second alternative function */
  { P0_26, PINSEL_FUNC_1}, /* AD0[3] is on P0.26 second alternative function */
  { P1_30, PINSEL_FUNC_3}, /* AD0[4] is on P1.30 third alternative function */
  { P1_31, PINSEL_FUNC_3}, /* AD0[5] is on P1.31 third alternative function */
  { P0_3,  PINSEL_FUNC_2}, /* AD0[6] is on P0.3  third alternative function */
  { P0_2,  PINSEL_FUNC_2}  /* AD0[7] is on P0.2  third alternative function */
};

static uint32_t adc_GetPclk(void);

/***************************************************************************************************
                         void ADC_Init()
****************************************************************************************************
 * I/P Arguments: none.
 * Return value	: none

 * description :This function initializes the ADC module.
				Refer adc.h for ADC configuration

***************************************************************************************************/
void ADC_Init()
{
	 uint32_t v_Pclk_u32;
	 
   /* Enable CLOCK for to controller */
	LPC_SC->PCONP |= (1 << 12);

	v_Pclk_u32 = adc_GetPclk();
	v_Pclk_u32 = M_GetClkDivValue(v_Pclk_u32);

	LPC_ADC->ADCR = ((1<<SBIT_PDN) | v_Pclk_u32);  // no START, no BURST, ADC normal operation, with selected channel

}
  
  
  
/***************************************************************************************************
                         uint16_t ADC_GetAdcValue(uint8_t v_adcChannel_u8)
****************************************************************************************************
 * I/P Arguments: uint8_t(channel number).
 * Return value	: uint16_t(12 bit ADC result)

 * description  :This function does the ADC conversion for the Selected Channel
                 and returns the converted 12 bit result
                 The ADC value per bit depends on the resolution of the ADC.
***************************************************************************************************/				 
uint16_t ADC_GetAdcValue(uint8_t v_adcChannel_u8)
 {
	uint16_t v_adcResult_u16;
    if(v_adcChannel_u8 < C_MaxAdcChannels_U8)
    {
       /* Select channel is with range, COnfigure the channel for ADC and DO the A/D conversion */ 
	   GPIO_PinFunction(AdcConfig[v_adcChannel_u8].pinNumber,AdcConfig[v_adcChannel_u8].PinFunSel);
       LPC_ADC->ADCR  = (LPC_ADC->ADCR  & 0xFFFFFF00) | (0x01 << v_adcChannel_u8 );		/* set the channel */
	   
       DELAY_us(10);		/* allow the channel voltage to stabilize*/
       
	   util_BitSet(LPC_ADC->ADCR,SBIT_START);			/*Start ADC conversion*/  	
	   while(util_GetBitStatus(LPC_ADC->ADGDR,SBIT_DONE)==0);	/* wait till conversion completes */
	   
	   v_adcResult_u16 = (LPC_ADC->ADGDR >> SBIT_RESULT) & 0xfff; /*Read the 12bit adc result*/
    }
    else
    {
        /* Channel is out of range, return 0*/
      v_adcResult_u16 = 0;  
    }        
	
	return(v_adcResult_u16);                        			/* Return the 12-bit result */
 }


 
 

/***************************************************************************************************
						static uint32_t adc_GetPclk( void )
 ***************************************************************************************************
 * I/P Arguments: -
 * Return value	: v_Pclk_u32 in terms of system frequency

 * description  :This function is used to calculate pclock in terms of system frequency.

***************************************************************************************************/

static uint32_t adc_GetPclk(void)
{
    uint32_t v_AdcPclk_u32,v_Pclk_u32;
	/** 
	   PCLKSELx registers contains the PCLK info for all the clock dependent peripherals.
	   Bit6,Bit7 contains the Uart Clock(ie.UART_PCLK) information.
	   The UART_PCLK and the actual Peripheral Clock(PCLK) is calculated as below.
	   (Refer data sheet for more info)
	   
	   ADC_PCLK    PCLK
	     0x00       SystemFreq/4        
		 0x01       SystemFreq
		 0x02       SystemFreq/2
		 0x03       SystemFreq/8   
	 **/

	v_AdcPclk_u32 = (LPC_SC->PCLKSEL0 >> 6) & 0x03;
	
	switch( v_AdcPclk_u32 )
	{
	      case 0x00:
			v_Pclk_u32 = SystemCoreClock/4;
			break;
		  case 0x01:
			v_Pclk_u32 = SystemCoreClock;
			break; 
		  case 0x02:
			v_Pclk_u32 = SystemCoreClock/2;
			break; 
		  case 0x03:
			v_Pclk_u32 = SystemCoreClock/8;
			break;
	}
	return (v_Pclk_u32);
}
