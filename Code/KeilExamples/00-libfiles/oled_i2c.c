

/*
  OLED_I2C.cpp - Arduino/chipKit library support for 128x64 pixel SSD1306 OLEDs
  Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved

  This library has been made to make it easy to use 128x64 pixel OLED displays
  based on the SSD1306 controller chip with an Arduino or a chipKit.

  You can always find the latest version of the library at 
  http://www.RinkyDinkElectronics.com/

  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.

  Commercial use of this library requires you to buy a license that
  will allow commercial use. This includes using the library,
  modified or not, as a tool to sell products.

  The license applies to all part of the library including the 
  examples and tools supplied with the library.
 */
#include "stdutils.h"
#include "OLED_I2C.h"
#include "delay.h"
#include "gpio.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

void oled_sendStartCondition(uint8_t addr);
void oled_sendStopCondition(void);
void oled_sendCommand(uint8_t value);
void oled_sendAck(void);
void oled_sendNack(void);
void oled_waitForAck(void);
void oled_writeByte(uint8_t value);
void oled_shiftOut(uint8_t value);


uint8_t         SCL_PIN, SDA_PIN;
_current_font   cfont;
uint8_t         scrbuf[1024];




/**************************************************************************************************
Functions to Innitilaize, Update, Clear, Fill, Invert and Control Brightness of the OLED display 
**************************************************************************************************/
void OLED_Init(uint8_t data_pin, uint8_t sclk_pin)
{ 
    SDA_PIN = data_pin;
    SCL_PIN = sclk_pin;

    
    GPIO_PinDirection(SCL_PIN, OUTPUT);


    oled_sendCommand(SSD1306_DISPLAY_OFF);
    oled_sendCommand(SSD1306_SET_DISPLAY_CLOCK_DIV_RATIO);
    oled_sendCommand(0x80);
    oled_sendCommand(SSD1306_SET_MULTIPLEX_RATIO);
    oled_sendCommand(0x3F);
    oled_sendCommand(SSD1306_SET_DISPLAY_OFFSET);
    oled_sendCommand(0x0);
    oled_sendCommand(SSD1306_SET_START_LINE | 0x0);
    oled_sendCommand(SSD1306_CHARGE_PUMP);
    oled_sendCommand(0x14);
    oled_sendCommand(SSD1306_MEMORY_ADDR_MODE);
    oled_sendCommand(0x00);
    oled_sendCommand(SSD1306_SET_SEGMENT_REMAP | 0x1);
    oled_sendCommand(SSD1306_COM_SCAN_DIR_DEC);
    oled_sendCommand(SSD1306_SET_COM_PINS);
    oled_sendCommand(0x12);
    oled_sendCommand(SSD1306_SET_CONTRAST_CONTROL);
    oled_sendCommand(0xCF);
    oled_sendCommand(SSD1306_SET_PRECHARGE_PERIOD);
    oled_sendCommand(0xF1);
    oled_sendCommand(SSD1306_SET_VCOM_DESELECT);
    oled_sendCommand(0x40);
    oled_sendCommand(SSD1306_DISPLAY_ALL_ON_RESUME);
    oled_sendCommand(SSD1306_NORMAL_DISPLAY);
    oled_sendCommand(SSD1306_DISPLAY_ON);

    OLED_ClearScreen();
    OLED_Update();
    cfont.font=0;
}




void OLED_Update()
{
int b;
    oled_sendCommand(SSD1306_SET_COLUMN_ADDR);
    oled_sendCommand(0);
    oled_sendCommand(127);

    oled_sendCommand(SSD1306_SET_PAGE_ADDR);
    oled_sendCommand(0);
    oled_sendCommand(7);

    oled_sendStartCondition(SSD1306_ADDR<<1);
    oled_waitForAck();
    oled_writeByte(SSD1306_DATA_CONTINUE);
    oled_waitForAck();  

    for (b=0; b<1024; b++)      // Send data
    {  
        oled_writeByte(scrbuf[b]);
        oled_waitForAck();
    }

    oled_sendStopCondition();    
}




void OLED_ClearScreen()
{
    memset(scrbuf, 0, 1024);
}

void OLED_FillScreen()
{
    memset(scrbuf, 0xff, 1024);
}



void OLED_SetBrightness(uint8_t value)
{
    oled_sendCommand(SSD1306_SET_CONTRAST_CONTROL);
    oled_sendCommand(value);
}




void OLED_InvertDisplay(uint8_t mode)
{
    if (mode==TRUE)
        oled_sendCommand(SSD1306_INVERT_DISPLAY);
    else
        oled_sendCommand(SSD1306_NORMAL_DISPLAY);
}




void OLED_InvertDisplayText(uint8_t mode)
{
    if (mode==TRUE)
        cfont.OLED_InvertDisplayed=1;
    else
        cfont.OLED_InvertDisplayed=0;
}




void OLED_InvertPixel(uint16_t x, uint16_t y)
{
    int by, bi;

    if ((x<128) && (y<64))
    {
        by=((y/8)*128)+x;
        bi=y % 8;

        if ((scrbuf[by] & (1<<bi))==0)
            scrbuf[by]=scrbuf[by] | (1<<bi);
        else
            scrbuf[by]=scrbuf[by] & ~(1<<bi);
    }
}




void OLED_SetPixel(uint16_t x, uint16_t y)
{
    int by, bi;

    if ((x<128) && (y<64))
    {
        by=((y/8)*128)+x;
        bi=y % 8;

        scrbuf[by]=scrbuf[by] | (1<<bi);
    }
}



void OLED_ClearPixel(uint16_t x, uint16_t y)
{
    int by, bi;

    if ((x<128) && (y<64))
    {
        by=((y/8)*128)+x;
        bi=y % 8;

        scrbuf[by]=scrbuf[by] & ~(1<<bi);
    }
}
/*************************************************************************************************/








/**************************************************************************************************
               Functions to select the Font, Display chars, strings and numbers
**************************************************************************************************/
void OLED_SetFont(uint8_t* font)
{
    cfont.font=font;
    cfont.x_size=fontbyte(0);
    cfont.y_size=fontbyte(1);
    cfont.offset=fontbyte(2);
    cfont.numchars=fontbyte(3);
    cfont.OLED_InvertDisplayed=0;
}





void OLED_PrintChar(unsigned char c, int x, int y)
{

int rowcnt,cnt,font_idx,b,cbit,cbyte,cx,cy;

 if ((cfont.y_size % 8) == 0)
    {
         font_idx = ((c - cfont.offset)*(cfont.x_size*(cfont.y_size/8)))+4;
        for ( rowcnt=0; rowcnt<(cfont.y_size/8); rowcnt++)
        {
            for( cnt=0; cnt<cfont.x_size; cnt++)
            {
                for ( b=0; b<8; b++)
                    if ((fontbyte(font_idx+cnt+(rowcnt*cfont.x_size)) & (1<<b))!=0)
                        if (cfont.OLED_InvertDisplayed==0)
                            OLED_SetPixel(x+cnt, y+(rowcnt*8)+b);
                        else
                            OLED_ClearPixel(x+cnt, y+(rowcnt*8)+b);
                    else
                        if (cfont.OLED_InvertDisplayed==0)
                            OLED_ClearPixel(x+cnt, y+(rowcnt*8)+b);
                        else
                            OLED_SetPixel(x+cnt, y+(rowcnt*8)+b);
            }
        }
    }
    else
    {
         font_idx = ((c - cfont.offset)*((cfont.x_size*cfont.y_size/8)))+4;
         cbyte=fontbyte(font_idx);
         cbit=7;
        for ( cx=0; cx<cfont.x_size; cx++)
        {
            for ( cy=0; cy<cfont.y_size; cy++)
            {
                if ((cbyte & (1<<cbit)) != 0)
                    if (cfont.OLED_InvertDisplayed==0)
                        OLED_SetPixel(x+cx, y+cy);
                    else
                        OLED_ClearPixel(x+cx, y+cy);
                else
                    if (cfont.OLED_InvertDisplayed==0)
                        OLED_ClearPixel(x+cx, y+cy);
                    else
                        OLED_SetPixel(x+cx, y+cy);
                cbit--;
                if (cbit<0)
                {
                    cbit=7;
                    font_idx++;
                    cbyte=fontbyte(font_idx);
                }
            }
        }
    }
}





void OLED_Puts(char *st, int x, int y)
{

    int stl,cnt;

    stl = strlen(st);
    if (x == RIGHT)
        x = 128-(stl*cfont.x_size);
    if (x == CENTER)
        x = (128-(stl*cfont.x_size))/2;

    for (cnt=0; cnt<stl; cnt++)
        OLED_PrintChar(*st++, x + (cnt*(cfont.x_size)), y);
}




void OLED_PrintBinaryNumber(long num, int x, int y, int length)
{
    int i;
    char st[33];
    
    for(i=0;i<length;i++)
    {
         if(num)
         {
        st[i] = (num & 0x01)  + 0x30;
        num = num>>1;
         }
         else
         {
            st[i] = ' '; 
         }
    }
    
    st[length] = 0;

    OLED_Puts(st,x,y);
}





void OLED_PrintDecimalNumber(long num, int x, int y, int length)
{
    int i;
    char buf[25];
    char st[27];
    uint8_t neg=FALSE;
    int c=0, f=0;

    if (num==0)
    {
        if (length!=0)
        {
            for (c=0; c<(length-1); c++)
                st[c]=' ';
            st[c]=48;
            st[c+1]=0;
        }
        else
        {
            st[0]=48;
            st[1]=0;
        }
    }
    else
    {
        if (num<0)
        {
            neg=TRUE;
            num=-num;
        }

        while (num>0)
        {
            buf[c]=48+(num % 10);
            c++;
            num=(num-(num % 10))/10;
        }
        buf[c]=0;

        if (neg)
        {
            st[0]=45;
        }

        if (length>(c+neg))
        {
            for ( i=0; i<(length-c-neg); i++)
            {
                st[i+neg]=' ';
                f++;
            }
        }

        for ( i=0; i<c; i++)
        {
            st[i+neg+f]=buf[c-i-1];
        }
        st[c+neg+f]=0;

    }

    OLED_Puts(st,x,y);
}





void OLED_PrintHexNumber(long num, int x, int y, int length)
{
    char st[20];
    st[length+1] = 0;
    while((num) && (length))
    {
       st[length--] = util_Dec2Ascii(num & 0x0f);
       num = num>>4;      
    }
    
    while(length)
    {
        st[length--] = ' '; 
    }
    
    OLED_Puts(st+1,x,y);
}





void OLED_PrintFloatNumber(double num, uint8_t dec, int x, int y)
{
    int powerten=1;
    char st[50]="0.17";
    char buffer[10];
    //uint8_t neg=FALSE; TODO: Handle negative numbers
    uint32_t int_part,decimal_part;
    int i=0,len;
    double rounding = 0.5;

    if (num<0)
    {
        //  neg = TRUE;
        num =-num;
    }  



    for (i=0; i<dec; ++i)
    {
        rounding /= 10.0;
        powerten *=10;
    }

    num +=rounding; 
    int_part = (uint32_t)num;

    decimal_part = (num - int_part)*powerten;;
    sprintf(st,"%d",int_part);
    sprintf(buffer,"%d",decimal_part);
    len= strlen(st);
    st[len++] = '.';

    for(i=0;buffer[i];i++)
    {
        st[len++] = buffer[i];
    }

    while(i<dec)
    {
        i++;
        st[len++] = 0x30;
    }

    st[len]=0; 
    OLED_Puts(st,x,y);
}
/*************************************************************************************************/














/**************************************************************************************************
                                     Graphics related functions
***************************************************************************************************/
void OLED_DrawHorzLine(int x, int y, int l)
{
    int by, bi,cx;

    if ((x<128) && (y<64))
    {
        for ( cx=0; cx<l; cx++)
        {
            by=((y/8)*128)+x;
            bi=y % 8;

            scrbuf[by+cx] |= (1<<bi);
        }
    }
}

void OLED_ClearHorzLine(int x, int y, int l)
{
    int by, bi,cx;

    if ((x<128) && (y<64))
    {
        for ( cx=0; cx<l; cx++)
        {
            by=((y/8)*128)+x;
            bi=y % 8;

            scrbuf[by+cx] &= ~(1<<bi);
        }
    }
}

void OLED_DrawVertLine(int x, int y, int l)
{
   int cy;

    if ((x<128) && (y<64))
    {
        for ( cy=0; cy<l; cy++)
        {
            OLED_SetPixel(x, y+cy);
        }
    }
}

void OLED_ClearVertLine(int x, int y, int l)
{
   int cy;

    if ((x<128) && (y<64))
    {
        for ( cy=0; cy<l; cy++)
        {
            OLED_ClearPixel(x, y+cy);
        }
    }
}

void OLED_DrawLine(int x1, int y1, int x2, int y2)
{
    int tmp,i;
    double delta, tx, ty;


    if (((x2-x1)<0))
    {
        tmp=x1;
        x1=x2;
        x2=tmp;
        tmp=y1;
        y1=y2;
        y2=tmp;
    }
    if (((y2-y1)<0))
    {
        tmp=x1;
        x1=x2;
        x2=tmp;
        tmp=y1;
        y1=y2;
        y2=tmp;
    }

    if (y1==y2)
    {
        if (x1>x2)
        {
            tmp=x1;
            x1=x2;
            x2=tmp;
        }
        OLED_DrawHorzLine(x1, y1, x2-x1);
    }
    else if (x1==x2)
    {
        if (y1>y2)
        {
            tmp=y1;
            y1=y2;
            y2=tmp;
        }
        OLED_DrawVertLine(x1, y1, y2-y1);
    }
    else if (util_GetAbsolute(x2-x1)>util_GetAbsolute(y2-y1))
    {
        delta=((double)(y2-y1)/(double)(x2-x1));
        ty=(double)y1;
        if (x1>x2)
        {
            for ( i=x1; i>=x2; i--)
            {
                OLED_SetPixel(i, (int)(ty+0.5));
                ty=ty-delta;
            }
        }
        else
        {
            for ( i=x1; i<=x2; i++)
            {
                OLED_SetPixel(i, (int)(ty+0.5));
                ty=ty+delta;
            }
        }
    }
    else
    {
        delta=((float)(x2-x1)/(float)(y2-y1));
        tx=(float)(x1);
        if (y1>y2)
        {
            for ( i=y2+1; i>y1; i--)
            {
                OLED_SetPixel((int)(tx+0.5), i);
                tx=tx+delta;
            }
        }
        else
        {
            for ( i=y1; i<y2+1; i++)
            {
                OLED_SetPixel((int)(tx+0.5), i);
                tx=tx+delta;
            }
        }
    }

}

void OLED_ClearLine(int x1, int y1, int x2, int y2)
{
    int tmp,i;
    double delta, tx, ty;


    if (((x2-x1)<0))
    {
        tmp=x1;
        x1=x2;
        x2=tmp;
        tmp=y1;
        y1=y2;
        y2=tmp;
    }
    if (((y2-y1)<0))
    {
        tmp=x1;
        x1=x2;
        x2=tmp;
        tmp=y1;
        y1=y2;
        y2=tmp;
    }

    if (y1==y2)
    {
        if (x1>x2)
        {
            tmp=x1;
            x1=x2;
            x2=tmp;
        }
        OLED_ClearHorzLine(x1, y1, x2-x1);
    }
    else if (x1==x2)
    {
        if (y1>y2)
        {
            tmp=y1;
            y1=y2;
            y2=tmp;
        }
        OLED_ClearVertLine(x1, y1, y2-y1);
    }
    else if (util_GetAbsolute(x2-x1)>util_GetAbsolute(y2-y1))
    {
        delta=((double)(y2-y1)/(double)(x2-x1));
        ty=(double)(y1);
        if (x1>x2)
        {
            for ( i=x1; i>=x2; i--)
            {
                OLED_ClearPixel(i, (int)(ty+0.5));
                ty=ty-delta;
            }
        }
        else
        {
            for ( i=x1; i<=x2; i++)
            {
                OLED_ClearPixel(i, (int)(ty+0.5));
                ty=ty+delta;
            }
        }
    }
    else
    {
        delta=((float)(x2-x1)/(float)(y2-y1));
        tx=(float)(x1);
        if (y1>y2)
        {
            for ( i=y2+1; i>y1; i--)
            {
                OLED_ClearPixel((int)(tx+0.5), i);
                tx=tx+delta;
            }
        }
        else
        {
            for ( i=y1; i<y2+1; i++)
            {
                OLED_ClearPixel((int)(tx+0.5), i);
                tx=tx+delta;
            }
        }
    }

}

void OLED_DrawRectangle(int x1, int y1, int x2, int y2)
{
    int tmp;

    if (x1>x2)
    {
        tmp=x1;
        x1=x2;
        x2=tmp;
    }
    if (y1>y2)
    {
        tmp=y1;
        y1=y2;
        y2=tmp;
    }

    OLED_DrawHorzLine(x1, y1, x2-x1);
    OLED_DrawHorzLine(x1, y2, x2-x1);
    OLED_DrawVertLine(x1, y1, y2-y1);
    OLED_DrawVertLine(x2, y1, y2-y1+1);
}

void OLED_ClearRectangle(int x1, int y1, int x2, int y2)
{
    int tmp;

    if (x1>x2)
    {
        tmp=x1;
        x1=x2;
        x2=tmp;
    }
    if (y1>y2)
    {
        tmp=y1;
        y1=y2;
        y2=tmp;
    }

    OLED_ClearHorzLine(x1, y1, x2-x1);
    OLED_ClearHorzLine(x1, y2, x2-x1);
    OLED_ClearVertLine(x1, y1, y2-y1);
    OLED_ClearVertLine(x2, y1, y2-y1+1);
}

void OLED_DrawRoundRectangle(int x1, int y1, int x2, int y2)
{
    int tmp;

    if (x1>x2)
    {
        tmp=x1;
        x1=x2;
        x2=tmp;
    }
    if (y1>y2)
    {
        tmp=y1;
        y1=y2;
        y2=tmp;
    }
    if ((x2-x1)>4 && (y2-y1)>4)
    {
        OLED_SetPixel(x1+1,y1+1);
        OLED_SetPixel(x2-1,y1+1);
        OLED_SetPixel(x1+1,y2-1);
        OLED_SetPixel(x2-1,y2-1);
        OLED_DrawHorzLine(x1+2, y1, x2-x1-3);
        OLED_DrawHorzLine(x1+2, y2, x2-x1-3);
        OLED_DrawVertLine(x1, y1+2, y2-y1-3);
        OLED_DrawVertLine(x2, y1+2, y2-y1-3);
    }
}

void OLED_ClearRoundRectangle(int x1, int y1, int x2, int y2)
{
    int tmp;

    if (x1>x2)
    {
        tmp=x1;
        x1=x2;
        x2=tmp;
    }
    if (y1>y2)
    {
        tmp=y1;
        y1=y2;
        y2=tmp;
    }
    if ((x2-x1)>4 && (y2-y1)>4)
    {
        OLED_ClearPixel(x1+1,y1+1);
        OLED_ClearPixel(x2-1,y1+1);
        OLED_ClearPixel(x1+1,y2-1);
        OLED_ClearPixel(x2-1,y2-1);
        OLED_ClearHorzLine(x1+2, y1, x2-x1-3);
        OLED_ClearHorzLine(x1+2, y2, x2-x1-3);
        OLED_ClearVertLine(x1, y1+2, y2-y1-3);
        OLED_ClearVertLine(x2, y1+2, y2-y1-3);
    }
}

void OLED_DrawCircle(int x, int y, int radius)
{
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int x1 = 0;
    int y1 = radius;


    OLED_SetPixel(x, y + radius);
    OLED_SetPixel(x, y - radius);
    OLED_SetPixel(x + radius, y);
    OLED_SetPixel(x - radius, y);

    while(x1 < y1)
    {
        if(f >= 0) 
        {
            y1--;
            ddF_y += 2;
            f += ddF_y;
        }
        x1++;
        ddF_x += 2;
        f += ddF_x;    
        OLED_SetPixel(x + x1, y + y1);
        OLED_SetPixel(x - x1, y + y1);
        OLED_SetPixel(x + x1, y - y1);
        OLED_SetPixel(x - x1, y - y1);
        OLED_SetPixel(x + y1, y + x1);
        OLED_SetPixel(x - y1, y + x1);
        OLED_SetPixel(x + y1, y - x1);
        OLED_SetPixel(x - y1, y - x1);
    }
}






void OLED_ClearCircle(int x, int y, int radius)
{
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int x1 = 0;
    int y1 = radius;


    OLED_ClearPixel(x, y + radius);
    OLED_ClearPixel(x, y - radius);
    OLED_ClearPixel(x + radius, y);
    OLED_ClearPixel(x - radius, y);

    while(x1 < y1)
    {
        if(f >= 0) 
        {
            y1--;
            ddF_y += 2;
            f += ddF_y;
        }
        x1++;
        ddF_x += 2;
        f += ddF_x;    
        OLED_ClearPixel(x + x1, y + y1);
        OLED_ClearPixel(x - x1, y + y1);
        OLED_ClearPixel(x + x1, y - y1);
        OLED_ClearPixel(x - x1, y - y1);
        OLED_ClearPixel(x + y1, y + x1);
        OLED_ClearPixel(x - y1, y + x1);
        OLED_ClearPixel(x + y1, y - x1);
        OLED_ClearPixel(x - y1, y - x1);
    }
}







void OLED_DrawBitmap(int x, int y, uint8_t* bitmap, int sx, int sy)
{
    int bit,cy,cx;
    uint8_t data;

    for ( cy=0; cy<sy; cy++)
    {
        bit= cy % 8;
        for(cx=0; cx<sx; cx++)
        {
            data=bitmapbyte(cx+((cy/8)*sx));
            if ((data & (1<<bit))>0)
                OLED_SetPixel(x+cx, y+cy);
            else
                OLED_ClearPixel(x+cx, y+cy);
        }
    }      
}
/*************************************************************************************************/










/**************************************************************************************************
                                  Local Functions
**************************************************************************************************/                
void oled_sendStartCondition(uint8_t addr)
{
    GPIO_PinDirection(SDA_PIN, OUTPUT);
    GPIO_PinWrite(SDA_PIN, LOW);  //delay_us(10);
    GPIO_PinWrite(SDA_PIN, HIGH); //delay_us(10);
    GPIO_PinWrite(SCL_PIN, HIGH); //delay_us(10);
    GPIO_PinWrite(SDA_PIN, LOW);  //delay_us(10);
    GPIO_PinWrite(SCL_PIN, LOW);  //delay_us(10);
    oled_shiftOut(addr);
}

void oled_sendStopCondition()
{
    GPIO_PinDirection(SDA_PIN, OUTPUT);    //delay_us(1);
    GPIO_PinWrite(SDA_PIN, LOW);  //delay_us(1);
    GPIO_PinWrite(SCL_PIN, HIGH); //delay_us(1);
    GPIO_PinWrite(SDA_PIN, HIGH); //delay_us(1);
    GPIO_PinDirection(SDA_PIN, INPUT);
}

void oled_sendNack()
{
    GPIO_PinDirection(SDA_PIN, OUTPUT);    //delay_us(1);
    GPIO_PinWrite(SCL_PIN, LOW);  //delay_us(1);
    GPIO_PinWrite(SDA_PIN, HIGH); //delay_us(1);
    GPIO_PinWrite(SCL_PIN, HIGH); //delay_us(1);
    GPIO_PinWrite(SCL_PIN, LOW);  //delay_us(1);
    GPIO_PinDirection(SDA_PIN, INPUT);
}

void oled_sendAck()
{
    GPIO_PinDirection(SDA_PIN, OUTPUT);    //delay_us(1);
    GPIO_PinWrite(SCL_PIN, LOW);  //delay_us(1);
    GPIO_PinWrite(SDA_PIN, LOW);  //delay_us(1);
    GPIO_PinWrite(SCL_PIN, HIGH); //delay_us(1);
    GPIO_PinWrite(SCL_PIN, LOW);  //delay_us(1);
    GPIO_PinDirection(SDA_PIN, INPUT);
}

void oled_waitForAck()
{
    GPIO_PinDirection(SDA_PIN, INPUT);                //delay_us(1);
    GPIO_PinWrite(SCL_PIN, HIGH);            //delay_us(10);
    while (GPIO_PinRead(SDA_PIN)==HIGH) {}
    GPIO_PinWrite(SCL_PIN, LOW);             //delay_us(1);
}

void oled_writeByte(uint8_t value)
{
    GPIO_PinDirection(SDA_PIN, OUTPUT);
    oled_shiftOut(value);
}




void oled_sendCommand(uint8_t value)
{
    oled_sendStartCondition(SSD1306_ADDR<<1);
    oled_waitForAck();
    oled_writeByte(SSD1306_COMMAND);
    oled_waitForAck();
    oled_writeByte(value);
    oled_waitForAck();
    oled_sendStopCondition();
}


void oled_shiftOut(uint8_t value) {
    int i;

    GPIO_PinWrite(SCL_PIN, LOW);
    for ( i = 0; i < 8; i++) 
    {  

        GPIO_PinWrite(SDA_PIN, (value >> (7 - i)) & 0x1);
        GPIO_PinWrite(SCL_PIN, HIGH); 
        GPIO_PinWrite(SCL_PIN, LOW);
    }
}
/**************************************************************************************************/


