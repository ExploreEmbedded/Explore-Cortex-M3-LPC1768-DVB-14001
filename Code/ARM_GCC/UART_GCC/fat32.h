//**************************************************
// ***** HEADER FILE : FAT32.h ******
//**************************************************
#ifndef _FAT32_H_
#define _FAT32_H_
#include "stdutils.h"
#include "stdint.h"

//Structure to access Master Boot Record for getting info about partioions
struct MBRinfo_Structure{
    unsigned char	nothing[446];		//ignore, placed here to fill the gap in the structure
    unsigned char	partitionData[64];	//partition records (16x4)
    uint16_t	signature;		//0xaa55
};

//Structure to access info of the first partioion of the disk 
struct partitionInfo_Structure{ 				
    unsigned char	status;				//0x80 - active partition
    unsigned char 	headStart;			//starting head
    uint16_t	cylSectStart;		//starting cylinder and sector
    unsigned char	type;				//partition type
    unsigned char	headEnd;			//ending head of the partition
    uint16_t	cylSectEnd;			//ending cylinder and sector
    uint32_t	firstSector;		//total sectors between MBR & the first sector of the partition
    uint32_t	sectorsTotal;		//size of this partition in sectors
};



//Structure to access boot sector data
struct BS_Structure{
    uint8_t jumpBoot[3]; //default: 0x009000EB
    uint8_t OEMName[8];
    uint8_t bytesPerSector[2]; //deafault: 512                    2
    uint8_t sectorPerCluster;
    uint8_t reservedSectorCount[2];
    uint8_t numberofFATs;
    uint8_t rootEntryCount[2];
    uint8_t totalSectors_F16[2]; //must be 0 for FAT32
    uint8_t mediaType;
    uint8_t FATsize_F16[2]; //must be 0 for FAT32
    uint8_t sectorsPerTrack[2];
    uint8_t numberofHeads[2];
    uint8_t hiddenSectors[4];
    uint8_t totalSectors_F32[4];
    uint8_t FATsize_F32[4]; //count of sectors occupied by one FAT
    uint8_t extFlags[2];
    uint8_t FSversion[2]; //0x0000 (defines version 0.0)
    uint8_t rootCluster[4]; //first cluster of root directory (=2)
    uint8_t FSinfo[2]; //sector number of FSinfo structure (=1)
    uint8_t BackupBootSector[2];
    uint8_t reserved[12];
    uint8_t driveNumber;
    uint8_t reserved1;
    uint8_t bootSignature;
    uint8_t volumeID[4];
    uint8_t volumeLabel[11]; //"NO NAME "
    uint8_t fileSystemType[8]; //"FAT32"
    uint8_t bootData[420];
    uint8_t bootEndSignature[2]; //0xaa55
};


//Structure to access FSinfo sector data
struct FSInfo_Structure
{
    uint32_t leadSignature; //0x41615252
    unsigned char reserved1[480];
    uint32_t structureSignature; //0x61417272
    uint32_t freeClusterCount; //initial: 0xffffffff
    uint32_t nextFreeCluster; //initial: 0xffffffff
    unsigned char reserved2[12];
    uint32_t trailSignature; //0xaa550000
};

//Structure to access Directory Entry in the FAT
typedef struct{
    unsigned char name[11];
    unsigned char attrib; //file attributes
    unsigned char NTreserved; //always 0
    unsigned char timeTenth; //tenths of seconds, set to 0 here
    uint16_t createTime; //time file was created
    uint16_t createDate; //date file was created
    uint16_t lastAccessDate;
    uint16_t firstClusterHI; //higher word of the first cluster number
    uint16_t writeTime; //time of last write
    uint16_t writeDate; //date of last write
    uint16_t firstClusterLO; //lower word of the first cluster number
    uint32_t fileSize; //size of file in bytes
}dir_Structure;


#define FAT32_DEBUG_ENABLE   1
#define AVR_CONTROLLER       0



//Attribute definitions for file/directory
#define ATTR_READ_ONLY     0x01
#define ATTR_HIDDEN        0x02
#define ATTR_SYSTEM        0x04
#define ATTR_VOLUME_ID     0x08
#define ATTR_DIRECTORY     0x10
#define ATTR_ARCHIVE       0x20
#define ATTR_LONG_NAME     0x0f


#define END_OF_CLUSTERS    0x0fffffff
#define DIR_ENTRY_SIZE     0x32
#define EMPTY              0x00
#define DELETED            0xe5
#define GET     0
#define SET     1
#define READ	0
#define VERIFY  1
#define WRITE   2
#define APPEND  3
#define ADD		0
#define REMOVE	1
#define TOTAL_FREE   1
#define NEXT_FREE    2
#define GET_LIST     0
#define GET_FILE     1
#define DELETE		 2
#define EOF     26u


#define FAT32_FILE_OPEN_SUCCESSFUL                   0
#define FAT32_FILE_DELETED_OR_NOT_FOUND              1
#define FAT32_FILE_ALREADY_EXISTS                    2
#define FAT32_VALID_FILE_NAME                        3
#define FAT32_INVALID_FLIE_NAME                      4
#define FAT32_NO_FREE_CLUSTERS_FOUND                 5
#define FAT32_FILE_OPENED_CANNOT_BE_DELETED          6
#define FAT32_TO_MANY_FILES_OPENED                   7
#define FAT32_MORE_FILES_To_READ                     8
#define FAT32_END_OF_FILE_LIST                       9

#define CONTINUE_LOOP                   0x55




#define C_MaxFilesOpening_U8     2
#define C_FileAllocated_U8       1
#define C_FileNotAllocated_U8    0
#define C_MaxFileNameSize_U8     12
#define C_MaxPageSize_U16        512


typedef struct
{
    uint32_t firstSector;
    uint32_t cluster;
    uint32_t prevCluster;
    uint32_t appendFileSector;
    uint32_t appendFileLocation;
    uint32_t fileSize;
    uint32_t byteCounter;
    uint32_t blockNumber_u32;
    uint32_t appendStartCluster;
    uint16_t sectorIndex;
    uint16_t bufferIndex_u16;
    uint16_t firstClusterHigh;
    uint16_t firstClusterLow;
    uint8_t fileOperation_u8;
    uint8_t fileOpenedFlag;
    uint8_t fileCreatedFlag;
    uint8_t appendFileFlag;
    uint8_t sectorEndFlag;
    uint8_t endOfFileDetected;
    char fileName[C_MaxFileNameSize_U8];
    char fileBuffer[C_MaxPageSize_U16];
}fileConfig_st;

typedef struct{
    uint32_t fileSize;
    char fileName[C_MaxFileNameSize_U8];
}fileInfo;

fileConfig_st* FILE_Open(char* filename,uint8_t fileOperation,uint8_t *fileOpenSts);
void FILE_Close(fileConfig_st *ptr);
uint8_t FILE_Delete(char *fileName);
char FILE_GetCh(fileConfig_st *filePtr);
void FILE_PutCh(fileConfig_st *filePtr, char data);
uint8_t FILE_GetList (fileInfo *fileList);
void FILE_GetMemoryStatics(uint32_t *ptr_totalMemory, uint32_t *ptr_freeMemory);
uint8_t getBootSectorData (void);


#endif
