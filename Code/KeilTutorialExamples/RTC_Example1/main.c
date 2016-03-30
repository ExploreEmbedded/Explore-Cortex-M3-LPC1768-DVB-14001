#include <lpc17xx.h>	//Device specific header file 
#include "lcd.h"	    // Explore Embedded LCD library

/* bit position of CCR register */
#define SBIT_CLKEN	   0	/* RTC Clock Enable*/
#define SBIT_CTCRST	   1	/* RTC Clock Reset */
#define SBIT_CCALEN	   4	/* RTC Calibration counter enable */


int main()
{
	uint16_t year;
	uint8_t hour, min, sec, date, month;
	
	SystemInit();			//Clock and PLL configuration
	
	/* Setup/Map the controller pins for LCD operation 
	           RS   RW   EN   D0   D1   D2   D3   D4    D5    D6    D7  (P_NC: Not Connected)*/
    LCD_SetUp(P2_0,P2_1,P2_2,P_NC,P_NC,P_NC,P_NC,P1_24,P1_25,P1_26,P1_27);
	
	/* Specify the LCD type(2x16) for initialization*/
	LCD_Init(2,16);    
	
	/* Disable RTC clock, reset clock, Enable RTC calibration */
	LPC_RTC->CCR = ((1 << SBIT_CTCRST ) | (1 << SBIT_CCALEN));
	LPC_RTC->CALIBRATION = 0x00;
	LPC_RTC->CCR = (1 << SBIT_CLKEN);    /* Enable the clock for RTC */
	
	/* Set Date and Time only once, comment these lines after setting the time and date */                                           
    // Set Date 14th Nov 2015 
	LPC_RTC->DOM	= 14;	// Update date value 
	LPC_RTC->MONTH  = 11;	// Update month value
	LPC_RTC->YEAR   = 2015;	// Update year value
	
	// Set Time 10:40:25 AM 
	LPC_RTC->HOUR	= 10;	// Update hour value 
	LPC_RTC->MIN    = 40;	// Update min value
	LPC_RTC->SEC    = 25;	// Update sec value	
	
	
	while (1)
	{
		/* Read Time */
		hour = LPC_RTC->HOUR;
		min  = LPC_RTC->MIN; 
		sec  = LPC_RTC->SEC; 
		
		/* Read Date */
		date  = LPC_RTC->DOM;	
		month = LPC_RTC->MONTH;  
		year  = LPC_RTC->YEAR;   
			
		/* Display date and time on LCD */
		LCD_GoToLine(0);			/* Go to First line of 2x16 LCD */
		LCD_Printf("Time: %2d:%2d:%2d",hour,min,sec);
		LCD_GoToLine(1);			/* Go to Second line of 2x16 LCD */
		LCD_Printf("Date: %2d/%2d/%4u",date,month,year);   
	}	
}
