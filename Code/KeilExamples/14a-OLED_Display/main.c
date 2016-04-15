#include "OLED_I2C.h"
#include "delay.h"
#include "gpio.h"

 

extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];

int y = 50,i;
void SystemInit(void);
void loop(void);
int main()
{

	SystemInit();
	OLED_Init(P0_0, P0_1);
	
  OLED_SetFont(SmallFont);

	
		while(1)
		{
			OLED_SetFont(MediumNumbers);
			for(i=0;i<1000;i++)
			{
			
			   OLED_PrintFloatNumber(i/3.0,3,50,0);
				OLED_Update();
				delay_ms(1000);
			}
		}
}




