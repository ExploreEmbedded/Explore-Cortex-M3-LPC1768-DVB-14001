#include <lpc17xx.h>
#include "stdutils.h"
#include "systick.h"
#include "gpio.h"


void myIsr(void)
{
  GPIO_PinToggle(P2_0);              /* Toggle the P2_0 to measure the time */
}
int main()
{
    SystemInit();
    GPIO_PinDirection(P2_0,OUTPUT);  /* Configure the Led Pin as Output */
    SysTick_Init();                  /* Initialize SysTick for 1ms(default)*/
    SysTick_AttachInterrupt(myIsr);  /* myIsr will be called by SysTick_Handler every ms */
    SysTick_Start();                 /* Start SysTick Timer */

   
   while(1)
   {
         /* Do Nothing */
   }
}
