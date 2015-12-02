#include "lpc17xx.h"
#include "spi.h"
#include "gpio.h"

extern uint32_t SystemFrequency;
static uint32_t spi_GetPclk(void);


void SPI_Init(void)
{	
	uint8_t var_spiCntValue_u8;

	GPIO_PinFunction(SCK_PIN,SPI_FUNCTION);
	GPIO_PinFunction(MOSI_PIN,SPI_FUNCTION);
	GPIO_PinFunction(MISO_PIN,SPI_FUNCTION);
	GPIO_PinFunction(SSEL_PIN,PINSEL_FUNC_0);

	GPIO_PinDirection(SCK_PIN,OUTPUT);
	GPIO_PinDirection(MOSI_PIN,OUTPUT);
	GPIO_PinDirection(MISO_PIN,INPUT);
	GPIO_PinDirection(SSEL_PIN,OUTPUT);

	SPI_DisableChipSelect();

	LPC_SC->PCONP |= (1 << 8); 		//enable power to spi clock

	var_spiCntValue_u8 = spi_GetPclk()/SCK_Freq;// calculate spi clock
	LPC_SPI->SPCCR =  var_spiCntValue_u8;  // set spi clock

	LPC_SPI->SPCR = ((0<<CPHA_BIT) | (1<<CPOL_BIT) | (1<<MSTR_BIT));
	var_spiCntValue_u8  = LPC_SPI->SPSR;   // Dummy read to clear the flags                             
	var_spiCntValue_u8  = LPC_SPI->SPDR;   // Dummy read to clear the flags
}



uint8_t SPI_Write (uint8_t var_data_u8)
{
	uint8_t dummy = var_data_u8;

	LPC_SPI->SPDR = var_data_u8;
	while(util_GetBitStatus(LPC_SPI->SPSR,SPIF_BIT) == 0);			//wait until data sent.	
	var_data_u8 = LPC_SPI->SPSR;
	var_data_u8 = LPC_SPI->SPDR;	

	return dummy;
}



uint8_t SPI_Read(void)
{
	uint8_t var_data_u8;

	LPC_SPI->SPDR = 0xff; 

	while(util_GetBitStatus(LPC_SPI->SPSR,SPIF_BIT) == 0);
	var_data_u8 = LPC_SPI->SPSR;
	var_data_u8 = (uint8_t)LPC_SPI->SPDR;

	return var_data_u8;
}



static uint32_t spi_GetPclk(void)
{
	uint32_t var_SPIPclk_u32,var_Pclk_u32;
	/** 
	   PCLKSELx registers contains the PCLK info for all the clock dependent peripherals.
	   Bit16,Bit17 contains the SPI Clock(ie.SPI_PCLK) information.
	   The SPI_PCLK and the actual Peripheral Clock(PCLK) is calculated as below.
	   (Refer data sheet for more info)

	   SPI_PCLK    PCLK
	     0x00       SystemFreq/4        
		 0x01       SystemFreq
		 0x02       SystemFreq/2
		 0x03       SystemFreq/8   
	 **/

	var_SPIPclk_u32 = (LPC_SC->PCLKSEL0 >> 16) & 0x03;

	switch( var_SPIPclk_u32 )
	{
	case 0x00:
		var_Pclk_u32 = SystemFrequency/4;		   //SystemFrequency or  SystemCoreClock
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

	return (var_Pclk_u32);
}





