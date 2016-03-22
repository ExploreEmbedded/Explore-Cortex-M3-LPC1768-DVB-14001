/***************************************************************************************************
                                   ExploreEmbedded    
****************************************************************************************************
 * File:   softi2c.c
 * Version: 15.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: Contains the library routines for I2C read,write operation

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
****************************************************************************************************
15.0: Initial version 
***************************************************************************************************/
#include "gpio.h"
#include "delay.h"
#include "softi2c.h"

uint8_t SDA_PIN;
uint8_t SCL_PIN;
    
/***************************************************************************************************
                          Local Function declaration
***************************************************************************************************/                          
static void softI2c_Clock(void);
static void softI2c_Ack(void);
static void softI2c_NoAck(void);
/**************************************************************************************************/







/***************************************************************************************************
                 void SoftI2C_Init(uint8_t sda_pin,uint8_t scl_pin)
****************************************************************************************************
 * I/P Arguments: none.
 * Return value  : none

 * description  :This function is used to initialize the I2C module.
***************************************************************************************************/
 void SoftI2C_Init(uint8_t sda_pin,uint8_t scl_pin)
 {
    SDA_PIN = sda_pin;
    SCL_PIN = scl_pin;
    GPIO_PinDirection(SDA_PIN, OUTPUT);
    GPIO_PinDirection(SCL_PIN, OUTPUT);
 }
 
 
 
 

/***************************************************************************************************
                         void SoftI2C_Start(void)
****************************************************************************************************
 * I/P Arguments: none.
 * Return value  : none

 * description  :This function is used to generate I2C Start Condition.
                 Start Condition: SDA goes low when SCL is High.

                               ____________
                SCL:          |            |
                      ________|            |______
                           _________
                SDA:                |
                                    |____________

***************************************************************************************************/
void SoftI2C_Start(void)
{
    GPIO_PinWrite(SCL_PIN,LOW);        // Pull SCL low
    GPIO_PinWrite(SDA_PIN,HIGH);        // Pull SDA High
    DELAY_us(1);
    GPIO_PinWrite(SCL_PIN,HIGH);        //Pull SCL high
    DELAY_us(1);
    GPIO_PinWrite(SDA_PIN,LOW);        //Now Pull SDA LOW, to generate the Start Condition
    DELAY_us(1);
    GPIO_PinWrite(SCL_PIN,LOW);        //Finally Clear the SCL to complete the cycle
}





/***************************************************************************************************
                         void SoftI2C_Stop(void)
****************************************************************************************************
 * I/P Arguments: none.
 * Return value  : none

 * description  :This function is used to generate I2C Stop Condition.
                 Stop Condition: SDA goes High when SCL is High.

                               ____________
                SCL:          |            |
                      ________|            |______
                                 _________________
                SDA:            |
                      __________|

***************************************************************************************************/

void SoftI2C_Stop(void)
{
    GPIO_PinWrite(SCL_PIN,LOW);            // Pull SCL low
    DELAY_us(1);
    GPIO_PinWrite(SDA_PIN,LOW);            // Pull SDA  low
    DELAY_us(1);
    GPIO_PinWrite(SCL_PIN,HIGH);            // Pull SCL High
    DELAY_us(1);
    GPIO_PinWrite(SDA_PIN,HIGH);            // Now Pull SDA High, to generate the Stop Condition
}








/***************************************************************************************************
                         void SoftI2C_Write(uint8_t v_i2cData_u8)
****************************************************************************************************
 * I/P Arguments: uint8_t-->8bit data to be sent.
 * Return value  : none

 * description  :This function is used to send a byte on SDA line using I2C protocol
                 8bit data is sent bit-by-bit on each clock cycle.
                 MSB(bit) is sent first and LSB(bit) is sent at last.
                 Data is sent when SCL is low.

         ___     ___     ___     ___     ___     ___     ___     ___     ___     ___
 SCL:   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
      __|   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |___

 SDA:    D8       D7     D6      D5      D4       D3      D2      D1      D0     ACK


***************************************************************************************************/
void SoftI2C_Write(uint8_t v_i2cData_u8)
{
    uint8_t i;

    for(i=0;i<8;i++)                   // loop 8 times to send 1-byte of data
    {
        GPIO_PinWrite(SDA_PIN,util_GetBitStatus(v_i2cData_u8,7)); // Send Bit by Bit on SDA line starting from MSB
        softI2c_Clock();                   // Generate Clock at SCL
        v_i2cData_u8 = v_i2cData_u8<<1;// Bring the next bit to be transmitted to MSB position
    }
                             
    softI2c_Clock();
}






/***************************************************************************************************
                         uint8_t SoftI2C_Read(uint8_t v_ackOption_u8)
****************************************************************************************************
 * I/P Arguments: none.
 * Return value  : uint8_t(received byte)

 * description :This fun is used to receive a byte on SDA line using I2C protocol.
               8bit data is received bit-by-bit each clock and finally packed into Byte.
               MSB(bit) is received first and LSB(bit) is received at last.


         ___     ___     ___     ___     ___     ___     ___     ___     ___     ___
SCL:    |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
      __|   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |___|   |__

 SDA:    D8       D7     D6      D5       D4     D3       D2      D1     D0      ACK


***************************************************************************************************/
uint8_t SoftI2C_Read(uint8_t v_ackOption_u8)
{
    uint8_t i, v_i2cData_u8=0x00;

    GPIO_PinDirection(SDA_PIN,INPUT);   //Make SDA as I/P
    for(i=0;i<8;i++)     // loop 8times to read 1-byte of data
    {
        DELAY_us(1);
        GPIO_PinWrite(SCL_PIN,HIGH);         // Pull SCL High
        DELAY_us(1);

        v_i2cData_u8 = v_i2cData_u8<<1;    //v_i2cData_u8 is Shifted each time and
        v_i2cData_u8 = v_i2cData_u8 | GPIO_PinRead(SDA_PIN); //ORed with the received bit to pack into byte

        GPIO_PinWrite(SCL_PIN,LOW);         // Clear SCL to complete the Clock
    }
    
    GPIO_PinDirection(SDA_PIN,OUTPUT);  //Make SDA back as O/P
    
    if(v_ackOption_u8==1)  /*Send the Ack/NoAck depending on the user option*/
    {
        softI2c_Ack();
    }
    else
    {
        softI2c_NoAck();
    }
    
    
    
    return v_i2cData_u8;           // Finally return the received Byte*
}






/***************************************************************************************************
                                Local functions
***************************************************************************************************/
                                

/***************************************************************************************************
                         static void softI2c_Clock()
****************************************************************************************************
 * I/P Arguments: none.
 * Return value  : none

 * description  :This function is used to generate a clock pulse on SCL line.
***************************************************************************************************/
static void softI2c_Clock(void)
{
    DELAY_us(1);
    GPIO_PinWrite(SCL_PIN,HIGH);            // Wait for Some time and Pull the SCL line High
    DELAY_us(1);        // Wait for Some time
    GPIO_PinWrite(SCL_PIN,LOW);            // Pull back the SCL line low to Generate a clock pulse
}





/***************************************************************************************************
                         static void softI2c_Ack()
****************************************************************************************************
 * I/P Arguments: none.
 * Return value  : none

 * description  :This function is used to generate the Positive ACK
                 pulse on SDA after receiving a byte.
***************************************************************************************************/
static void softI2c_Ack(void)
{
    GPIO_PinWrite(SDA_PIN,LOW);        //Pull SDA low to indicate Positive ACK
    softI2c_Clock();    //Generate the Clock
    GPIO_PinWrite(SDA_PIN,HIGH);        // Pull SDA back to High(IDLE state)
}





/***************************************************************************************************
                        static void softI2c_NoAck()
****************************************************************************************************
 * I/P Arguments: none.
 * Return value  : none

 * description  :This function is used to generate the Negative/NO ACK
                 pulse on SDA after receiving all bytes.
***************************************************************************************************/
static void softI2c_NoAck(void)
{
    GPIO_PinWrite(SDA_PIN,HIGH);         //Pull SDA high to indicate Negative/NO ACK
    softI2c_Clock();     // Generate the Clock  
    GPIO_PinWrite(SCL_PIN,HIGH);         // Pull SDA back to High(IDLE state)
}
