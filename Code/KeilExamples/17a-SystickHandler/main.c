#include <lpc17xx.h>
#include "stdutils.h"
#include "systick.h"
#include "uart.h"

uint32_t myCount=0;

void myIsr(void)
{
  myCount++;
}
int main()
{
    SystemInit();
    SysTick_Init();
    UART0_Init(9600);
    SysTick_AttachInterrupt(myIsr);
    SysTick_Start();

   
   while(1)
   {
    // UART0_Printf("\n\rTimer Count = %8U",SysTick_GetMsTime());  
     UART0_Printf("\n\rTimer Count = %8U  Local COunt=%8U",SysTick_GetMsTime(),myCount);           
   }
}
