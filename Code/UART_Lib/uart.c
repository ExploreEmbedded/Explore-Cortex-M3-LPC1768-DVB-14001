/***************************************************************************************************
                                   ExploreEmbedded    
 ****************************************************************************************************
 * File:   uart.c (AVR controllers)
 * Version: 15.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: File contains the Library routines for UART

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


/**************************************************************************************************
                             Revision History
 ****************************************************************************************************
15.0: Initial version 
15.1: Updated the UART_TxNumber function to display Bin/Dec/Hex numbers.
      Removed the functions UART_TxHexNumber and UART_TxBinaryNumber.
 ***************************************************************************************************/
#include <stdarg.h>
#include "uart.h"
#include "gpio.h"

#define C_MaxUartChannels_U8 4u
#define C_UartOne_U8         1u


const uartChannelConfig_st STR_UartConfig[C_MaxUartChannels_U8]=
{  /* TxPin RxPin UART_PinFun   PCON Bit Associated UART Structure    */
    { P0_2, P0_3, PINSEL_FUNC_1,  3     ,(LPC_UART_TypeDef *)LPC_UART0_BASE}, /* Configure P0_2,P0_3 for UART0 function */
    { P2_0, P2_1, PINSEL_FUNC_2,  4     ,(LPC_UART_TypeDef *)LPC_UART1_BASE}, /* Configure P2_0,P2_1 for UART1 function */
    { P0_10,P0_11,PINSEL_FUNC_1,  24    ,(LPC_UART_TypeDef *)LPC_UART2_BASE}, /* Configure P0_10,P0_11 for UART2 function */
    { P4_28,P4_29,PINSEL_FUNC_3,  25    ,(LPC_UART_TypeDef *)LPC_UART3_BASE}  /* Configure P4_28,P4_29 for UART3 function */ 
};



/***************************************************************************************************
                    void UART_Init(uint8_t var_uartChannel_u8, uint32_t var_baudRate_u32)
 ****************************************************************************************************
 * I/P Arguments: uint32_t : Baudrate to be configured.
 * Return value    : none

 * description  :This function is used to initialize the UART at specified baud rate.
                 If the requested baud rate is not within the supported range then
                 the default baud rate of 9600 is set.


            Refer uart.h file for Supported(range) baud rates.        
 ***************************************************************************************************/
void UART_Init(uint8_t var_uartChannel_u8, uint32_t var_baudRate_u32)
{    
    if(var_uartChannel_u8< C_MaxUartChannels_U8)
    {	 
        GPIO_PinFunction(STR_UartConfig[var_uartChannel_u8].TxPin,STR_UartConfig[var_uartChannel_u8].PinFunSel);
        GPIO_PinFunction(STR_UartConfig[var_uartChannel_u8].RxPin,STR_UartConfig[var_uartChannel_u8].PinFunSel);
		util_BitSet(LPC_SC->PCONP,STR_UartConfig[var_uartChannel_u8].pconBit);
        
        /* Enable FIFO and reset Rx/Tx FIFO buffers */
        STR_UartConfig[var_uartChannel_u8].UARTx->FCR = (1<<SBIT_FIFO) | (1<<SBIT_RxFIFO) | (1<<SBIT_TxFIFO); 

        /* 8bit data, 1Stop bit, No parity */
        STR_UartConfig[var_uartChannel_u8].UARTx->LCR = (0x03<<SBIT_WordLenght) | (1<<SBIT_DLAB);

        UART_SetBaudRate(var_uartChannel_u8,var_baudRate_u32);    
    }
}






/***************************************************************************************************
                 void UART_SetBaudRate(uint8_t var_uartChannel_u8, uint32_t var_baudRate_u32)
 ***************************************************************************************************
 * I/P Arguments: uint32_t : var_baudRate_u32 to be configured.
 * Return value    : none

 * description  :This function is used to Set/Change the baudrate on the fly.
                 If the requested baud rate is not within the supported range then
                 the default baudrate of 9600 is set.

            Refer uart.h file for Supported range of baud rates.
 ***************************************************************************************************/
void UART_SetBaudRate(uint8_t var_uartChannel_u8, uint32_t var_baudRate_u32)
{
    uint32_t var_UartPclk_u32,var_Pclk_u32,var_RegValue_u32;
    
    if(var_uartChannel_u8 < C_MaxUartChannels_U8 )
    {  
        /** Baud Rate Calculation :
       PCLKSELx registers contains the PCLK info for all the clock dependent peripherals.
       Bit6,Bit7 contains the Uart Clock(ie.UART_PCLK) information.
       The UART_PCLK and the actual Peripheral Clock(PCLK) is calculated as below.
       (Refer data sheet for more info)

       UART_PCLK    PCLK
         0x00       SystemFreq/4        
         0x01       SystemFreq
         0x02       SystemFreq/2
         0x03       SystemFreq/8   
         **/

        var_UartPclk_u32 = (LPC_SC->PCLKSEL0 >> 6) & 0x03;

        switch( var_UartPclk_u32 )
        {
        case 0x00:
            var_Pclk_u32 = SystemFrequency/4;
            break;
        case 0x01:
            var_Pclk_u32 = SystemFrequency;
            break; 
        case 0x02:
            var_Pclk_u32 = SystemFrequency/2;
            break; 
        case 0x03:
            var_Pclk_u32 = SystemFrequency/8;
            break;
        }    

         var_RegValue_u32 = ( var_Pclk_u32 / (16 * var_baudRate_u32 )); 

		 STR_UartConfig[var_uartChannel_u8].UARTx->DLL = util_ExtractByte0to8(var_RegValue_u32);
         STR_UartConfig[var_uartChannel_u8].UARTx->DLM = util_ExtractByte8to16(var_RegValue_u32);
         
         util_BitClear(STR_UartConfig[var_uartChannel_u8].UARTx->LCR, SBIT_DLAB); // Clear DLAB after setting DLL,DLM
    } 
}





/***************************************************************************************************
                    char UART_RxChar(uint8_t var_uartChannel_u8)
 ***************************************************************************************************
 * I/P Arguments: none.
 * Return value    : char: Ascii value of the character received

 * description :This function is used to receive a char from UART module.
                It waits till a char is received and returns it after reception.
 ***************************************************************************************************/
char UART_RxChar(uint8_t var_uartChannel_u8)
{
    char ch = 0; 
    
    if(var_uartChannel_u8 < C_MaxUartChannels_U8 )
    {
          /* Wait till the data is received */
         while(util_IsBitCleared(STR_UartConfig[var_uartChannel_u8].UARTx->LSR,SBIT_RDR)); 
         ch = STR_UartConfig[var_uartChannel_u8].UARTx->RBR; // Copy the received data
    }
    return ch;    
}








/***************************************************************************************************
                         void UART_TxChar(char var_uartData_u8)
 ****************************************************************************************************
 * I/P Arguments: char--> Ascii value of the character to be transmitted.
 * Return value    : none.

 * description  :This function is used to transmit a char through UART module.
 ***************************************************************************************************/
void UART_TxChar(uint8_t var_uartChannel_u8, char var_uartData_u8)
{
    if(var_uartChannel_u8 < C_MaxUartChannels_U8)
    {
       while(util_IsBitCleared(STR_UartConfig[var_uartChannel_u8].UARTx->LSR,SBIT_THRE)); // Wait for Previous transmission
       STR_UartConfig[var_uartChannel_u8].UARTx->THR=var_uartData_u8;   // Load the data to be transmitted   
    }
}







/***************************************************************************************************
                         void UART_TxString(char *ptr_stringPointer_u8)
 ****************************************************************************************************
 * I/P Arguments: String(Address of the string) to be transmitted.
 * Return value    : none

 * description :This function is used to transmit a NULL terminated string through UART.
               1.The ptr_stringPointer_u8 points to the first char of the string
                    and traverses till the end(NULL CHAR) and transmits a char each time
 ***************************************************************************************************/
#if (Enable_UART_TxString==1)
void UART_TxString(uint8_t var_uartChannel_u8, char *ptr_stringPointer_u8)
{
    while(*ptr_stringPointer_u8)
        UART_TxChar(var_uartChannel_u8, *ptr_stringPointer_u8++);// Loop through the string and transmit char by char
}
#endif






/***************************************************************************************************
                         void UART_RxString(char *ptr_stringPointer_u8)
 ****************************************************************************************************
 * I/P Arguments: char *:  Address of the string/buffer where the received data needs to be stored
 * Return value    : none

 * description  :
              1.This function is used to receive a ASCII string through UART till the carriage_return/New_line
              2.The stream of data is copied to the buffer till carriage_return/New_line is encountered.
              3. Once the Carriage_return/New_Line is received the string will be null terminated.

 *****NOTE*******:
  1.The received char is ECHOED back,
    if not required then comment UART_TxChar(ch) in the code.
  2.BackSlash is not taken care.
 ***************************************************************************************************/
#if (Enable_UART_RxString==1)
void UART_RxString(uint8_t var_uartChannel_u8, char *ptr_stringPointer_u8)
{
    char ch;
    while(1)
    {
        ch=UART_RxChar(var_uartChannel_u8);    //Receive a char
        UART_TxChar(var_uartChannel_u8,ch);     //Echo back the received char

        if((ch=='\r') || (ch=='\n')) //read till enter key is pressed
        {                             //once enter key is pressed null terminate the string
            *ptr_stringPointer_u8=0;           //and break the loop
            break;                  
        }
        *ptr_stringPointer_u8=ch;              //copy the char into string and increment the pointer
        ptr_stringPointer_u8++;
    }
}
#endif






/***************************************************************************************************
void UART_TxNumber(uint8_t var_numericSystem_u8, uint32_t var_number_u32, uint8_t var_numOfDigitsToTransmit_u8)
 ***************************************************************************************************
 * I/P Arguments: 
                  uint8_t :  specifies type of number C_BINARY_U8(2),C_DECIMAL_U8(10), C_HEX_U8(16)
                  uint32_t: Number to be transmitted on UART.
                  uint8_t : Number of digits to be transmitted

 * Return value    : none

 * description  :This function is used to transmit a max of 10digit decimal number.
                2nd parameter specifies the number of digits from the right side to be transmitted
                 The output for the input combinations is as below

    Binary:     1.(10,4) then 4-LSB will be transmitted ie. 1010
                2.(10,8) then 8-LSB will be transmitted ie. 00001010
                3.(10,2) then 2-LSB will be transmitted ie. 10     

    Decimal            
                4.(12345,4) then 4-digits ie. 2345 will be transmitted
                5.(12345,6) then 6-digits ie. 012345 will be transmitted
                6.(12345,C_DefaultDigitsToTransmit_U8) then 12345 will be transmitted.

    Hex:
                7.(0x12AB,3) then 3-digits ie. 2AB will be transmitted
                8.(0x12AB,6) then 6-digits ie. 0012AB will be transmitted
                9.(0x12AB,C_DefaultDigitsToTransmit_U8) then 12AB will be transmitted.    
 ***************************************************************************************************/
#if ((Enable_UART_TxNumber==1) || (Enable_UART_TxFloatNumber==1) || (Enable_UART_Printf == 1))
void UART_TxNumber(uint8_t var_uartChannel_u8, uint8_t var_numericSystem_u8, uint32_t var_number_u32, uint8_t var_numOfDigitsToTransmit_u8)
{
    uint8_t i=0,a[10];

    if(C_BINARY_U8 == var_numericSystem_u8)
    {
        while(var_numOfDigitsToTransmit_u8!=0)
        {
            /* Start Extracting the bits from the specified bit positions.
             Get the Acsii values of the bits and transmit */
            i = util_GetBitStatus(var_number_u32,(var_numOfDigitsToTransmit_u8-1));
            UART_TxChar(var_uartChannel_u8,util_Dec2Ascii(i));
            var_numOfDigitsToTransmit_u8--;
        }    
    }     
    else if(var_number_u32==0)
    {
        /* If the number is zero then update the array with the same for transmitting */
        for(i=0;((i<var_numOfDigitsToTransmit_u8) && (i<C_MaxDigitsToTransmit_U8)) ;i++)
            UART_TxChar(var_uartChannel_u8,'0');
    }
    else
    {
        for(i=0;i<var_numOfDigitsToTransmit_u8;i++)
        {
            /* Continue extracting the digits from right side
               till the Specified var_numOfDigitsToTransmit_u8 */
            if(var_number_u32!=0)
            {
                /* Extract the digits from the number till it becomes zero.
                First get the remainder and divide the number by 10 each time.

                example for Decimal number:
                If var_number_u32 = 123 then extracted remainder will be 3 and number will be 12.
                The process continues till it becomes zero or max digits reached*/
                a[i]=util_GetMod32(var_number_u32,var_numericSystem_u8);
                var_number_u32=var_number_u32/var_numericSystem_u8;
            }
            else if( (var_numOfDigitsToTransmit_u8 == C_DefaultDigitsToTransmit_U8) ||
                    (var_numOfDigitsToTransmit_u8 > C_MaxDigitsToTransmit_U8))
            {
                /* Stop the iteration if the Max number of digits are reached or
                 the user expects exact(Default) digits in the number to be transmitted */ 
                break;
            }
            else
            {
                /*In case user expects more digits to be transmitted than the actual digits in number,
                  then update the remaining digits with zero.
                Ex: var_number_u32 is 123 and user wants five digits then 00123 has to be transmitted */
                a[i]=0;
            }
        }

        while(i)
        { 
            /* Finally get the ascii values of the digits and transmit*/
            UART_TxChar(var_uartChannel_u8,util_Hex2Ascii(a[i-1]));
            i--;
        }
    }


}
#endif










/***************************************************************************************************
            void  UART_TxFloatNumber(float var_floatNumber_f32)
 ***************************************************************************************************
 * Function name:  UART_TxFloatNumber()
 * I/P Arguments: float: float Number to be transmitted on UART.

 * Return value    : none

 * description  :This function is used to transmit a floating point number

 * Note         :It supports 6digits of precision.  
         Float will be disabled by default as it takes huge controller resources
         It can be enabled by changing value of Enable_UART_TxFloatNumber to 1 in uart.h     
 ***************************************************************************************************/
#if (Enable_UART_TxFloatNumber==1)
void UART_TxFloatNumber(uint8_t var_uartChannel_u8, float var_floatNumber_f32)
{
    uint32_t var_tempNumber_u32;
    /* Dirty hack to support the floating point by extracting the integer and fractional part.
      1.Type cast the number to int to get the integer part.
      2.transmit the extracted integer part followed by a decimal point(.).
      3.Later the integer part is made zero by subtracting with the extracted integer value.
      4.Finally the fractional part is multiplied by 100000 to support 6-digit precision */

    var_tempNumber_u32 = (uint32_t) var_floatNumber_f32;
    UART_TxNumber(var_uartChannel_u8,C_DECIMAL_U8,var_tempNumber_u32,C_DefaultDigitsToTransmit_U8);

    UART_TxChar(var_uartChannel_u8,'.');

    var_floatNumber_f32 = var_floatNumber_f32 - var_tempNumber_u32;
    var_tempNumber_u32 = var_floatNumber_f32 * 1000000;
    UART_TxNumber(var_uartChannel_u8,C_DECIMAL_U8,var_tempNumber_u32,C_DefaultDigitsToTransmit_U8);
}
#endif






/***************************************************************************************************
            void UART_Printf(const char *argList, ...)
 ***************************************************************************************************
 * Function name:  UART_Printf()
 * I/P Arguments: variable length arguments similar to printf

 * Return value    : none

 * description  :This function is similar to printf function in C.
                 It takes the arguments with specified format and prints accordingly
                 The supported format specifiers are as below.
                 1. %c: character
                 2. %d: signed 16-bit number
                 3. %D: signed 32-bit number
                 4. %u: unsigned 16-bit number
                 5. %U: unsigned 32-bit number
                 6. %b: 16-bit binary number
                 7. %B: 32-bit binary number
                 8. %f: Float number
                 9. %x: 16-bit hexadecimal number
                 10. %X: 32-bit hexadecimal number
                 11. %s: String



  Extra feature is available to specify the number of digits to be transmitted using printf.
     ex: %4d: will transmit the lower four digits of the decimal number.
         %12b: will transmit the 12-LSB of the number
         %d: Will transmit the exact digits of the number

#####: In case of printing the 8-bit variables, it is recommended to type cast and promote them to uint16_t.
        uint8_t var_Num_u8;
        LCD_Printf("num1:%u",(uint16_t)var_Num_u8);         
 ***************************************************************************************************/
#if ( Enable_UART_Printf   == 1 ) 
void UART_Printf(uint8_t var_uartChannel_u8, const char *argList, ...)
{
    const char *ptr;
    double var_floatNum_f32;
    va_list argp;
    sint16_t var_num_s16;
    sint32_t var_num_s32;
    uint16_t var_num_u16;
    uint32_t var_num_u32;
    char *str;
    char  ch;
    uint8_t var_numOfDigitsToTransmit_u8;

    va_start(argp, argList);

    /* Loop through the list to extract all the input arguments */
    for(ptr = argList; *ptr != '\0'; ptr++)
    {

        ch= *ptr;
        if(ch == '%')         /*Check for '%' as there will be format specifier after it */
        {
            ptr++;
            ch = *ptr;
            if((ch>=0x30) && (ch<=0x39))
            {
                var_numOfDigitsToTransmit_u8 = 0;
                while((ch>=0x30) && (ch<=0x39))
                {
                    var_numOfDigitsToTransmit_u8 = (var_numOfDigitsToTransmit_u8 * 10) + (ch-0x30);
                    ptr++;
                    ch = *ptr;
                }
            }
            else
            {
                var_numOfDigitsToTransmit_u8 = C_MaxDigitsToTransmitUsingPrintf_U8;
            }                


            switch(ch)       /* Decode the type of the argument */
            {

            case 'C':
            case 'c':     /* Argument type is of char, hence read char data from the argp */
                ch = va_arg(argp, int);
                UART_TxChar(var_uartChannel_u8,ch);
                break;



            case 'd':    /* Argument type is of signed integer, hence read 16bit data from the argp */
                var_num_s16 = va_arg(argp, int);
#if (Enable_UART_TxNumber == 1)
                if(var_num_s16<0)
                { /* If the number is -ve then display the 2's complement along with '-' sign */ 
                    var_num_s16 = -var_num_s16;
                    UART_TxChar(var_uartChannel_u8,'-');
                }
                UART_TxNumber(var_uartChannel_u8,C_DECIMAL_U8,var_num_s16,var_numOfDigitsToTransmit_u8);
#endif
                break;


			   
            case 'D':    /* Argument type is of integer, hence read 16bit data from the argp */
                var_num_s32 = va_arg(argp, sint32_t);
#if (Enable_UART_TxNumber == 1)                
                if(var_num_s32<0)
                { /* If the number is -ve then display the 2's complement along with '-' sign */
                    var_num_s32 = -var_num_s32;
                    UART_TxChar(var_uartChannel_u8,'-');
                }
                UART_TxNumber(var_uartChannel_u8,C_DECIMAL_U8,var_num_s32,var_numOfDigitsToTransmit_u8);
#endif                
                break;    



            case 'u':    /* Argument type is of unsigned integer, hence read 16bit unsigned data */
                var_num_u16 = va_arg(argp, int);
#if (Enable_UART_TxNumber == 1)                
                UART_TxNumber(var_uartChannel_u8,C_DECIMAL_U8,var_num_u16,var_numOfDigitsToTransmit_u8);
#endif                
                break;



            case 'U':    /* Argument type is of integer, hence read 32bit unsigend data */
                var_num_u32 = va_arg(argp, uint32_t);
#if (Enable_UART_TxNumber == 1)                
                UART_TxNumber(var_uartChannel_u8,C_DECIMAL_U8,var_num_u32,var_numOfDigitsToTransmit_u8);
#endif                
                break;            


            case 'x':  /* Argument type is of hex, hence hexadecimal data from the argp */
                var_num_u16 = va_arg(argp, int);
#if (Enable_UART_TxNumber == 1)                
                UART_TxNumber(var_uartChannel_u8,C_HEX_U8, var_num_u16,var_numOfDigitsToTransmit_u8);
#endif                
                break;



            case 'X':  /* Argument type is of hex, hence hexadecimal data from the argp */
                var_num_u32 = va_arg(argp, uint32_t);
#if (Enable_UART_TxNumber == 1)                        
                UART_TxNumber(var_uartChannel_u8,C_HEX_U8, var_num_u32,var_numOfDigitsToTransmit_u8);
#endif                
                break;



            case 'b':  /* Argument type is of binary,Read int and convert to binary */
                var_num_u16 = va_arg(argp, int);
#if (Enable_UART_TxNumber == 1)                        
                if(var_numOfDigitsToTransmit_u8 == C_MaxDigitsToTransmitUsingPrintf_U8)
                {
                    var_numOfDigitsToTransmit_u8 = 16;
                }
                UART_TxNumber(var_uartChannel_u8,C_BINARY_U8, var_num_u16,var_numOfDigitsToTransmit_u8);
#endif                
                break;



            case 'B':  /* Argument type is of binary,Read int and convert to binary */
                var_num_u32 = va_arg(argp, uint32_t);
#if (Enable_UART_TxNumber == 1)                
                if(var_numOfDigitsToTransmit_u8 == C_MaxDigitsToTransmitUsingPrintf_U8)
                    var_numOfDigitsToTransmit_u8 = 16;                
                UART_TxNumber(var_uartChannel_u8,C_BINARY_U8, var_num_u32,var_numOfDigitsToTransmit_u8);    
#endif                
                break;



            case 'F':
            case 'f': /* Argument type is of float, hence read double data from the argp */
                var_floatNum_f32 = va_arg(argp, double);
#if (Enable_UART_TxFloatNumber == 1)                
                UART_TxFloatNumber(var_uartChannel_u8,var_floatNum_f32);
#endif
                break;



            case 'S':
            case 's': /* Argument type is of string, hence get the pointer to sting passed */
                str = va_arg(argp, char *);
                UART_TxString(var_uartChannel_u8,str);                
                break;



            case '%':
                UART_TxChar(var_uartChannel_u8,'%');
                break;
            }
        }
        else
        {
            /* As '%' is not detected transmit the char passed */
            UART_TxChar(var_uartChannel_u8,ch);
        }
    }

    va_end(argp);
}
#endif

