#include "LPC17xx.h"


#define SBIT_TIMER0  1
#define SBIT_TIMER1  2

#define SBIT_MR0I    0
#define SBIT_MR0R    1

#define SBIT_CNTEN   0

#define PCLK_TIMER0  2
#define PCLK_TIMER1  4    

#define LED1         0 // P2_0
#define LED2         1 // P2_1

#define MiliToMicroSec(x)  (x*1000)  /* ms is multiplied by 1000 to get us*/

extern unsigned int SystemCoreClock;
unsigned int getPrescalarForUs(uint8_t timerPclkBit);


int main (void) 
{
    SystemInit();

    LPC_SC->PCONP |= (1<<SBIT_TIMER0) | (1<<SBIT_TIMER1); /* Power ON Timer0,1 */

    LPC_TIM0->MCR  = (1<<SBIT_MR0I) | (1<<SBIT_MR0R);     /* Clear TC on MR0 match and Generate Interrupt*/
    LPC_TIM0->PR   = getPrescalarForUs(PCLK_TIMER0);      /* Prescalar for 1us */
    LPC_TIM0->MR0  = MiliToMicroSec(100);                 /* Load timer value to generate 100ms delay*/
    LPC_TIM0->TCR  = (1 <<SBIT_CNTEN);                    /* Start timer by setting the Counter Enable*/
    NVIC_EnableIRQ(TIMER0_IRQn);                          /* Enable Timer0 Interrupt */

    LPC_TIM1->MCR  = (1<<SBIT_MR0I) | (1<<SBIT_MR0R);     /* Clear TC on MR0 match and Generate Interrupt*/
    LPC_TIM1->PR   = getPrescalarForUs(PCLK_TIMER1);      /* Prescalar for 1us */
    LPC_TIM1->MR0  = MiliToMicroSec(500);                 /* Load timer value to generate 500ms delay*/
    LPC_TIM1->TCR  = (1 <<SBIT_CNTEN);                    /* Start timer by setting the Counter Enable*/
    NVIC_EnableIRQ(TIMER1_IRQn);                          /* Enable Timer0 Interrupt */

    LPC_GPIO2->FIODIR = (1<<LED1) | (1<<LED2);            /* LEDs are connected P2.0, P2_1 */
  
    while(1)
    {
        //do nothing
    }
}

void TIMER0_IRQHandler(void)
{
    unsigned int isrMask;

    isrMask = LPC_TIM0->IR; 
    LPC_TIM0->IR = isrMask;         /* Clear the Interrupt Bit */

    LPC_GPIO2->FIOPIN ^= (1<<LED1); /* Toggle the LED1 (P2_0) */
}


void TIMER1_IRQHandler(void)
{
    unsigned int isrMask;

    isrMask = LPC_TIM1->IR;
    LPC_TIM1->IR = isrMask;        /* Clear the Interrupt Bit */

    LPC_GPIO2->FIOPIN ^= (1<<LED2); /* Toggle the LED2 (P2_1) */
}


unsigned int getPrescalarForUs(uint8_t timerPclkBit)
{
    unsigned int pclk,prescalarForUs;
    pclk = (LPC_SC->PCLKSEL0 >> timerPclkBit) & 0x03;  /* get the pclk info for required timer */

    switch ( pclk )                                    /* Decode the bits to determine the pclk*/
    {
    case 0x00:
        pclk = SystemCoreClock/4;
        break;

    case 0x01:
        pclk = SystemCoreClock;
        break; 

    case 0x02:
        pclk = SystemCoreClock/2;
        break; 

    case 0x03:
        pclk = SystemCoreClock/8;
        break;

    default:
        pclk = SystemCoreClock/4;
        break;  
    }

    prescalarForUs =pclk/1000000;                      /* Prescalar for 1us (1000000Counts/sec) */

    return prescalarForUs;
}
