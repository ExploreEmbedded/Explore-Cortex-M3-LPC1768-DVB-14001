/*-----------------------------------------------------------------------------
                 Program to demonstrate Led_Blinging
  -----------------------------------------------------------------------------
 
note: Leds can be connected to any of the ports as all the ports are blinked

------------------------------------------------------------------------------*/
#define EXAMPLE 4

#if (EXAMPLE==1)
/*In this program we are going to do both INPUT and OUTPUT operation. 
The port pin to which switch is connected is configured as Input and 
the pin to which LED is connected is configured as OUTPUT. 
Here the switch status is read and accordingly the LED will be turned ON/OFF.*/

#include <lpc17xx.h>   

#define SwitchPinNumber 12
#define LedPinNumber    0

/* start the main program */
void main() 
{
    uint32_t switchStatus;
	SystemInit();	                //Clock and PLL configuration 
	LPC_PINCON->PINSEL2 = 0x000000; //Configure the Pins for GPIO;
    LPC_GPIO2->FIODIR = ((1<<LedPinNumber) | (0<<SwitchPinNumber)); //Configure the PORT pins as OUTPUT;

  while(1)
    {

	 /* Turn On all the leds and wait for one second */ 
       switchStatus = (LPC_GPIO2->FIOPIN>>SwitchPinNumber) & 0x01 ;	 // Make all the Port pins as high
	   
	   if(switchStatus == 1)
	   {  
	     LPC_GPIO2->FIOPIN = (1<<LedPinNumber);
	   }
	   else
	   {
	   	 LPC_GPIO2->FIOPIN = (0<<LedPinNumber);
	   }	  
	}
}
#endif







#if (EXAMPLE==2) 
/*This is the alternate method using the stdutils macros.*/
#include <lpc17xx.h>   
#include "delay.h"     //User defined library which conatins the delay routines
#include "stdutils.h"

#define SwitchPinNumber 12
#define LedPinNumber    0

/* start the main program */
void main() 
{
    uint32_t switchStatus;
	SystemInit();	                //Clock and PLL configuration 
	LPC_PINCON->PINSEL2 = 0x000000; //Configure the Pins for GPIO;
    LPC_GPIO2->FIODIR = ((1<<LedPinNumber) | (0<<SwitchPinNumber)); //Configure the PORT pins as OUTPUT;

  while(1)
    {

	 /* Turn On all the leds and wait for one second */ 
       switchStatus = util_GetBitStatus(LPC_GPIO2->FIOPIN,SwitchPinNumber);	 // Make all the Port pins as high
	   
	   if(switchStatus == 1)
	   {  
	     util_BitSet(LPC_GPIO2->FIOPIN,LedPinNumber);
	   }
	   else
	   {
	   	 util_BitClear(LPC_GPIO2->FIOPIN,LedPinNumber);
	   }	  
	}
} 
#endif






#if (EXAMPLE==3)
/*In this program multiple(3) switches are read and multiple LEDs are turned ON/OFF 
depending on the respective switch status. As shown in the schematic the LEDs are connected from P2.0 to P2.3. 
And the switches are connected to P2.10 to P2.12, accordingly the 3bit mask will be 0x07. */
#include <lpc17xx.h>   
 
#define SwitchPinNumber 10
#define LedPinNumber    0
 
#define ThreeBitMask   0x07

 
 
/* start the main program */
void main() 
{
    uint32_t switchStatus;
    SystemInit();                    //Clock and PLL configuration 
    LPC_PINCON->PINSEL2 = 0x000000; //Configure the Pins for GPIO;
    /* Configure all the LED pins as output and SwitchPins as input */
    LPC_GPIO2->FIODIR = ((ThreeBitMask<<LedPinNumber) | (0<<SwitchPinNumber)); 
 
  while(1)
    {
 
     /* Read the switch status*/ 
       switchStatus = (LPC_GPIO2->FIOPIN>>SwitchPinNumber) & ThreeBitMask ;                           
       LPC_GPIO2->FIOPIN = (switchStatus<<LedPinNumber);  //Turn ON/OFF LEDs depending on switch status
    }
}

#endif




#if (EXAMPLE == 4)
#include <lpc17xx.h>   
#include "delay.h"     //User defined library which conatins the delay routines
#include "gpio.h"

#define MY_LED    P2_0     // Led is connected to P2.0
#define MY_SWITCH P2_12    // Switch is connected to P2.12

/* start the main program */
void main() 
{
    uint8_t value;
    SystemInit();                    //Clock and PLL configuration

    GPIO_PinFunction(MY_SWITCH,PINSEL_FUNC_0); // Configure Pin for Gpio
    GPIO_PinDirection(MY_SWITCH,INPUT);          // Configure the switch pin as Input

    GPIO_PinFunction(MY_LED,PINSEL_FUNC_0); // Configure Pin for Gpio
    GPIO_PinDirection(MY_LED,OUTPUT);        // Configure the Led pin as OUTPUT

  while(1)
    {

       value = GPIO_PinRead(MY_SWITCH);  // Read the switch status
       GPIO_PinWrite(MY_LED,value);     // ON/OFF the let as per switch status  
    }
}	

#endif
