//**************************************************************
// ****** FUNCTIONS FOR SD RAW DATA TRANSFER *******
//**************************************************************
//Controller: ATmega32 (Clock: 8 Mhz-internal)
//Compiler	: AVR-GCC (winAVR with AVRStudio)
//Version 	: 2.3
//Author	: CC Dharmani, Chennai (India)
//			  www.dharmanitech.com
//Date		: 08 May 2010
//**************************************************************

//**************************************************
// ***** HEADER FILE : SD_routines.h ******
//**************************************************
#ifndef _SD_ROUTINES_H_
#define _SD_ROUTINES_H_

#include "stdutils.h"

//Use following macro if you don't want to activate the multiple block access functions
//those functions are not required for FAT32

#define FAT_TESTING_ONLY

/***********************************************************************************
                               SD CARD INIT STATUS
 ***********************************************************************************/
#define SDCARD_INIT_SUCCESSFUL 0
#define SDCARD_NOT_DETECTED    1
#define SDCARD_INIT_FAILED     2
#define SDCARD_FAT_INVALID     3


#define SDCARD_TYPE_UNKNOWN        0
#define SDCARD_TYPE_STANDARD       1
#define SDCARD_TYPE_HIGH_CAPACITY  2
/***********************************************************************************/











//SD commands, many of these are not used here
#define GO_IDLE_STATE            0
#define SEND_OP_COND             1
#define SEND_IF_COND			 8
#define SEND_CSD                 9
#define STOP_TRANSMISSION        12
#define SEND_STATUS              13
#define SET_BLOCK_LEN            16
#define READ_SINGLE_BLOCK        17
#define READ_MULTIPLE_BLOCKS     18
#define WRITE_SINGLE_BLOCK       24
#define WRITE_MULTIPLE_BLOCKS    25
#define ERASE_BLOCK_START_ADDR   32
#define ERASE_BLOCK_END_ADDR     33
#define ERASE_SELECTED_BLOCKS    38
#define SD_SEND_OP_COND			 41   //ACMD
#define APP_CMD					 55
#define READ_OCR				 58
#define CRC_ON_OFF               59


#define ON     1
#define OFF    0



uint8_t SD_Init(uint8_t *cardType);
unsigned char SD_sendCommand(unsigned char cmd, uint32_t arg);
unsigned char SD_readSingleBlock(char *buffer,uint32_t startBlock);
unsigned char SD_writeSingleBlock(char *buffer,uint32_t startBlock);
unsigned char SD_readMultipleBlock (uint32_t startBlock, uint32_t totalBlocks);
unsigned char SD_writeMultipleBlock(uint32_t startBlock, uint32_t totalBlocks);
unsigned char SD_erase (uint32_t startBlock, uint32_t totalBlocks);

#endif
