#include <lpc17xx.h>
#include "pwm.h"
#include "delay.h"

#define CYCLE_TIME 255

/* start the main program */
int main() 
{
    int dutyCycle;
    SystemInit();             /* Clock and PLL configuration */ 
    PWM_Init(CYCLE_TIME);     /* Initialize the PWM module and the Cycle time(Ton+Toff) is set to 255(similar to arduino)*/
    PWM_Start(PWM_1|PWM_2|PWM_3|PWM_4); /* Enable PWM output on PWM_1-PWM_4 (P2_0 - P2_3) */

    while(1)
    {

        for(dutyCycle=0;dutyCycle<CYCLE_TIME;dutyCycle++) /* Increase the Brightness of the Leds */
        {
            PWM_SetDutyCycle(PWM_1,dutyCycle);  //P2_0
            PWM_SetDutyCycle(PWM_2,dutyCycle);  //P2_1
            PWM_SetDutyCycle(PWM_3,dutyCycle);  //P2_2
            PWM_SetDutyCycle(PWM_4,dutyCycle);  //P2_3
            DELAY_ms(5);
        }

        for(dutyCycle=CYCLE_TIME;dutyCycle>0;dutyCycle--) /* Decrease the Brightness of the Leds */
        {
            PWM_SetDutyCycle(PWM_1,dutyCycle);  //P2_0
            PWM_SetDutyCycle(PWM_2,dutyCycle);  //P2_1
            PWM_SetDutyCycle(PWM_3,dutyCycle);  //P2_2
            PWM_SetDutyCycle(PWM_4,dutyCycle);  //P2_3
            DELAY_ms(5);
        }
    }                              
}
