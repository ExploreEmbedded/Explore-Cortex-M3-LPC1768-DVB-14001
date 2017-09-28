/**********************************************************************************************************************************
                                   Copyright  Notice
 **********************************************************************************************************************************
 * File:   spi.h
 * Version: 16.0
 * Author: CC Dharmani, ExploreEmbedded
 * Website: www.dharmanitech.com, http://www.exploreembedded.com/wiki
 * Description: File contains the functions to read and write data from SD CARD

The original files can be downloaded from www.dharmanitech.com.
The files have been modified to provide the standard interfaces like FILE_Putch, File_Getch etc.
Multiple file accesses is also supported. The max number of files to be accessed can be configured in fat32.h(C_MaxFilesOpening_U8).
The fat32 is tested on atmega8/16/32/128, lpc2148, lpc1114, lpc1768.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
Neither DharmaniTech/ExploreEmbedded shall be responsible for any kind of hardware/system failures due to these libraries.

All the rights will be still with original author www.dharmanitech.com.

********* Information related to Fat file system *******************************
https://www.win.tue.nl/~aeb/linux/fs/fat/fat-1.html
http://www.tavi.co.uk/phobos/fat.html
 *************************************************************************************************************************************/


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
unsigned char SD_erase (uint32_t startBlock, uint32_t totalBlocks);

#endif
