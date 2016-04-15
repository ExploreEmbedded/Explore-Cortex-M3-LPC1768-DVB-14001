#include <lpc17xx.h>   

#define PINSEL_EINT0    20
#define PINSEL_EINT1    22

#define LED1            0   
#define LED2            1    

#define SBIT_EINT0      0
#define SBIT_EINT1      1  

#define SBIT_EXTMODE0   0
#define SBIT_EXTMODE1   1

#define SBIT_EXTPOLAR0  0
#define SBIT_EXTPOLAR1  1


void EINT0_IRQHandler(void)
{
    LPC_SC->EXTINT = (1<<SBIT_EINT0);  /* Clear Interrupt Flag */
    LPC_GPIO2->FIOPIN ^= (1<< LED1);   /* Toggle the LED1 everytime INTR0 is generated */
}


void EINT1_IRQHandler(void)
{
    LPC_SC->EXTINT = (1<<SBIT_EINT1);  /* Clear Interrupt Flag */
    LPC_GPIO2->FIOPIN ^= (1<< LED2);   /* Toggle the LED2 everytime INTR1 is generated */
}
      


int main()
{
    SystemInit();

    LPC_SC->EXTINT      = (1<<SBIT_EINT0)    | (1<<SBIT_EINT1);	    /* Clear Pending interrupts */
    LPC_PINCON->PINSEL4 = (1<<PINSEL_EINT0)  | (1<<PINSEL_EINT1);   /* Configure P2_10,P2_11 as EINT0/1 */
    LPC_SC->EXTMODE     = (1<<SBIT_EXTMODE0) | (1<<SBIT_EXTMODE1);  /* Configure EINTx as Edge Triggered*/
    LPC_SC->EXTPOLAR    = (1<<SBIT_EXTPOLAR0)| (1<<SBIT_EXTPOLAR0); /* Configure EINTx as Falling Edge */

    LPC_GPIO2->FIODIR   = (1<<LED1)  | (1<<LED2);                   /* Configure LED pins as OUTPUT */
    LPC_GPIO2->FIOPIN   =  0x00;

    NVIC_EnableIRQ(EINT0_IRQn);    /* Enable the EINT0,EINT1 interrupts */
    NVIC_EnableIRQ(EINT1_IRQn);  

    while(1)
    {
      // Do nothing
    }      
}
