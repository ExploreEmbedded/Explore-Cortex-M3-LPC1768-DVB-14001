

#include "spi.h"
#include "sdcard.h"
#include "uart.h"
#include "fat32.h"


uint8_t Var_SdHighcapacityFlag_u8 = 0;
uint8_t init_SdCard(uint8_t *cardType);

//******************************************************************
//Function	: to initialize the SD/SDHC card in SPI mode
//Arguments	: none
//return	: unsigned char; will be 0 if no error,
// 			  otherwise the response byte will be sent
//******************************************************************

uint8_t SD_Init(uint8_t *cardType)
{
    uint8_t response,retry=0;

    SPI_Init();

    do{

        response = init_SdCard(cardType);
        retry++;
    }while((response != SDCARD_INIT_SUCCESSFUL) && (retry!=10) );

    if(response == SDCARD_INIT_SUCCESSFUL)
    {
        response = getBootSectorData (); //read boot sector and keep necessary data in global variables
    }

    return response;
}



//******************************************************************
//Function	: to send a command to SD card
//Arguments	: unsigned char (8-bit command value)
// 			  & uint32_t (32-bit command argument)
//return	: unsigned char; response byte
//******************************************************************
unsigned char SD_sendCommand(unsigned char cmd, uint32_t arg)
{
    unsigned char response, retry=0, status;

    //SD card accepts byte address while SDHC accepts block address in multiples of 512
    //so, if it's SD card we need to convert block address into corresponding byte address by
    //multipying it with 512. which is equivalent to shifting it left 9 times
    //following 'if' loop does that

    if(Var_SdHighcapacityFlag_u8 == 0)
    {
        if(cmd == READ_SINGLE_BLOCK     ||
                cmd == READ_MULTIPLE_BLOCKS  ||
                cmd == WRITE_SINGLE_BLOCK    ||
                cmd == WRITE_MULTIPLE_BLOCKS ||
                cmd == ERASE_BLOCK_START_ADDR||
                cmd == ERASE_BLOCK_END_ADDR )
        {
            arg = arg << 9;
        }
    }


    SPI_EnableChipSelect();


    SPI_Write(cmd | 0x40); //send command, first two bits always '01'
    SPI_Write(arg>>24);
    SPI_Write(arg>>16);
    SPI_Write(arg>>8);
    SPI_Write(arg);


    if(cmd == SEND_IF_COND)	 //it is compulsory to send correct CRC for CMD8 (CRC=0x87) & CMD0 (CRC=0x95)
        SPI_Write(0x87);    //for remaining commands, CRC is ignored in SPI mode
    else
        SPI_Write(0x95);

    while((response = SPI_Read()) == 0xff) //wait response
        if(retry++ > 0xfe) break; //time out error

    if(response == 0x00 && cmd == 58)  //checking response of CMD58
    {
        status = SPI_Read() & 0x40;     //first byte of the OCR register (bit 31:24)
        if(status == 0x40)
        {
            Var_SdHighcapacityFlag_u8 = 1;  //we need it to verify SDHC card
        }
        else
        {
            Var_SdHighcapacityFlag_u8 = 0;
        }

        SPI_Read(); //remaining 3 bytes of the OCR register are ignored here
        SPI_Read(); //one can use these bytes to check power supply limits of SD
        SPI_Read();
    }

    SPI_Read(); //extra 8 CLK
    SPI_DisableChipSelect();

    return response; //return state
}

//*****************************************************************
//Function	: to erase specified no. of blocks of SD card
//Arguments	: none
//return	: unsigned char; will be 0 if no error,
// 			  otherwise the response byte will be sent
//*****************************************************************
unsigned char SD_erase (uint32_t startBlock, uint32_t totalBlocks)
{
    unsigned char response;

    response = SD_sendCommand(ERASE_BLOCK_START_ADDR, startBlock); //send starting block address
    if(response != 0x00) //check for SD status: 0x00 - OK (No flags set)
        return response;

    response = SD_sendCommand(ERASE_BLOCK_END_ADDR,(startBlock + totalBlocks - 1)); //send end block address
    if(response != 0x00)
        return response;

    response = SD_sendCommand(ERASE_SELECTED_BLOCKS, 0); //erase all selected blocks
    if(response != 0x00)
        return response;

    return 0; //normal return
}

//******************************************************************
//Function	: to read a single block from SD card
//Arguments	: none
//return	: unsigned char; will be 0 if no error,
// 			  otherwise the response byte will be sent
//******************************************************************
unsigned char SD_readSingleBlock(char *inputbuffer,uint32_t startBlock)
{
    unsigned char response;
    uint16_t i, retry=0;

    response = SD_sendCommand(READ_SINGLE_BLOCK, startBlock); //read a Block command

    if(response != 0x00)
    {
        return response; //check for SD status: 0x00 - OK (No flags set)
    }

    SPI_EnableChipSelect();

    retry = 0;
    while(SPI_Read() != 0xfe) //wait for start block token 0xfe (0x11111110)
    {
        if(retry++ > 0xfffe)
        {
            SPI_DisableChipSelect();
            return 1; //return if time-out
        }
    }

    for(i=0; i<512; i++) //read 512 bytes
        inputbuffer[i] = SPI_Read();

    SPI_Read(); //receive incoming CRC (16-bit), CRC is ignored here
    SPI_Read();

    SPI_Read(); //extra 8 clock pulses
    SPI_DisableChipSelect();

    return 0;
}

//******************************************************************
//Function	: to write to a single block of SD card
//Arguments	: none
//return	: unsigned char; will be 0 if no error,
// 			  otherwise the response byte will be sent
//******************************************************************
unsigned char SD_writeSingleBlock(char *inputbuffer,uint32_t startBlock)
{
    unsigned char response;
    uint16_t i, retry=0;

    response = SD_sendCommand(WRITE_SINGLE_BLOCK, startBlock); //write a Block command

    if(response != 0x00) return response; //check for SD status: 0x00 - OK (No flags set)

    SPI_EnableChipSelect();

    SPI_Write(0xfe);     //Send start block token 0xfe (0x11111110)

    for(i=0; i<512; i++)    //send 512 bytes data
        SPI_Write(inputbuffer[i]);

    SPI_Write(0xff);     //transmit dummy CRC (16-bit), CRC is ignored here
    SPI_Write(0xff);

    response = SPI_Read();

    if( (response & 0x1f) != 0x05) //response= 0xXXX0AAA1 ; AAA='010' - data accepted
    {                              //AAA='101'-data rejected due to CRC error
        SPI_DisableChipSelect();              //AAA='110'-data rejected due to write error
        return response;
    }

    while(!SPI_Read()) //wait for SD card to complete writing and get idle
    {
        if(retry++ > 0xfffe)
        {
            SPI_DisableChipSelect();
            return 1;
        }
    }

    SPI_DisableChipSelect();
    SPI_Write(0xff);   //just spend 8 clock cycle delay before reasserting the CS line
    SPI_EnableChipSelect();         //re-asserting the CS line to verify if card is still busy

    while(!SPI_Read()) //wait for SD card to complete writing and get idle
    {
        if(retry++ > 0xfffe)
        {
            SPI_DisableChipSelect();
            return 1;
        }
    }
    SPI_DisableChipSelect();


    return 0;
}


uint8_t init_SdCard(uint8_t *cardType)
{
    uint8_t  i, response, sd_version;
    uint16_t retry=0 ;

    for(i=0;i<10;i++)
        SPI_Write(0xff);   //80 clock pulses spent before sending the first command

    SPI_EnableChipSelect();
    do
    {

        response = SD_sendCommand(GO_IDLE_STATE, 0); //send 'reset & go idle' command
        retry++;
        if(retry>0x20)
            return SDCARD_NOT_DETECTED;   //time out, card not detected

    } while(response != 0x01);

    SPI_DisableChipSelect();
    SPI_Write (0xff);
    SPI_Write (0xff);

    retry = 0;

    sd_version = 2; //default set to SD compliance with ver2.x;
    //this may change after checking the next command
    do
    {
        response = SD_sendCommand(SEND_IF_COND,0x000001AA); //Check power supply status, mendatory for SDHC card
        retry++;
        if(retry>0xfe)
        {
            
            sd_version = 1;
            *cardType = SDCARD_TYPE_STANDARD;
            break;
        } //time out

    }while(response != 0x01);

    retry = 0;

    do
    {
        response = SD_sendCommand(APP_CMD,0); //CMD55, must be sent before sending any ACMD command
        response = SD_sendCommand(SD_SEND_OP_COND,0x40000000); //ACMD41

        retry++;
        if(retry>0xfe)
        {
            
            return SDCARD_INIT_FAILED;  //time out, card initialization failed
        }

    }while(response != 0x00);


    retry = 0;
    Var_SdHighcapacityFlag_u8 = 0;

    if (sd_version == 2)
    {
        do
        {
            response = SD_sendCommand(READ_OCR,0);
            retry++;
            if(retry>0xfe)
            {
                
                *cardType = SDCARD_TYPE_UNKNOWN;
                break;
            } //time out

        }while(response != 0x00);

        if(Var_SdHighcapacityFlag_u8 == 1)
        {
            *cardType = SDCARD_TYPE_HIGH_CAPACITY;
        }
        else
        {
            *cardType = SDCARD_TYPE_STANDARD;
        }
    }

    return response;
}

