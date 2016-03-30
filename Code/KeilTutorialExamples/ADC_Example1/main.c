#include<lpc17xx.h>
#include "lcd.h"        //ExploreEmbedded LCD library which contains the lcd routines
#include "delay.h"      //ExploreEmbedded delay library which contains the delay routines
        
/* Bit positions of ADCR and ADGR registers */ 
#define SBIT_BURST      16u
#define SBIT_START      24u
#define SBIT_PDN        21u
#define SBIT_EDGE       27u 
#define SBIT_DONE       31u
#define SBIT_RESULT     4u
#define SBIT_CLCKDIV    8u


int main()
{
    uint16_t adc_result;
    SystemInit();                              //Clock and PLL configuration

   /* Setup/Map the controller pins for LCD operation 
               RS   RW   EN   D0    D1    D2    D3    D4    D5    D6    D7   */
    LCD_SetUp(P2_0,P2_1,P2_2,P1_20,P1_21,P1_22,P1_23,P1_24,P1_25,P1_26,P1_27);

  /* Specify the LCD type(2x16) for initialization*/
    LCD_Init(2,16);    
    
      
    LPC_SC->PCONP |= (1 << 12);                            /* Enable CLOCK for internal ADC controller */

    LPC_ADC->ADCR = ((1<<SBIT_PDN) | (10<<SBIT_CLCKDIV));  /*Set the clock and Power ON ADC module */

    LPC_PINCON->PINSEL1|= 0x01<<14;      	                 /* Select the P0_23 AD0[0] for ADC function */

    

    while(1)
    {
        
        LPC_ADC->ADCR  |= 0x01;                                /* Select Channel 0 by setting 0th bit of ADCR */
        DELAY_us(10);                                          /* allow the channel voltage to stabilize*/
         
        util_BitSet(LPC_ADC->ADCR,SBIT_START);                 /*Start ADC conversion*/
        
        while(util_GetBitStatus(LPC_ADC->ADGDR,SBIT_DONE)==0); /* wait till conversion completes */
        
        adc_result = (LPC_ADC->ADGDR >> SBIT_RESULT) & 0xfff;  /*Read the 12bit adc result*/
        
        LCD_GoToLine(0);                                       /* Go to First line of 2x16 LCD */
        LCD_Printf("Adc0: %4d",adc_result);                    /* Display 4-digit adc result */  
    }
}
