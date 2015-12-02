/*-----------------------------------------------------------------------------
                 Program to demonstrate Led_Blinging
  -----------------------------------------------------------------------------
 
note: Leds can be connected to any of the ports as all the ports are blinked

------------------------------------------------------------------------------*/
#define EXAMPLE 1



#if (EXAMPLE==1)		    //  BLink All Leds connected to Port2

#include "lpc17xx.h"   
#include "delay.h"     //User defined library which conatins the delay routines
#include "stdutils.h"			  
/* start the main program */
void main() 
{
	SystemInit();	                //Clock and PLL configuration 
	LPC_PINCON->PINSEL2 = 0x000000; //Configure the Pins for GPIO;
    LPC_GPIO2->FIODIR = 0xffffffff; //Configure the PORT pins as OUTPUT;

  while(1)
    {

	 /* Turn On all the leds and wait for one second */ 
       LPC_GPIO2->FIOSET = 0xffffffff;	 // Make all the Port pins as high  
	   DELAY_sec(1);


       LPC_GPIO2->FIOCLR = 0xffffffff;	 // Make all the Port pins as low  
	   DELAY_sec(1);
	}
}
#endif








#if (EXAMPLE==2)			 // BLink All Leds connected to Port2

#include <lpc17xx.h>   
#include "delay.h"     //User defined library which conatins the delay routines



/* start the main program */
void main() 
{
	SystemInit();	                //Clock and PLL configuration 
	LPC_PINCON->PINSEL2 = 0x000000; //Configure the Pins for GPIO;
    LPC_GPIO2->FIODIR = 0xffffffff; //Configure the PORT pins as OUTPUT;

  while(1)
    {

	 /* Turn On all the leds and wait for one second */ 
       LPC_GPIO2->FIOPIN = 0xffffffff;	 // Make all the Port pins as high  
	   DELAY_sec(1);


       LPC_GPIO2->FIOPIN = 0x00000000;	 // Make all the Port pins as low  
	   DELAY_sec(1);
	}
}	

#endif









#if (EXAMPLE==3)				  // BLink Particular Led

#include <lpc17xx.h>   
#include "delay.h"     //User defined library which conatins the delay routines
#include "gpio.h"

#define LED P2_0     // Led is connected to P2.0

/* start the main program */
void main() 
{
    SystemInit();                    //Clock and PLL configuration
	GPIO_PinFunction(LED,PINSEL_FUNC_0); // Configure Pin for Gpio
    GPIO_PinDirection(LED,OUTPUT);	 // C0nfigure the pin as OUTPUT

  while(1)
    {

     /* Turn On all the leds and wait for one second */ 
       GPIO_PinWrite(LED,HIGH);     // Make all the Port pin as high  
       DELAY_sec(1);


       GPIO_PinWrite(LED,LOW);     // Make all the Port pin as low  
       DELAY_sec(1);
    }
}	

#endif
