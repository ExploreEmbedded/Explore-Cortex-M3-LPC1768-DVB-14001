#include<lpc17xx.h>
#include "lcd.h"    //User defined LCD library which contains the lcd routines
#include "delay.h"  //User defined library which contains the delay routines
#include "adc.h"
        

int main()
{
   uint16_t pot_value,ldr_value, temp_raw, temp_final;
   float voltage;
   
   SystemInit();                              //Clock and PLL configuration

   /* Setup/Map the controller pins for LCD operation 
               RS   RW   EN    D0    D1    D2   D3     D4   D5    D6    D7*/
    LCD_SetUp(P2_0,P2_1,P2_2,P1_20,P1_21,P1_22,P1_23,P1_24,P1_25,P1_26,P1_27);

  
    LCD_Init(2,16);      /* Specify the LCD type(2x16) for initialization*/
    ADC_Init();          /* Initialize the ADC */
    while(1)
    {
        pot_value  = ADC_GetAdcValue(0); /* Read pot value connect to AD0(P0_23) */
        ldr_value  = ADC_GetAdcValue(1); /* Read LDR value connect to AD1(P0_24) */
        temp_raw   = ADC_GetAdcValue(2); /* Read Temp value connect to AD2(P0_25) */

     /* Converting the raw adc value to equivalent temperature with 3.3v as ADC reference using 12bit resolution.
        Step size of ADC= (3.3v/2^12)= 3.3/4096 = 0.0008056640625 = 0.0806mv
        For every degree celcius the Lm35 provides 10mv voltage change.
        1 degree celcius = 10mv = 10mv/0.0806mv = 12.41 uinits            
        Hence the Raw ADC value can be divided by 12.41 to get equivalent temp
        */        
       
         temp_final = temp_raw/12.41;

        /* Vin = (Adc_value * REF)/ 2^12 */
        voltage = (pot_value * 3.3)/ 4096.0;

        LCD_GoToLine(0);
        LCD_Printf("P:%4d %f",pot_value,voltage);
        LCD_Printf("\nL:%4d T:%4d",ldr_value,temp_final);       
    }
}
