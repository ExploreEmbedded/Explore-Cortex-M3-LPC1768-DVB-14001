/**********************************************************************************************************************************
                                   Copyright  Notice
 **********************************************************************************************************************************
 * File:   fat32.c
 * Version: 16.0
 * Author: CC Dharmani, ExploreEmbedded
 * Website: www.dharmanitech.com, http://www.exploreembedded.com/wiki
 * Description: File contains the functions to read and write data from file

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

#include <string.h>
#include "stdutils.h"
#include "fat32.h"
#include "uart.h"
#include "sdcard.h"



/***************************************************************************************
                           Global Variables
 ****************************************************************************************/
uint32_t firstDataSector, rootCluster, totalClusters;
uint16_t  bytesPerSector, sectorPerCluster, reservedSectorCount;
uint32_t unusedSectors;
uint8_t freeClusterCountUpdated;

/* If only file operation is selected then GlobalBuffer shares the same RAM space.
   If simultaneously more than one file operation are selected then separate GlobalBuffer space is used.
 */
#if (C_MaxFilesOpening_U8 == 1)
fileConfig_st fatMappingTable[C_MaxFilesOpening_U8];
#define GlobalBuffer fatMappingTable[0].fileBuffer
#else
fileConfig_st fatMappingTable[C_MaxFilesOpening_U8];
char  GlobalBuffer[512];
#endif

/***************************************************************************************/




/*****************************************************************************************************************
                                          Local Functions
 *****************************************************************************************************************/
uint8_t getBootSectorData (void);
uint32_t getFirstSector(uint32_t clusterNumber);
uint32_t getSetFreeCluster(char *inputBuffer,uint8_t totOrNext, uint8_t get_set, uint32_t FSEntry);
uint32_t getSetNextCluster (char *inputBuffer,uint32_t clusterNumber,uint8_t get_set,uint32_t clusterEntry);
uint8_t openFile (uint8_t fileOperation, fileConfig_st *filePtr);
uint8_t convertFileName (char *inputFileName);
uint32_t searchNextFreeCluster (char *inputBuffer,uint32_t startCluster);
void freeMemoryUpdate (char *inputBuffer,uint8_t flag, uint32_t size);
uint8_t deletefile (char *fileName);
//void print_DebugMsg(char *strPtr);
uint16_t file_GetTime(void);
uint16_t file_GetDate(void);
/*****************************************************************************************************************/





/****************************************************************************************************************
                 fileConfig_st* FILE_Open(char* filename,uint8_t fileOperation,uint8_t *fileOpenSts)
 ****************************************************************************************************************
 * I/P Arguments :
                  char *: Name of the file to be opened.
                  uint8_t: READ, WRITE, APPEND
                  char *: pointer to update the file opening status.

 * Return value  :
                  fileConfig_st *: Pointer to file structure if opened successfully.
                                 : NULL, if file is not opened.

 * description :
                 This functions opens the file with requested mode(Read/Write). 
                 If the file is successfully opened, it returns the pointer to file structure.
                 In case the file is not opened it returns NULL, fileOpenSts contains the failure cause.
 ******************************************************************************************************************/
fileConfig_st* FILE_Open(char* filename,uint8_t fileOperation,uint8_t *fileOpenSts)
{
    fileConfig_st *filePtr = 0x00;
    uint8_t i,freeLocationDetected = FALSE;
    uint32_t nextCluster, clusterCount;

    for(i=0;((i< C_MaxFilesOpening_U8) && (freeLocationDetected == FALSE));i++)
    {
        if(fatMappingTable[i].fileOpenedFlag == FALSE)
        {
            freeLocationDetected = TRUE;

            filePtr = &fatMappingTable[i];
            filePtr->fileOpenedFlag = TRUE;
            strcpy(filePtr->fileName,filename);
            filePtr->fileOperation_u8 = fileOperation;
            filePtr->firstSector= 0x00;
            filePtr->cluster= 0x00;
            filePtr->blockNumber_u32 = 0x00;
            filePtr->appendFileSector= 0x00;
            filePtr->appendFileLocation= 0x00;
            filePtr->fileSize= 0x00;
            filePtr->byteCounter= 0x00;
            filePtr->appendStartCluster= 0x00;
            filePtr->sectorIndex= 0x00;
            filePtr->bufferIndex_u16= 0x00;

            filePtr->fileCreatedFlag = 0x00;
            filePtr->endOfFileDetected = 0x00;

            if(fileOperation == READ)
            {
                *fileOpenSts = openFile( READ, filePtr);
                if(*fileOpenSts != FAT32_FILE_OPEN_SUCCESSFUL)
                {

                    print_DebugMsg("FILE_DELETED_OR_NOT_FOUND");
                    return  0; /* Set the pointer to NULL indication File not found or Read failed, */
                }
            }
            else if((fileOperation == WRITE) || (fileOperation == APPEND))
            {
                *fileOpenSts = openFile (VERIFY, filePtr);

                if(*fileOpenSts == FAT32_FILE_OPEN_SUCCESSFUL)
                {
                    print_DebugMsg("File already existing, appending data..");

                    filePtr->appendFileFlag = 1;
                    filePtr->cluster = filePtr->appendStartCluster;
                    clusterCount=0;
                    while(1)
                    {
                        nextCluster = getSetNextCluster (filePtr->fileBuffer,filePtr->cluster, GET, 0);
                        if(nextCluster == END_OF_CLUSTERS) break;
                        filePtr->cluster = nextCluster;
                        clusterCount++;
                    }

                    filePtr->sectorIndex = (filePtr->fileSize - (clusterCount * sectorPerCluster * bytesPerSector)) / bytesPerSector;

                    filePtr->blockNumber_u32 = getFirstSector (filePtr->cluster) + filePtr->sectorIndex;
                    SD_readSingleBlock (filePtr->fileBuffer,filePtr->blockNumber_u32);
                    filePtr->bufferIndex_u16 = filePtr->fileSize % bytesPerSector;
                }
                else if(*fileOpenSts == FAT32_FILE_DELETED_OR_NOT_FOUND)
                {
                    *fileOpenSts = FAT32_FILE_OPEN_SUCCESSFUL;

                    print_DebugMsg("Creating File..");
                    filePtr->cluster = getSetFreeCluster (filePtr->fileBuffer,NEXT_FREE, GET, 0);
                    if(filePtr->cluster > totalClusters)
                    {
                        filePtr->cluster = rootCluster;
                    }
                    print_DebugMsg("searchNextFreeCluster..");
                    filePtr->cluster = searchNextFreeCluster(filePtr->fileBuffer,filePtr->cluster);
                    if(filePtr->cluster == 0)
                    {
                        print_DebugMsg("No free cluster!");
                        *fileOpenSts = FAT32_NO_FREE_CLUSTERS_FOUND;
                        return 0;
                    } 

                    print_DebugMsg("get/Set NextCluster..");   //last cluster of the file, marked END_OF_CLUSTERS
                    getSetNextCluster(filePtr->fileBuffer,filePtr->cluster, SET, END_OF_CLUSTERS);

                    filePtr->firstClusterHigh = (uint16_t) ((filePtr->cluster & 0xffff0000) >> 16 );
                    filePtr->firstClusterLow = (uint16_t) ( filePtr->cluster & 0x0000ffff);
                    filePtr->fileSize = 0;

                    filePtr->blockNumber_u32 = getFirstSector (filePtr->cluster);
                    filePtr->bufferIndex_u16=0;
                    filePtr->sectorIndex=0;
                }
                else
                {
                    filePtr = 0;
                }
            }

            if(filePtr!= 0)
            {
                fatMappingTable[i].fileOpenedFlag = C_FileAllocated_U8;
            }
        }
    }

    if(freeLocationDetected == FALSE)
    {
        *fileOpenSts = FAT32_TO_MANY_FILES_OPENED;
    }

    return filePtr;
}




/****************************************************************************************************************
                         void FILE_Close(fileConfig_st *ptr)
 ****************************************************************************************************************
 * I/P Arguments :
                  fileConfig_st *: structure pointer containing the file info that needs to be closed.

 * Return value  :
                  none

 * description :
                 This functions closes the file and releases the memory used by the file.
                 Once the file is closed, no more file operation are allowed.
 ******************************************************************************************************************/
void FILE_Close(fileConfig_st *ptr)
{
    if(ptr!= 0)
        ptr->fileOpenedFlag = FALSE; /* Clear the file Opened flag */
}




/****************************************************************************************************************
                         uint8_t FILE_Delete(char *fileName)
 ****************************************************************************************************************
 * I/P Arguments :
                  char *: Name of the file to be deleted.

 * Return value  :
                  uint8_t:
                  FAT32_INVALID_FLIE_NAME--> In case file name is not valid.
                  FAT32_FILE_OPENED_CANNOT_BE_DELETED--> File cannot be deleted as it is open.
                  FAT32_FILE_DELETED_OR_NOT_FOUND --> File is deleted or not found.

 * description :
                 This functions deletes the file from SD card .
 ******************************************************************************************************************/
uint8_t FILE_Delete(char *fileName)
{
    uint8_t returnStatus,i;
    char localfileName[20];
    strcpy(localfileName,fileName);

    returnStatus = convertFileName (localfileName);

    if(returnStatus == FAT32_VALID_FILE_NAME)
    {
        print_DebugMsg("Searching for Opened File");
        for(i=0;i<C_MaxFilesOpening_U8;i++)
        {
            if(fatMappingTable[i].fileOpenedFlag == TRUE)
            {
                /* Check the file to be deleted is already opened */
                if(strcmp(localfileName,fatMappingTable[i].fileName)==0)
                {
                    print_DebugMsg("File already Opened");
                    break;
                }
            }
        }

        if(i<C_MaxFilesOpening_U8)
        {
            returnStatus = FAT32_FILE_OPENED_CANNOT_BE_DELETED;
        }
        else
        {
            print_DebugMsg("Deleting File");
            deletefile(localfileName);
            returnStatus = FAT32_FILE_DELETED_OR_NOT_FOUND;
        }
    }

    return returnStatus;
}






/****************************************************************************************************************
                         char FILE_GetCh(fileConfig_st *filePtr)
 ****************************************************************************************************************
 * I/P Arguments :
                  fileConfig_st *: structure pointer containing the file info from which the data needs to be read.

 * Return value  :
                   char : Byte of data read from file.


 * description :
                 This functions reads a byte of data from the file.
                 It checks whether the requested file is opened or not, it reads the data only if the file is open else returns EOF.
                 Once the End of File is reached it returns EOF(26). User shoukd stop calling the function after this.
 ******************************************************************************************************************/
char FILE_GetCh(fileConfig_st *filePtr)
{
    char ch;

    if(((filePtr->byteCounter) < (filePtr->fileSize))   &&  (filePtr->fileOpenedFlag == TRUE))
    {
        if(filePtr->bufferIndex_u16 == 0)
        {
            if((filePtr->sectorIndex == 0))
            {
                filePtr->firstSector = getFirstSector (filePtr->cluster);
            }
            SD_readSingleBlock(filePtr->fileBuffer,filePtr->firstSector + filePtr->sectorIndex);
        }

        ch = filePtr->fileBuffer[filePtr->bufferIndex_u16];
        filePtr->bufferIndex_u16 ++;
        filePtr->byteCounter++;


        if(filePtr->bufferIndex_u16 == 512)
        {
            filePtr->sectorIndex++;
            filePtr->bufferIndex_u16 = 0;
            if(filePtr->sectorIndex == sectorPerCluster)
            {
                filePtr->sectorIndex = 0;
                filePtr->cluster = getSetNextCluster (filePtr->fileBuffer,filePtr->cluster, GET, 0);
            }
        }
    }
    else
    {
        ch = EOF;
    }

    return ch;
}




/****************************************************************************************************************
                         void FILE_PutCh (fileConfig_st *filePtr, char data)
 ****************************************************************************************************************
 * I/P Arguments :
                  fileConfig_st *: structure pointer containing the file info where the data needs to be written.
                  char           : Byte of data to be written to the file.

 * Return value  :
                  none


 * description :
                 This functions writes a byte of data to the file.
                 It checks whether the requested file is opened or not, it writes the data only if the file is open.

           Note: CTRL+Z(26) needs to be passed to mark the end of file. After which it saves the file and updates the FAT.
 ******************************************************************************************************************/
void FILE_PutCh (fileConfig_st *filePtr, char data)
{
    uint8_t k;
    dir_Structure *dir;
    uint32_t extraMemory;
    if(filePtr->fileOpenedFlag == TRUE)
    {
        if((filePtr->endOfFileDetected == 0) && (data != EOF))
        {
            if(filePtr->sectorEndFlag == 1) //special case when the last character in previous sector was '\r'
            {
                filePtr->fileBuffer[filePtr->bufferIndex_u16++] = '\n'; //appending 'Line Feed (LF)' character
                filePtr->fileSize++;
                filePtr->sectorEndFlag = 0;
            }

            if(data == 0x08)    //'Back Space' key pressed
            {
                if(filePtr->bufferIndex_u16 != 0)
                {
                    filePtr->bufferIndex_u16--;
                    filePtr->fileSize--;
                }
            }
            else
            {
                filePtr->fileBuffer[filePtr->bufferIndex_u16++] = data;
                filePtr->fileSize++;
            }


            if(data == '\r')  //'Carriege Return (CR)' character
            {
                if(filePtr->bufferIndex_u16 == 512)
                    filePtr->sectorEndFlag = 1;  //flag to indicate that the appended '\n' char should be put in the next sector
                else
                {
                    filePtr->fileBuffer[filePtr->bufferIndex_u16++] = '\n'; //appending 'Line Feed (LF)' character
                    filePtr->fileSize++;
                }
            }

            if(filePtr->bufferIndex_u16 >= 512)   //though 'i' will never become greater than 512, it's kept here to avoid
            {               //infinite loop in case it happens to be greater than 512 due to some data corruption
                filePtr->bufferIndex_u16=0;
                SD_writeSingleBlock (filePtr->fileBuffer,filePtr->blockNumber_u32);
                filePtr->sectorIndex++;
                if(filePtr->sectorIndex == sectorPerCluster)
                {
                    filePtr->sectorIndex = 0;
                    filePtr->prevCluster = filePtr->cluster;

                    filePtr->cluster = searchNextFreeCluster(filePtr->fileBuffer,filePtr->prevCluster); //look for a free cluster starting from the current cluster

                    if(filePtr->cluster == 0)
                    {
                        print_DebugMsg("No free cluster!");
                        return;
                    }

                    getSetNextCluster(filePtr->fileBuffer,filePtr->prevCluster, SET, filePtr->cluster);
                    getSetNextCluster(filePtr->fileBuffer,filePtr->cluster, SET, END_OF_CLUSTERS);   //last cluster of the file, marked END_OF_CLUSTERS

                    filePtr->blockNumber_u32 = getFirstSector (filePtr->cluster);
                    filePtr->bufferIndex_u16=0;
                }
                filePtr->blockNumber_u32++;
            }
        }
        else if(filePtr->endOfFileDetected == 0)
        {
            filePtr->endOfFileDetected = 1;
            for(;filePtr->bufferIndex_u16<512;filePtr->bufferIndex_u16++)  //fill the rest of the buffer with 0x00
                filePtr->fileBuffer[filePtr->bufferIndex_u16]= 0x00;

            SD_writeSingleBlock (filePtr->fileBuffer,filePtr->blockNumber_u32);


            getSetFreeCluster (filePtr->fileBuffer,NEXT_FREE, SET, filePtr->cluster); //update FSinfo next free cluster entry

            if(filePtr->appendFileFlag)  //executes this loop if file is to be appended
            {
                SD_readSingleBlock (filePtr->fileBuffer,filePtr->appendFileSector);
                dir = (dir_Structure *) &filePtr->fileBuffer[filePtr->appendFileLocation];
                extraMemory = filePtr->fileSize - dir->fileSize;
                dir->fileSize = filePtr->fileSize;
                SD_writeSingleBlock (filePtr->fileBuffer,filePtr->appendFileSector);
                freeMemoryUpdate (filePtr->fileBuffer,REMOVE, extraMemory); //updating free memory count in FSinfo sector;
                print_DebugMsg("File appended!");
                return;
            }

            //executes following portion when new file is created

            filePtr->prevCluster = rootCluster; //root cluster

            while(1)
            {
                filePtr->firstSector = getFirstSector (filePtr->prevCluster);

                for(filePtr->sectorIndex = 0; filePtr->sectorIndex < sectorPerCluster; filePtr->sectorIndex++)
                {
                    SD_readSingleBlock (filePtr->fileBuffer,filePtr->firstSector + filePtr->sectorIndex);


                    for(filePtr->bufferIndex_u16=0; filePtr->bufferIndex_u16<bytesPerSector; filePtr->bufferIndex_u16+=32)
                    {
                        dir = (dir_Structure *) &filePtr->fileBuffer[filePtr->bufferIndex_u16];

                        if(filePtr->fileCreatedFlag)   //to mark last directory entry with 0x00 (empty) mark
                        {                     //indicating end of the directory file list
                            dir->name[0] = 0x00;
                            return;
                        }

                        if((dir->name[0] == EMPTY) || (dir->name[0] == DELETED))  //looking for an empty slot to enter file info
                        {
                            for(k=0; k<11; k++)
                                dir->name[k] = filePtr->fileName[k];
                            dir->attrib = ATTR_ARCHIVE; //settting file attribute as 'archive'
                            dir->NTreserved = 0;            //always set to 0
                            dir->timeTenth = 0;         //always set to 0
                            dir->createTime = file_GetTime();       // time of creation
                            dir->createDate = file_GetDate();       // date of creation
                            dir->lastAccessDate = file_GetDate();   // date of last access
                            dir->writeTime = file_GetTime();        // time of last write
                            dir->writeDate = file_GetDate();        // date of last write
                            dir->firstClusterHI = filePtr->firstClusterHigh;
                            dir->firstClusterLO = filePtr->firstClusterLow;
                            dir->fileSize = filePtr->fileSize;

                            SD_writeSingleBlock (filePtr->fileBuffer,filePtr->firstSector + filePtr->sectorIndex);
                            filePtr->fileCreatedFlag = 1;
                            print_DebugMsg("File Created!");
                            freeMemoryUpdate (filePtr->fileBuffer,REMOVE, filePtr->fileSize); //updating free memory count in FSinfo sector
                        }
                    }
                }

                filePtr->cluster = getSetNextCluster (filePtr->fileBuffer,filePtr->prevCluster, GET, 0);

                if(filePtr->cluster > 0x0ffffff6)
                {
                    if(filePtr->cluster == END_OF_CLUSTERS)   //this situation will come when total files in root is multiple of (32*sectorPerCluster)
                    {
                        filePtr->cluster = searchNextFreeCluster(filePtr->fileBuffer,filePtr->prevCluster); //find next cluster for root directory entries
                        getSetNextCluster(filePtr->fileBuffer,filePtr->prevCluster, SET, filePtr->cluster); //link the new cluster of root to the previous cluster
                        getSetNextCluster(filePtr->fileBuffer,filePtr->cluster, SET, END_OF_CLUSTERS);  //set the new cluster as end of the root directory
                    }
                    else
                    {
                        print_DebugMsg("End of Cluster Chain");
                        return;
                    }
                }
                if(filePtr->cluster == 0)
                {
                    print_DebugMsg("Error in getting cluster");
                    return;
                }

                filePtr->prevCluster = filePtr->cluster;
            }
        }
        else
        {

        }
    }
    else
    {
        /* FIle not opened, do not handle the file operation */
    }

    return;
}






/****************************************************************************************************************
                         uint8_t FILE_GetList (fileInfo *fileList)
 ****************************************************************************************************************
 * I/P Arguments :
                  fileInfo *: structure pointer to store the filename and size..

 * Return value  :
                  uint8_t : FAT32_MORE_FILES_To_READ(8)--> If there are files to be listed.
                            FAT32_END_OF_FILE_LIST(9)  --> If there are no more files to be listed.


 * description :
                 This functions traverses through the SD card and lists the available files with their size.
                 It updates the file name and its size in the structure pointer passed to the function.
                 It returns FAT32_MORE_FILES_To_READ(8), if there are files to be listed.
                 In case there are no files to be listed it returns FAT32_END_OF_FILE_LIST(9).
 ******************************************************************************************************************/
uint8_t FILE_GetList (fileInfo *fileList)
{
    static uint32_t clusterNumber = 0xffffffff, sectorNumber,firstSectorIndex = 0;
    dir_Structure *dir;
    static uint16_t i;
    uint8_t j;
    uint8_t fileNameIndex = 0;

    if(clusterNumber == 0xffffffff)
    {
        clusterNumber = rootCluster;
        sectorNumber = 0;
        i = 0;
        firstSectorIndex = getFirstSector (clusterNumber);
        SD_readSingleBlock (GlobalBuffer,firstSectorIndex + sectorNumber);
    }

    do
    {
        dir = (dir_Structure *) &GlobalBuffer[i];

        if(dir->name[0] == EMPTY) //indicates end of the file list of the directory
        {
            print_DebugMsg("File does not exist!");
            clusterNumber = 0xffffffff;
            return (FAT32_END_OF_FILE_LIST);
        }
        if((dir->name[0] != DELETED) && (dir->attrib != ATTR_LONG_NAME))
        {

            for(j=0; j<11; j++)
            {
                if(j == 8)
                {
                    fileList->fileName[fileNameIndex++] = ('.');
                }


                if(dir->name[j] != ' ')
                {
                    fileList->fileName[fileNameIndex++] = dir->name[j];
                }
            }
            fileList->fileName[fileNameIndex] = 0;
            fileList->fileSize = dir->fileSize;
        }

        i = i+32;

        if(i >= bytesPerSector)
        {
            i = 0;
            sectorNumber++;

            if(sectorNumber == sectorPerCluster)
            {
                sectorNumber = 0;
                clusterNumber = (getSetNextCluster (GlobalBuffer,clusterNumber, GET, 0));

                if(clusterNumber > 0x0ffffff6)
                {
                    print_DebugMsg("End of Cluster");
                    clusterNumber = 0xffffffff;
                    return FAT32_END_OF_FILE_LIST;
                }

                if(clusterNumber == 0)
                {
                    print_DebugMsg("Error in getting cluster");
                    clusterNumber = 0xffffffff;;
                    return FAT32_END_OF_FILE_LIST;
                }

                firstSectorIndex = getFirstSector (clusterNumber);
            }

            SD_readSingleBlock(GlobalBuffer,firstSectorIndex + sectorNumber);
        }
    }while(fileNameIndex == 0);


    return FAT32_MORE_FILES_To_READ;
}




/****************************************************************************************************************
      void FILE_GetMemoryStatics(uint32_t *ptr_totalMemory, uint32_t *ptr_freeMemory)
 ****************************************************************************************************************
 * I/P Arguments :
                  uint32_t *: pointer to store the media size in bytes.
                  uint32_t *: Pointer to store free memory in bytes.

 * Return value  :
                  none

 * description :
                 This functions calculates the total and free memory of the SD card.
                 Note: It takes upto 10secs(for 32GB) to calculate the memory statics depending on the size of CARD
 ******************************************************************************************************************/
void FILE_GetMemoryStatics(uint32_t *ptr_totalMemory, uint32_t *ptr_freeMemory)
{
    uint32_t freeClusters = 0, totalClusterCount, cluster;
    uint32_t sector, *value;
    uint8_t i;

    *ptr_totalMemory = totalClusters * sectorPerCluster / 1024;
    *ptr_totalMemory *= bytesPerSector;

    freeClusters = getSetFreeCluster (GlobalBuffer,TOTAL_FREE, GET, 0);

    if(freeClusters > totalClusters)
    {
        freeClusterCountUpdated = 0;
        freeClusters = 0;
        totalClusterCount = 0;
        cluster = rootCluster;
        while(1)
        {
            sector = unusedSectors + reservedSectorCount + ((cluster * 4) / bytesPerSector) ;
            SD_readSingleBlock(GlobalBuffer,sector);
            for(i=0; i<128; i++)
            {
                value = (uint32_t *) &GlobalBuffer[i*4];
                if(((*value)& 0x0fffffff) == 0)
                    freeClusters++;;

                totalClusterCount++;
                if(totalClusterCount == (totalClusters+2)) break;
            }
            if(i < 128) break;
            cluster+=128;
        }
    }

    *ptr_freeMemory = freeClusters * sectorPerCluster / 1024;
    *ptr_freeMemory *= bytesPerSector ;
}




/***********************************************************************************************************************************************
                                     Local Functions
 ************************************************************************************************************************************************/
uint32_t extract_DataBySize(uint8_t *ptr,uint8_t size)
{
    uint8_t i;
    uint32_t v_data_u32 = 0;
    for(i=0;i<size;i++)
    {
        v_data_u32 += (ptr[i] << (8 * i));
    }
    return v_data_u32;
} 




uint8_t getBootSectorData (void)
{
    struct BS_Structure *bpb; //mapping the buffer onto the structure
    struct MBRinfo_Structure *mbr;
    uint32_t dataSectors;


    unusedSectors = 0;

    SD_readSingleBlock(GlobalBuffer,0);
    bpb = (struct BS_Structure *)GlobalBuffer;



    if(bpb->jumpBoot[0]!=0xE9 && bpb->jumpBoot[0]!=0xEB)   //check if it is boot sector
    {
        mbr = (struct MBRinfo_Structure *) GlobalBuffer;       //if it is not boot sector, it must be MBR



        if(mbr->signature != 0xaa55)
        {
            return SDCARD_FAT_INVALID;       //if it is not even MBR then it's not FAT32
        }

        //partition = (struct partitionInfo_Structure *)(mbr->partitionData);//first partition
        unusedSectors = extract_DataBySize(((uint8_t*)GlobalBuffer)+454,4);//partition->firstSector; //the unused sectors, hidden to the FAT

        SD_readSingleBlock(GlobalBuffer,unusedSectors);//read the bpb sector
        bpb = (struct BS_Structure *)GlobalBuffer;
        if(bpb->jumpBoot[0]!=0xE9 && bpb->jumpBoot[0]!=0xEB)
        {
            return SDCARD_FAT_INVALID;
        }
    }

    bytesPerSector = extract_DataBySize((uint8_t*)bpb->bytesPerSector,2);
    sectorPerCluster = bpb->sectorPerCluster;
    reservedSectorCount = extract_DataBySize((uint8_t*)bpb->reservedSectorCount,2);
    rootCluster = extract_DataBySize((uint8_t*)bpb->rootCluster,4);

    firstDataSector = extract_DataBySize((uint8_t*)bpb->hiddenSectors,4) + reservedSectorCount + (bpb->numberofFATs * extract_DataBySize(bpb->FATsize_F32,4));

    dataSectors = (extract_DataBySize((uint8_t*)bpb->totalSectors_F32,4)) - reservedSectorCount - ( (bpb->numberofFATs) * extract_DataBySize(bpb->FATsize_F32,4));
    totalClusters = dataSectors / sectorPerCluster;

    if((getSetFreeCluster (GlobalBuffer,TOTAL_FREE, GET, 0)) > totalClusters)  //check if FSinfo free clusters count is valid
        freeClusterCountUpdated = 0;
    else
        freeClusterCountUpdated = 1;

    return SDCARD_INIT_SUCCESSFUL;
}




//***************************************************************************
//Function: to calculate first sector address of any given cluster
//Arguments: cluster number for which first sector is to be found
//return: first sector address
//***************************************************************************
uint32_t getFirstSector(uint32_t clusterNumber)
{
    return (((clusterNumber - 2) * sectorPerCluster) + firstDataSector);
}




//***************************************************************************
//Function: get cluster entry value from FAT to find out the next cluster in the chain
//or set new cluster entry in FAT
//Arguments: 1. current cluster number, 2. get_set (=GET, if next cluster is to be found or = SET,
//if next cluster is to be set 3. next cluster number, if argument#2 = SET, else 0
//return: next cluster number, if if argument#2 = GET, else 0
//****************************************************************************
uint32_t getSetNextCluster(char *inputBuffer,uint32_t clusterNumber,uint8_t get_set,uint32_t clusterEntry)
{
    uint16_t FATEntryOffset;
    uint32_t *FATEntryValue;
    uint32_t FATEntrySector;
    uint8_t retry = 0;

    //get sector number of the cluster entry in the FAT
    FATEntrySector = unusedSectors + reservedSectorCount + ((clusterNumber * 4) / bytesPerSector) ;

    //get the offset address in that sector number
    FATEntryOffset = (uint16_t) ((clusterNumber * 4) % bytesPerSector);

    //read the sector into a buffer
    while(retry <10)
    {
        if(!SD_readSingleBlock(inputBuffer,FATEntrySector))
            break;
        retry++;
    }

    //get the cluster address from the buffer
    FATEntryValue = (uint32_t *) &inputBuffer[FATEntryOffset];

    if(get_set == GET)
        return ((*FATEntryValue) & 0x0fffffff);


    *FATEntryValue = clusterEntry;   //for setting new value in cluster entry in FAT
    SD_writeSingleBlock(inputBuffer,FATEntrySector);

    return (0);
}





//********************************************************************************************
//Function: to get or set next free cluster or total free clusters in FSinfo sector of SD card
//Arguments: 1.flag:TOTAL_FREE or NEXT_FREE, 
//			 2.flag: GET or SET 
//			 3.new FS entry, when argument2 is SET; or 0, when argument2 is GET
//return: next free cluster, if arg1 is NEXT_FREE & arg2 is GET
//        total number of free clusters, if arg1 is TOTAL_FREE & arg2 is GET
//		  0xffffffff, if any error or if arg2 is SET
//********************************************************************************************
uint32_t getSetFreeCluster(char *inputBuffer, uint8_t totOrNext, uint8_t get_set, uint32_t FSEntry)
{
    struct FSInfo_Structure *FS = (struct FSInfo_Structure *) &inputBuffer;

    SD_readSingleBlock(inputBuffer,unusedSectors + 1);

    if((FS->leadSignature != 0x41615252) || (FS->structureSignature != 0x61417272) || (FS->trailSignature !=0xaa550000))
        return 0xffffffff;

    if(get_set == GET)
    {
        if(totOrNext == TOTAL_FREE)
            return(FS->freeClusterCount);
        else // when totOrNext = NEXT_FREE
            return(FS->nextFreeCluster);
    }
    else
    {
        if(totOrNext == TOTAL_FREE)
            FS->freeClusterCount = FSEntry;
        else // when totOrNext = NEXT_FREE
            FS->nextFreeCluster = FSEntry;

        SD_writeSingleBlock(inputBuffer,unusedSectors + 1);	//update FSinfo
    }
    return 0xffffffff;
}




//***************************************************************************
//Function: to search for the next free cluster in the root directory
//          starting from a specified cluster
//Arguments: Starting cluster
//return: the next free cluster
//****************************************************************
uint32_t searchNextFreeCluster (char *inputBuffer, uint32_t startCluster)
{
    uint32_t cluster, *value, sector;
    uint8_t i;

    startCluster -=  (startCluster % 128);   //to start with the first file in a FAT sector
    for(cluster =startCluster; cluster <totalClusters; cluster+=128)
    {
        sector = unusedSectors + reservedSectorCount + ((cluster * 4) / bytesPerSector);
        SD_readSingleBlock(inputBuffer,sector);
        for(i=0; i<128; i++)
        {
            value = (uint32_t *) &inputBuffer[i*4];
            if(((*value) & 0x0fffffff) == 0)
            {
                return(cluster+i);
            }
        }
    }
    return 0;
}





//***************************************************************************
//Function: to convert normal short file name into FAT format
//Arguments: pointer to the file name
//return: 0, if successful else 1.
//***************************************************************************
uint8_t convertFileName (char *inputFileName)
{
    uint8_t fileNameFAT[11];
    uint8_t j, k;

    for(j=0; j<12; j++)
        if(inputFileName[j] == '.') break;


    if(j>8)
    {
        return FAT32_INVALID_FLIE_NAME;
    }

    for(k=0; k<j; k++) //setting file name
        fileNameFAT[k] = inputFileName[k];

    for(k=j; k<=7; k++) //filling file name trail with blanks
        fileNameFAT[k] = ' ';

    j++;
    for(k=8; k<11; k++) //setting file extention
    {
        if(inputFileName[j] != 0)
            fileNameFAT[k] = inputFileName[j++];
        else //filling extension trail with blanks
            while(k<11)
                fileNameFAT[k++] = ' ';
    }

    for(j=0; j<11; j++) //converting small letters to caps
    {
        if((fileNameFAT[j] >= 0x61) && (fileNameFAT[j] <= 0x7a))
        {
            inputFileName[j] = fileNameFAT[j] - 0x20;
        }
        else
        {
            inputFileName[j] = fileNameFAT[j];
        }
    }

    return FAT32_VALID_FILE_NAME;
}


//************************************************************************************
//Function: to create a file in FAT32 format in the root directory if given 
//			file name does not exist; if the file already exists then append the data
//Arguments: pointer to the file name
//return: none
//************************************************************************************
uint8_t openFile (uint8_t fileOperation, fileConfig_st *filePtr)
{
    uint8_t returnStatus;
    uint32_t cluster = rootCluster, sector, firstSector;
    dir_Structure *dir;
    uint16_t i;
    uint8_t j;


    returnStatus = convertFileName (filePtr->fileName); //convert fileName into FAT format

    if(returnStatus == FAT32_INVALID_FLIE_NAME)
    {
        print_DebugMsg("Invalid fileName..");
    }
    else
    {
        print_DebugMsg("Valid File Name");


        returnStatus = CONTINUE_LOOP;

        do
        {
            firstSector = getFirstSector (cluster);

            for(sector = 0; ((sector < sectorPerCluster) && (returnStatus == CONTINUE_LOOP)); sector++)
            {
                SD_readSingleBlock (filePtr->fileBuffer,firstSector + sector);


                for(i=0; ((i<bytesPerSector) && (returnStatus == CONTINUE_LOOP)); i+=32)
                {
                    dir = (dir_Structure *) &filePtr->fileBuffer[i];

                    if(dir->name[0] == EMPTY) //indicates end of the file list of the directory
                    {
                        print_DebugMsg("File does not exist!");
                        returnStatus = FAT32_FILE_DELETED_OR_NOT_FOUND;
                        break;
                    }
                    if((dir->name[0] != DELETED) && (dir->attrib != ATTR_LONG_NAME))
                    {
                        for(j=0; j<11; j++)
                        {
                            if(dir->name[j] != filePtr->fileName[j])
                                break;
                        }
                        if(j == 11)
                        {
                            filePtr->appendFileSector = firstSector + sector;
                            filePtr->appendFileLocation = i;
                            filePtr->appendStartCluster = (((uint32_t) dir->firstClusterHI) << 16) | dir->firstClusterLO;
                            filePtr->fileSize = dir->fileSize;
                            returnStatus = FAT32_FILE_OPEN_SUCCESSFUL;
                            break;
                        }
                    }
                }
            }

            if(returnStatus == CONTINUE_LOOP)
            {
                cluster = (getSetNextCluster (filePtr->fileBuffer,cluster, GET, 0));

                if((cluster > 0x0ffffff6) || (cluster == 0))
                {
                    print_DebugMsg("Error in getting cluster");
                    returnStatus = FAT32_NO_FREE_CLUSTERS_FOUND;
                }
            }
        }while(returnStatus == CONTINUE_LOOP);


        if((returnStatus == FAT32_FILE_OPEN_SUCCESSFUL) && (fileOperation == READ ))
        {
            filePtr->cluster = (((uint32_t) dir->firstClusterHI) << 16) | dir->firstClusterLO;

            filePtr->fileSize = dir->fileSize;
        }
    }


    return returnStatus;
}








//********************************************************************
//Function: update the free memory count in the FSinfo sector. 
//			Whenever a file is deleted or created, this function will be called
//			to ADD or REMOVE clusters occupied by the file
//Arguments: #1.flag ADD or REMOVE #2.file size in Bytes
//return: none
//********************************************************************
void freeMemoryUpdate (char *inputBuffer, uint8_t flag, uint32_t size)
{
    uint32_t freeClusters;
    //convert file size into number of clusters occupied
    if((size % 512) == 0) size = size / 512;
    else size = (size / 512) +1;
    if((size % 8) == 0) size = size / 8;
    else size = (size / 8) +1;

    if(freeClusterCountUpdated)
    {
        freeClusters = getSetFreeCluster (inputBuffer,TOTAL_FREE, GET, 0);
        if(flag == ADD)
            freeClusters = freeClusters + size;
        else  //when flag = REMOVE
            freeClusters = freeClusters - size;
        getSetFreeCluster (inputBuffer,TOTAL_FREE, SET, freeClusters);
    }
}






uint8_t deletefile (char *fileName)
{
    unsigned long cluster, sector, firstSector, firstCluster, nextCluster;
    dir_Structure *dir;
    unsigned int i;
    uint8_t j;


    cluster = rootCluster; //root cluster

    while(1)
    {
        firstSector = getFirstSector (cluster);

        for(sector = 0; sector < sectorPerCluster; sector++)
        {
            SD_readSingleBlock (GlobalBuffer,firstSector + sector);


            for(i=0; i<bytesPerSector; i+=32)
            {
                dir = (dir_Structure *) &GlobalBuffer[i];

                if(dir->name[0] == EMPTY) //indicates end of the file list of the directory
                {
                    print_DebugMsg("File does not exist!");
                    return FAT32_FILE_DELETED_OR_NOT_FOUND;
                }
                if((dir->name[0] != DELETED) && (dir->attrib != ATTR_LONG_NAME))
                {
                    for(j=0; j<11; j++)
                        if(dir->name[j] != fileName[j]) break;
                    if(j == 11)
                    {
                        print_DebugMsg("Deleting..");
                        firstCluster = (((unsigned long) dir->firstClusterHI) << 16) | dir->firstClusterLO;

                        //mark file as 'deleted' in FAT table
                        dir->name[0] = DELETED;
                        SD_writeSingleBlock (GlobalBuffer,firstSector+sector);

                        freeMemoryUpdate (GlobalBuffer,ADD, dir->fileSize);

                        //update next free cluster entry in FSinfo sector
                        cluster = getSetFreeCluster (GlobalBuffer,NEXT_FREE, GET, 0);
                        if(firstCluster < cluster)
                            getSetFreeCluster (GlobalBuffer,NEXT_FREE, SET, firstCluster);

                        //mark all the clusters allocated to the file as 'free'
                        while(1)
                        {
                            nextCluster = getSetNextCluster(GlobalBuffer,firstCluster, GET, 0);
                            getSetNextCluster(GlobalBuffer,firstCluster, SET, 0);
                            if(nextCluster > 0x0ffffff6)
                            {
                                print_DebugMsg("File deleted!");
                                return FAT32_FILE_DELETED_OR_NOT_FOUND;
                            }
                            firstCluster = nextCluster;
                        }
                    }
                }
            }
        }

        cluster = getSetNextCluster (GlobalBuffer,cluster, GET, 0);

        if((cluster > 0x0ffffff6) || (cluster ==0))
        {
            print_DebugMsg("Error in getting cluster");
            return FAT32_FILE_DELETED_OR_NOT_FOUND;
        }
    }
}


/****************************************************************************************
                  Fat32 date and time encoding
 ****************************************************************************************
Each directory is of 32bytes.

Bytes   Content
0-10    File name (8 bytes) with extension (3 bytes)
11      Attribute - a bitvector. Bit 0: read only. Bit 1: hidden.
        Bit 2: system file. Bit 3: volume label. Bit 4: subdirectory.
        Bit 5: archive. Bits 6-7: unused.
12-21   Reserved (see below)
22-23   Time (5/6/5 bits, for hour/minutes/doubleseconds)
24-25   Date (7/4/5 bits, for year-since-1980/month/day)
26-27   Starting cluster (0 for an empty file)
28-31   Filesize in bytes 
 ****************************************************************************************/
uint16_t file_GetTime(void)
{
    uint8_t hour=13,min=40,sec=30;
    uint16_t fat32Time;
    // RTC_GetTime(&hour,&min,&sec);
    fat32Time = ((hour & 0x1f)<<11) | ((min&0x3f)<<5) | ((sec*2) & 0x1F);

    return fat32Time;
}


uint16_t file_GetDate(void)
{
    uint8_t day = 2, month=10;
    uint16_t fat32Date,year=2015;
    //	 RTC_GetDate(&year,&month,&day);
    fat32Date = ((year - 1980)<<9) | ((month & 0x0f)<<5) | (day & 0x1F);

    return fat32Date;
}

/*************************************************************End of Local Functions*************************************************************/

