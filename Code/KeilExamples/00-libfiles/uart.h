/***************************************************************************************************
                                   ExploreEmbedded	
****************************************************************************************************
 * File:   uart.h	(ARM controllers)
 * Version: 15.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: Contains the baudrate configurations and function prototypes for UART routines

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
 **************************************************************************************************/



/***************************************************************************************************
                             Revision History
****************************************************************************************************
15.0: Initial version 
***************************************************************************************************/
#ifndef _UART_H_
#define _UART_H_

#include <lpc17xx.h>
#include "stdutils.h"
#include "gpio.h"

/***************************************************************************************************
                             Baudrate configurations
***************************************************************************************************/
#define C_MinBaudRate_U32 2400
#define C_MaxBaudRate_U32 115200UL


#define M_GetBaudRateGeneratorValue(pclk,baudrate)  ( pclk / (16 * baudrate ))
/**************************************************************************************************/

/***************************************************************************************************
                                           Control register Bits
***************************************************************************************************/
#define SBIT_WordLenght    0x00u
#define SBIT_DLAB          0x07u
#define SBIT_FIFO          0x00u
#define SBIT_RxFIFO        0x01u
#define SBIT_TxFIFO        0x02u
 
#define SBIT_RDR           0x00u
#define SBIT_THRE          0x05u

/**************************************************************************************************/







/***************************************************************************************************
                           UART structure for channel configuration
***************************************************************************************************/                 
typedef struct
{ 
  gpioPins_et TxPin;
  gpioPins_et RxPin;
  uint8_t PinFunSel;
  uint8_t pconBit;
  LPC_UART_TypeDef *UARTx;
}uartChannelConfig_st;

#define C_UartZero_U8    0u
#define C_UartOne_U8     1u
#define C_UartTwo_U8     2u
#define C_UartThree_U8   3u
/**************************************************************************************************/






/***************************************************************************************************
                      PreCompile configurations to enable/disable the functions
****************************************************************************************************
PreCompile configuration to enable or disable the API's.
 1.Required interfaces can be enabled/disabled by configuring 
   its respective macros to 1/0.
 2. By default all the API's are enabled except for FLOAT transmission.
 3. Transmitting of floating number takes huge controller
    resources and need to be enabled only if required. 
	This implies for other interfaces as well. 
***************************************************************************************************/
#define    Enable_UART_TxString          1
#define    Enable_UART_RxString          1
#define    Enable_UART_TxNumber          1
#define    Enable_UART_TxFloatNumber     1
#define    Enable_UART_Printf            1
/**************************************************************************************************/





/***************************************************************************************************
                             Commonly used UART macros/Constants
***************************************************************************************************/
#define C_DefaultDigitsToTransmit_U8          0xffu    // Will transmit the exact digits in the number
#define C_MaxDigitsToTransmit_U8              10u      // Max decimal/hexadecimal digits to be transmitted
#define C_NumOfBinDigitsToTransmit_U8         16u      // Max bits of a binary number to be transmitted
#define C_MaxDigitsToTransmitUsingPrintf_U8   C_DefaultDigitsToTransmit_U8 /* Max dec/hexadecimal digits to be displayed using printf */
/**************************************************************************************************/





/***************************************************************************************************
                             Function Prototypes
***************************************************************************************************/
void UART_Init(uint8_t var_uartChannel_u8, uint32_t var_baudRate_u32);
void UART_SetBaudRate(uint8_t var_uartChannel_u8, uint32_t var_baudRate_u32);
void UART_TxChar(uint8_t var_uartChannel_u8, char var_uartData_u8);
char UART_RxChar(uint8_t var_uartChannel_u8);
void UART_TxString(uint8_t var_uartChannel_u8, char *ptr_stringPointer_u8);
uint8_t UART_RxString(uint8_t var_uartChannel_u8, char *ptr_string);
void UART_TxNumber(uint8_t var_uartChannel_u8, uint8_t var_numericSystem_u8, uint32_t var_number_u32, uint8_t var_numOfDigitsToTransmit_u8);
void UART_TxFloatNumber(uint8_t var_uartChannel_u8, float var_floatNumber_f32);
void UART_Printf(uint8_t var_uartChannel_u8, const char *argList, ...);
/**************************************************************************************************/





/****************************************************************************************************
                           Function Mapping To Support Muliple Channels
*****************************************************************************************************/
#define UART0_Init(baudrate) UART_Init(C_UartZero_U8,  baudrate)
#define UART1_Init(baudrate) UART_Init(C_UartOne_U8,   baudrate)
#define UART2_Init(baudrate) UART_Init(C_UartTwo_U8,   baudrate)
#define UART3_Init(baudrate) UART_Init(C_UartThree_U8, baudrate)

#define UART0_TxChar(ch) UART_TxChar(C_UartZero_U8,  ch)
#define UART1_TxChar(ch) UART_TxChar(C_UartOne_U8,   ch)
#define UART2_TxChar(ch) UART_TxChar(C_UartTwo_U8,   ch)
#define UART3_TxChar(ch) UART_TxChar(C_UartThree_U8, ch)


#define UART0_RxChar() UART_RxChar(C_UartZero_U8)
#define UART1_RxChar() UART_RxChar(C_UartOne_U8)
#define UART2_RxChar() UART_RxChar(C_UartTwo_U8)
#define UART3_RxChar() UART_RxChar(C_UartThree_U8)


#define UART0_TxString(ptr) UART_TxString(C_UartZero_U8,  ptr)
#define UART1_TxString(ptr) UART_TxString(C_UartOne_U8,   ptr)
#define UART2_TxString(ptr) UART_TxString(C_UartTwo_U8,   ptr)
#define UART3_TxString(ptr) UART_TxString(C_UartThree_U8, ptr)



#define UART0_RxString(ptr) UART_RxString(C_UartZero_U8,  ptr)
#define UART1_RxString(ptr) UART_RxString(C_UartOne_U8,   ptr)
#define UART2_RxString(ptr) UART_RxString(C_UartTwo_U8,   ptr)
#define UART3_RxString(ptr) UART_RxString(C_UartThree_U8, ptr)


#define UART0_TxNumber(NumberSystem, Number, NoOfDigits)  UART_TxNumber(C_UartZero_U8,  NumberSystem, Number, NoOfDigits)
#define UART1_TxNumber(NumberSystem, Number, NoOfDigits)  UART_TxNumber(C_UartOne_U8,   NumberSystem, Number, NoOfDigits)
#define UART2_TxNumber(NumberSystem, Number, NoOfDigits)  UART_TxNumber(C_UartTwo_U8,   NumberSystem, Number, NoOfDigits)
#define UART3_TxNumber(NumberSystem, Number, NoOfDigits)  UART_TxNumber(C_UartThree_U8, NumberSystem, Number, NoOfDigits)


#define UART0_TxFloatNumber(Number)  UART_TxFloatNumber(C_UartZero_U8,  Number)
#define UART1_TxFloatNumber(Number)  UART_TxFloatNumber(C_UartOne_U8,   Number)
#define UART2_TxFloatNumber(Number)  UART_TxFloatNumber(C_UartTwo_U8,   Number)
#define UART3_TxFloatNumber(Number)  UART_TxFloatNumber(C_UartThree_U8, Number)	

#define UART0_Printf(var_arg_list...) UART_Printf(C_UartZero_U8,  var_arg_list)
#define UART1_Printf(var_arg_list...) UART_Printf(C_UartOne_U8,   var_arg_list)
#define UART2_Printf(var_arg_list...) UART_Printf(C_UartTwo_U8,   var_arg_list)
#define UART3_Printf(var_arg_list...) UART_Printf(C_UartThree_U8, var_arg_list)
/****************************************************************************************************/
#endif




