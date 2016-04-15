#include <lpc17xx.h>   
#include "delay.h"     
#include "gpio.h"

#define MY_LED    P2_0     /* Led is connected to P2.0     */
#define MY_SWITCH P2_10    /* Switch is connected to P2.10 */


int main() 
{
    uint8_t value;
    SystemInit();                               /* Clock and PLL configuration */

    GPIO_PinFunction(MY_SWITCH,PINSEL_FUNC_0);  /* Configure Pin for Gpio */
    GPIO_PinDirection(MY_SWITCH,INPUT);         /* Configure the switch pin as Input */

    GPIO_PinFunction(MY_LED,PINSEL_FUNC_0);     /* Configure Pin for Gpio */
    GPIO_PinDirection(MY_LED,OUTPUT);           /* Configure the Led pin as OUTPUT */

    while(1)
    {
        value = GPIO_PinRead(MY_SWITCH);         /* Read the switch status */
        GPIO_PinWrite(MY_LED,value);             /*  ON/OFF the led as per switch status */  
    }
}
