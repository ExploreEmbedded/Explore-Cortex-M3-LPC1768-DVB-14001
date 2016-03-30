#include <LPC17xx.h>
#include "rtc.h"
#include "lcd.h"    

int main() 
{
    rtc_t rtc;

    SystemInit();

    /*Connect  RS   RW   EN   D0   D1   D2   D3   D4    D5    D6    D7 (P_NC: Not Connected)*/
    LCD_SetUp(P2_0,P2_1,P2_2,P_NC,P_NC,P_NC,P_NC,P1_24,P1_25,P1_26,P1_27);
    LCD_Init(2,16);

    RTC_Init();

    rtc.hour = 10; //  10:40:20 am
    rtc.min =  40;
    rtc.sec =  0;

    rtc.date = 1; //1st Jan 2016
    rtc.month = 1;
    rtc.year = 2016;
    rtc.weekDay = 5; // Friday: 5th day of week considering monday as first day.


    /*##### Set the time and Date only once. Once the Time and Date is set, comment these lines
         and reflash the code. Else the time will be set every time the controller is reset*/
    RTC_SetDateTime(&rtc);  //  10:40:20 am, 1st Jan 2016


    /* Display the Time and Date continuously */
    while(1)
    {
        RTC_GetDateTime(&rtc);
        LCD_GoToLine(0);
        LCD_Printf("time:%2d:%2d:%2d  \nDate:%2d/%2d/%4d",(uint16_t)rtc.hour,(uint16_t)rtc.min,(uint16_t)rtc.sec,(uint16_t)rtc.date,(uint16_t)rtc.month,(uint16_t)rtc.year);
    }            
}
