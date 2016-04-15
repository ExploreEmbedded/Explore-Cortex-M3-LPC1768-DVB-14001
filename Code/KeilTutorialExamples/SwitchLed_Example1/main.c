#include <lpc17xx.h>   

#define SwitchPinNumber 11
#define LedPinNumber    0

/* start the main program */
int main() 
{
    uint32_t switchStatus;
    SystemInit();                             /* Clock and PLL configuration  */
    LPC_PINCON->PINSEL2 = 0x000000;           /* Configure the Pins for GPIO  */
    /* Configure the LED pin as output and SwitchPin as input */
    LPC_GPIO2->FIODIR = ((1<<LedPinNumber) | (0<<SwitchPinNumber)); 

    while(1)
    {
        /* Turn On all the leds and wait for one second */ 
        switchStatus = (LPC_GPIO2->FIOPIN>>SwitchPinNumber) & 0x01 ;  /* Read the switch status */

        if(switchStatus == 1)                        /* Turn ON/OFF LEDs depending on switch status */
        {  
            LPC_GPIO2->FIOPIN = (1<<LedPinNumber);
        }
        else
        {
            LPC_GPIO2->FIOPIN = (0<<LedPinNumber);
        }      
    }
}
