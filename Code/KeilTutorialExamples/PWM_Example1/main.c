#include <lpc17xx.h>

void delay_ms(unsigned int ms)
{
    unsigned int i,j;
    for(i=0;i<ms;i++)
        for(j=0;j<50000;j++);
} 


#define SBIT_CNTEN     0 
#define SBIT_PWMEN     2

#define SBIT_PWMMR0R   1

#define SBIT_LEN0      0
#define SBIT_LEN1      1
#define SBIT_LEN2      2
#define SBIT_LEN3      3
#define SBIT_LEN4      4

#define SBIT_PWMENA1   9
#define SBIT_PWMENA2   10
#define SBIT_PWMENA3   11
#define SBIT_PWMENA4   12


#define PWM_1          0 //P2_0 (0-1 Bits of PINSEL4)
#define PWM_2          2 //P2_1 (2-3 Bits of PINSEL4)
#define PWM_3          4 //P2_2 (4-5 Bits of PINSEL4)
#define PWM_4          6 //P2_3 (6-7 Bits of PINSEL4)



int main(void)
{
    int dutyCycle;
    SystemInit();
    /* Cofigure pins(P2_0 - P2_3) for PWM mode. */
    LPC_PINCON->PINSEL4 = (1<<PWM_1) | (1<<PWM_2) | (1<<PWM_3) | (1<<PWM_4); 

    /* Enable Counters,PWM module */ 
    LPC_PWM1->TCR = (1<<SBIT_CNTEN) | (1<<SBIT_PWMEN);

    LPC_PWM1->PR  =  0x0;               /* No Prescalar */
    LPC_PWM1->MCR = (1<<SBIT_PWMMR0R);  /*Reset on PWMMR0, reset TC if it matches MR0 */

    LPC_PWM1->MR0 = 100;                /* set PWM cycle(Ton+Toff)=100) */
    LPC_PWM1->MR1 = 50;                 /* Set 50% Duty Cycle for all four channels */
    LPC_PWM1->MR2 = 50;
    LPC_PWM1->MR3 = 50;
    LPC_PWM1->MR4 = 50;
    /* Trigger the latch Enable Bits to load the new Match Values */
    LPC_PWM1->LER = (1<<SBIT_LEN0) | (1<<SBIT_LEN1) | (1<<SBIT_LEN2) | (1<<SBIT_LEN3) | (1<<SBIT_LEN4); 

    /* Enable the PWM output pins for PWM_1-PWM_4(P2_0 - P2_3) */
    LPC_PWM1->PCR = (1<<SBIT_PWMENA1) | (1<<SBIT_PWMENA2) | (1<<SBIT_PWMENA3) | (1<<SBIT_PWMENA4);

    while(1)
    {
        for(dutyCycle=0; dutyCycle<100; dutyCycle++)
        {
            LPC_PWM1->MR1 = dutyCycle;  /* Increase the dutyCycle from 0-100 */
            LPC_PWM1->MR2 = dutyCycle;
            LPC_PWM1->MR3 = dutyCycle;
            LPC_PWM1->MR4 = dutyCycle;
            /* Trigger the latch Enable Bits to load the new Match Values */
            LPC_PWM1->LER = (1<<SBIT_LEN0) | (1<<SBIT_LEN1) | (1<<SBIT_LEN2) | (1<<SBIT_LEN3) | (1<<SBIT_LEN4); 
            delay_ms(5);
        }

        for(dutyCycle=100; dutyCycle>0; dutyCycle--)
        {
            LPC_PWM1->MR1 = dutyCycle;  /* Decrease the dutyCycle from 100-0 */
            LPC_PWM1->MR2 = dutyCycle;
            LPC_PWM1->MR3 = dutyCycle;
            LPC_PWM1->MR4 = dutyCycle;
            /* Trigger the latch Enable Bits to load the new Match Values */
            LPC_PWM1->LER = (1<<SBIT_LEN0) | (1<<SBIT_LEN1) | (1<<SBIT_LEN2) | (1<<SBIT_LEN3) | (1<<SBIT_LEN4); 
            delay_ms(5);
        }
    }
} 
