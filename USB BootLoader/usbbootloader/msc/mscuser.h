/*----------------------------------------------------------------------------
 *      U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 *      Name:    MSCUSER.H
 *      Purpose: Mass Storage Class Custom User Definitions
 *      Version: V1.10
 *----------------------------------------------------------------------------
 *      This file is part of the uVision/ARM development tools.
 *      This software may only be used under the terms of a valid, current,
 *      end user licence from KEIL for a compatible version of KEIL software
 *      development tools. Nothing else gives you the right to use it.
 *
 *      Copyright (c) 2005-2007 Keil Software.
 *---------------------------------------------------------------------------*/

#ifndef __MSCUSER_H__
#define __MSCUSER_H__


/* Mass Storage Memory Layout */
#define MSC_MemorySize  ( BOOT_SECT_SIZE + FAT_SIZE + ROOT_DIR_SIZE + USER_FLASH_SIZE ) 
#define MSC_BlockSize   512
#define MSC_BlockCount  (MSC_MemorySize / MSC_BlockSize)

#define BOOT_SECT_SIZE 	MSC_BlockSize
#define ROOT_DIR_SIZE   (MSC_BlockSize * 1)
#define FAT_SIZE		(MSC_BlockSize * 2)

#define BLOCKS_PER_CLUSTER  2
#define NO_OF_CLUSTERS 	(MSC_BlockCount/BLOCKS_PER_CLUSTER)

#define DIR_ENTRY 64

/* Max In/Out Packet Size */
#define MSC_MAX_PACKET  64

/* MSC In/Out Endpoint Address */
#define MSC_EP_IN       0x82
#define MSC_EP_OUT      0x02

/* MSC Requests Callback Functions */
extern uint32_t MSC_Reset     (void);
extern uint32_t MSC_GetMaxLUN (void);

/* MSC Bulk Callback Functions */
extern void MSC_GetCBW (void);
extern void MSC_SetCSW (void);
extern void MSC_BulkIn (void);
extern void MSC_BulkOut(void);


#endif  /* __MSCUSER_H__ */
