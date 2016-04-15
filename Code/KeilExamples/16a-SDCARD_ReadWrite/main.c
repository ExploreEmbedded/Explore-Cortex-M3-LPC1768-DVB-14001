/***************************************************************************************************
                                   ExploreEmbedded
 ****************************************************************************************************
 * File:   main.c
 * Version: 16.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: File contains the example program to demonstrate the sdcard interface using fat32 file system.

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

#include <lpc17xx.h>
#include "sdcard.h"
#include "uart.h"
#include "fat32.h"
#include "stdutils.h"
#include "delay.h"
#include "spi.h"     


#define FILE_READ         '1'
#define FILE_WRITE        '2'
#define FILE_COPY         '3'
#define FILE_DELETE       '4'
#define FILE_LIST         '5'
#define MEMORY_STATICS    '6'



int main()
{
    uint8_t returnStatus,sdcardType,option;
    char ch,sourceFileName[12],destFileName[12];
    fileConfig_st *srcFilePtr,*destFilePtr;
    fileInfo fileList;
    uint32_t totalMemory,freeMemory;

    SystemInit();
    UART0_Init(9600);
    returnStatus = SD_Init(&sdcardType);

    if(returnStatus)
    {
        if(returnStatus == SDCARD_NOT_DETECTED)
        {
            UART0_TxString("\n\r SD card not detected..");
        }
        else if(returnStatus == SDCARD_INIT_FAILED)
        {
            UART0_TxString("\n\r Card Initialization failed..");
        }
        else if(returnStatus == SDCARD_FAT_INVALID)
        {
            UART0_TxString("\n\r Invalid Fat filesystem");
        }
        while(1);
    }
    else
    {
        UART0_TxString("\n\rSD Card Detected!");
    }


    while(1)
    {
        UART0_TxString("\n\r\n\rPress any key to continue");
        UART0_RxChar();
        UART0_TxString("\n\r\n\r------File options---------\n\r");
        UART0_TxString("1: Read File \n\r");
        UART0_TxString("2: Write File \n\r");
        UART0_TxString("3: File Copy \n\r");
        UART0_TxString("4: Delete File \n\r");
        UART0_TxString("5: Get File List \n\r");
        UART0_TxString("6: Memory Statics \n\r");
        UART0_TxString("--------------------------- \n\r");
        UART0_TxString("Choose one of the options: \n\r");


        do
        {
            option = UART0_RxChar();
        }while((option<'1') || (option>'6'));

        UART0_TxChar(option);
        UART0_TxString("\n\r\n\r");

        switch(option)
        {

        case FILE_READ: /* Read a File */
            UART0_TxString("\n\rEnter File name max 11 chars including file type: ");
            UART0_RxString(sourceFileName);

            srcFilePtr = FILE_Open(sourceFileName,READ,&returnStatus);
            if(srcFilePtr == 0)
            {
                UART0_TxString("\n\rFile Opening Failed");
            }
            else
            {
                 UART0_TxString("File Content: ");
                while(1)
                {
                    ch = FILE_GetCh(srcFilePtr);
                    if(ch == EOF)
                        break;
                    UART0_TxChar(ch);
                }

                FILE_Close(srcFilePtr);
            }
            break;



        case FILE_WRITE: /* Write to a File */
            UART0_TxString("\n\rEnter File name max 11 chars including file type: ");
            UART0_RxString(sourceFileName);

            srcFilePtr = FILE_Open(sourceFileName,WRITE,&returnStatus);
            if(srcFilePtr == 0)
            {
                UART0_TxString("\n\rFile Opening Failed");
            }
            else
            {
                UART0_TxString("\n\rEnter text ending with '>' :");
                while(1)
                {
                    ch = UART0_RxChar();
                    if(ch == '>')
                    {
                        FILE_PutCh(srcFilePtr,EOF);
                        break;
                    }
                    else
                    {   UART0_TxChar(ch);
                    FILE_PutCh(srcFilePtr,ch);
                    }
                }
                UART0_TxString("\n\rData saved to file, closing the file.");
                FILE_Close(srcFilePtr);
            }
            break;



        case FILE_COPY: /* File Copy */
            UART0_TxString("\n\rEnter source File name max 11 chars including file type: ");
            UART0_RxString(sourceFileName);

            srcFilePtr = FILE_Open(sourceFileName,READ,&returnStatus);
            if(srcFilePtr == 0)
            {
                UART0_TxString("\n\rSource File Opening Failed");
            }
            else
            {
                UART0_TxString("\n\rEnter destination File name max 11 chars including file type: ");
                UART0_RxString(destFileName);

                destFilePtr = FILE_Open(destFileName,WRITE,&returnStatus);

                if(destFilePtr == 0)
                {
                    UART0_TxString("\n\rDestination File Opening Failed");

                }
                else
                {
                    UART0_TxString("\n\rCopying the file.");
                    while(1)
                    {
                        ch = FILE_GetCh(srcFilePtr);
                        FILE_PutCh(destFilePtr,ch);
                        if(ch == EOF)
                        {
                            break;
                        }
                    }
                    UART0_TxString("\n\rDone Copying..");
                    FILE_Close(destFilePtr);
                }

                FILE_Close(srcFilePtr);
            }
            break;



        case FILE_DELETE: // FIle Delete
            UART0_TxString("\n\rEnter File name to be deleted max 11 chars including file type: ");
            UART0_RxString(sourceFileName);

            UART0_TxString("\n\rDeleting File: ");
            returnStatus = FILE_Delete(sourceFileName);
            if(returnStatus == FAT32_FILE_OPENED_CANNOT_BE_DELETED)
            {
                UART0_TxString("\n\rFile is open cannot be deleted");
            }
            else
            {
                UART0_TxString("\n\rDone! File Deleted");
            }  			
            break;



        case FILE_LIST: //Print the files with size
            while(1)
            {
                returnStatus = FILE_GetList(&fileList);
                if(returnStatus != FAT32_END_OF_FILE_LIST)
                {
                    UART0_Printf("\n\r%s   %Ubytes",fileList.fileName,fileList.fileSize);
                }
                else
                {
                    break;
                }
            }
            break;


        case MEMORY_STATICS: //Print the SD CARD memory Total/Free in bytes
            UART0_TxString("\n\rMemory Statics is being calculated..");
            FILE_GetMemoryStatics(&totalMemory,&freeMemory);
            UART0_Printf("\n\rTotal memory:%Ubytes  Free memory:%Ubytes",totalMemory,freeMemory);
            break;

        }// End of switch
    }// End of while
}// End of main
