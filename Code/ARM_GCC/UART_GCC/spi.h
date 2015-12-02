
#include "lpc17xx.h"
#include "stdutils.h"


#define MOSI_PIN	P0_18
#define MISO_PIN	P0_17
#define SSEL_PIN    P0_16
#define SCK_PIN		P0_15
#define SPI_FUNCTION  PINSEL_FUNC_3

#define SCK_Freq	1000000L


#define CPHA_BIT 3
#define CPOL_BIT 4
#define MSTR_BIT 5
#define LSBF_BIT 6
#define SPIE_BIT 7
#define SPIF_BIT 7


void SPI_Init(void);
uint8_t SPI_Write (uint8_t var_data_u8);
uint8_t SPI_Read(void);


#define SPI_EnableChipSelect()	 GPIO_PinWrite(SSEL_PIN,LOW)
#define SPI_DisableChipSelect()	 GPIO_PinWrite(SSEL_PIN,HIGH)



