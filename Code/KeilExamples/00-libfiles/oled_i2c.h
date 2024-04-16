/***************************************************************************************************
                                   ExploreEmbedded Copyright Notice 
****************************************************************************************************
 * File:   oled_i2c.c
 * Version: 16.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: SSD1306 I2C OLED library to display strings, numbers, graphs and logos

This code has been developed and tested on ExploreEmbedded boards.  
We strongly believe that the library works on any of development boards for respective controllers. 
Check this link http://www.exploreembedded.com/wiki for awesome tutorials on 8051,PIC,AVR,ARM,Robotics,RTOS,IOT.
ExploreEmbedded invests substantial time and effort developing open source HW and SW tools, to support consider 
buying the ExploreEmbedded boards.
 
The ExploreEmbedded libraries and examples are licensed under the terms of the new-bsd license(two-clause bsd license).
See also: http://www.opensource.org/licenses/bsd-license.php
EXPLOREEMBEDDED DISCLAIMS ANY KIND OF HARDWARE FAILURE RESULTING OUT OF USAGE OF LIBRARIES, DIRECTLY OR
INDIRECTLY. FILES MAY BE SUBJECT TO CHANGE WITHOUT PRIOR NOTICE. THE REVISION HISTORY CONTAINS THE INFORMATION 
RELATED TO UPDATES.
 
Permission to use, copy, modify, and distribute this software and its documentation for any purpose
and without fee is hereby granted, provided that this copyright notices appear in all copies 
and that both those copyright notices and this permission notice appear in supporting documentation.
**************************************************************************************************/


/* The orginal Arduino libaray from rinkyDink is modified for low end controllers with small RAM memory.
    ExploreEmbedded oled_i2c.c library does not have a RAM buffer and hence different fonts and graphics are not supported.
	You can download the arduino library from this link http://www.rinkydinkelectronics.com/library.php?id=79 .
		
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

 
#ifndef OLED_I2C_h
#define OLED_I2C_h

#include "stdutils.h"


#define SSD1306_ADDR		0x3C

#define LEFT	0
#define RIGHT	9999
#define CENTER	9998

#define SSD1306_COMMAND			0x00
#define SSD1306_DATA			0xC0
#define SSD1306_DATA_CONTINUE	0x40

#define RST_NOT_IN_USE	255

// SSD1306 Commandset
// ------------------
// Fundamental Commands
#define SSD1306_SET_CONTRAST_CONTROL					0x81
#define SSD1306_DISPLAY_ALL_ON_RESUME					0xA4
#define SSD1306_DISPLAY_ALL_ON							0xA5
#define SSD1306_NORMAL_DISPLAY							0xA6
#define SSD1306_INVERT_DISPLAY							0xA7
#define SSD1306_DISPLAY_OFF								0xAE
#define SSD1306_DISPLAY_ON								0xAF
#define SSD1306_NOP										0xE3
// Scrolling Commands
#define SSD1306_HORIZONTAL_SCROLL_RIGHT					0x26
#define SSD1306_HORIZONTAL_SCROLL_LEFT					0x27
#define SSD1306_HORIZONTAL_SCROLL_VERTICAL_AND_RIGHT	0x29
#define SSD1306_HORIZONTAL_SCROLL_VERTICAL_AND_LEFT		0x2A
#define SSD1306_DEACTIVATE_SCROLL						0x2E
#define SSD1306_ACTIVATE_SCROLL							0x2F
#define SSD1306_SET_VERTICAL_SCROLL_AREA				0xA3
// Addressing Setting Commands
#define SSD1306_SET_LOWER_COLUMN						0x00
#define SSD1306_SET_HIGHER_COLUMN						0x10
#define SSD1306_MEMORY_ADDR_MODE						0x20
#define SSD1306_SET_COLUMN_ADDR							0x21
#define SSD1306_SET_PAGE_ADDR							0x22
// Hardware Configuration Commands
#define SSD1306_SET_START_LINE							0x40
#define SSD1306_SET_SEGMENT_REMAP						0xA0
#define SSD1306_SET_MULTIPLEX_RATIO						0xA8
#define SSD1306_COM_SCAN_DIR_INC						0xC0
#define SSD1306_COM_SCAN_DIR_DEC						0xC8
#define SSD1306_SET_DISPLAY_OFFSET						0xD3
#define SSD1306_SET_COM_PINS							0xDA
#define SSD1306_CHARGE_PUMP								0x8D
// Timing & Driving Scheme Setting Commands
#define SSD1306_SET_DISPLAY_CLOCK_DIV_RATIO				0xD5
#define SSD1306_SET_PRECHARGE_PERIOD					0xD9
#define SSD1306_SET_VCOM_DESELECT						0xDB


#define fontbyte(x) cfont.font[x]  
#define bitmapbyte(x) bitmap[x]






typedef struct 
{
	uint8_t* font;
	uint8_t x_size;
	uint8_t y_size;
	uint8_t offset;
	uint8_t numchars;
	uint8_t OLED_InvertDisplayed;
}_current_font;


enum BitOrder {
	LSBFIRST = 0,
	MSBFIRST = 1
};



void OLED_Init(uint8_t data_pin, uint8_t sclk_pin);
void OLED_Update(void); 
void OLED_ClearScreen(void);
void OLED_FillScreen(void); 
void OLED_InvertDisplay(uint8_t mode);
void OLED_InvertPixel(uint16_t x, uint16_t y);
void OLED_InvertDisplayText(uint8_t mode);
void OLED_SetBrightness(uint8_t value);  


void OLED_SetFont(uint8_t* font);
void OLED_PrintChar(unsigned char c, int x, int row);
void OLED_Puts(char *st, int x, int y);
void OLED_PrintBinaryNumber(long num, int x, int y, int length);
void OLED_PrintDecimalNumber(long num, int x, int y, int length);
void OLED_PrintHexNumber(long num, int x, int y, int length);
void OLED_PrintFloatNumber(double num, uint8_t dec, int x, int y);



void OLED_SetPixel(uint16_t x, uint16_t y);
void OLED_ClearPixel(uint16_t x, uint16_t y);
void OLED_DrawBitmap(int x, int y, uint8_t* bitmap, int sx, int sy);
void OLED_DrawHorzLine(int x, int y, int l);
void OLED_ClearHorzLine(int x, int y, int l);
void OLED_DrawVertLine(int x, int y, int l);
void OLED_ClearVertLine(int x, int y, int l);
void OLED_DrawLine(int x1, int y1, int x2, int y2);
void OLED_ClearLine(int x1, int y1, int x2, int y2);
void OLED_DrawRectangle(int x1, int y1, int x2, int y2);
void OLED_ClearRectangle(int x1, int y1, int x2, int y2);
void OLED_DrawRoundRectangle(int x1, int y1, int x2, int y2);
void OLED_ClearRoundRectangle(int x1, int y1, int x2, int y2);
void OLED_DrawCircle(int x, int y, int radius);
void OLED_ClearCircle(int x, int y, int radius);


#endif

