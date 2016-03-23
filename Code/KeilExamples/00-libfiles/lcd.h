/***************************************************************************************************
                                   ExploreEmbedded    
*****************************************************************************************************
 * File:   lcd.h
 * Version: 16.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: Contains the LCD port configurations, CMD list, function prototypes

The libraries have been tested on ExploreEmbedded development boards. We strongly believe that the 
library works on any of development boards for respective controllers. However, ExploreEmbedded 
disclaims any kind of hardware failure resulting out of usage of libraries, directly or indirectly.
Files may be subject to change without prior notice. The revision history contains the information 
related to updates. 


GNU GENERAL PUBLIC LICENSE: 
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

Errors and omissions should be reported to codelibraries@exploreembedded.com
***************************************************************************************************/



/***************************************************************************************************
                             Revision History
*****************************************************************************************************
15.0: Initial version 
****************************************************************************************************/


#ifndef _LCD_H_
#define _LCD_H_

#include "stdutils.h"
#include "gpio.h"



/***************************************************************************************************
                 PreCompile configurations to enable/disable the functions
****************************************************************************************************
PreCompile configuration to enable or disable the API's.
 1.Required interfaces can be enabled/disabled by configuring its respective macros to 1/0.
 2. By default all the API's are disabled.
 3. Displaying of floating number takes huge controller resources and need to be enabled only 
    if required. This implies for other interfaces as well. 
****************************************************************************************************/
#define    Enable_LCD_SetCursor              0
#define    Enable_LCD_DisplayString          1
#define    Enable_LCD_ScrollMessage          1
#define    Enable_LCD_DisplayNumber          1
#define    Enable_LCD_DisplayFloatNumber     1
#define    Enable_LCD_Printf                 1
/**************************************************************************************************/







/***************************************************************************************************
                        List of commonly used LCD Commands 
****************************************************************************************************/
#define CMD_LCD_CLEAR                0x01u
#define CMD_LCD_RETURN_HOME          0x02u
#define CMD_DISPLAY_ON_CURSOR_OFF    0x0Cu
#define CMD_DISPLAY_ON_CURSOR_ON     0x0Eu
#define CMD_DISPLAY_ON_CURSOR_BLINK  0x0Fu
#define CMD_LCD_FOUR_BIT_MODE        0x28u
#define CMD_LCD_EIGHT_BIT_MODE       0x38u
/**************************************************************************************************/







/***************************************************************************************************
                             Commonly used LCD macros/Constants
**************************************************************************************************/
#define C_EightBitMode_U8 8u
#define C_FourBitMode_U8  4u

#define C_ScrollDelayTime_U8     200


#define C_DisplayDefaultDigits_U8            0xffu // Will display the exact digits in the number
#define C_MaxDigitsToDisplay_U8              10u   // Max decimal/hexadecimal digits to be displayed
#define C_NumOfBinDigitsToDisplay_U8         16u   // Max bits of a binary number to be displayed
#define C_MaxDigitsToDisplayUsingPrintf_U8   C_DisplayDefaultDigits_U8 /* Max dec/hexadecimal digits to be displayed using printf */
/**************************************************************************************************/







/***************************************************************************************************
                                 Struct/Enums used
****************************************************************************************************/
#define C_LcdLineZero   0x00
#define C_LcdLineOne    0x01
#define C_LcdLineTwo    0x02
#define C_LcdLineThree  0x03
#define C_LcdMaxLines   0x04


typedef struct
{
    uint8_t v_MaxSupportedLines_U8;
    uint8_t v_MaxSupportedChars_U8;
    uint8_t v_LcdMode_U8;
    uint8_t RS;
    uint8_t RW;
    uint8_t EN;
    uint8_t D0;
    uint8_t D1;
    uint8_t D2;
    uint8_t D3;
    uint8_t D4;
    uint8_t D5;
    uint8_t D6;
    uint8_t D7;
}LcdConfig_st;

/**************************************************************************************************/





/***************************************************************************************************
                             Function Prototypes
****************************************************************************************************/
void LCD_Init(uint8_t v_lcdNoOfLines_u8, uint8_t v_MaxCharsPerLine_u8);
void LCD_CmdWrite( uint8_t v_lcdCmd_u8);
void LCD_DisplayChar( char v_lcdData_u8);
void LCD_Clear(void);
void LCD_GoToLine(uint8_t v_lineNumber_u8);
void LCD_GoToNextLine(void);
void LCD_SetCursor(uint8_t v_lineNumber_u8, uint8_t v_charNumber_u8);
void LCD_DisplayString(const char *ptr_stringPointer_u8);
void LCD_ScrollMessage(uint8_t v_lineNumber_u8, char *ptr_msgPointer_u8);
void LCD_DisplayNumber(uint8_t v_numericSystem_u8, uint32_t v_number_u32, uint8_t v_numOfDigitsToDisplay_u8);
void LCD_DisplayFloatNumber(double v_floatNum_f32);
void LCD_Printf(const char *argList, ...);
void LCD_SetUp(uint8_t RS, 
               uint8_t RW, 
               uint8_t EN,
               uint8_t D0, 
               uint8_t D1, 
               uint8_t D2, 
               uint8_t D3,
               uint8_t D4,
               uint8_t D5,
               uint8_t D6,
               uint8_t D7 );
/**************************************************************************************************/

#endif
