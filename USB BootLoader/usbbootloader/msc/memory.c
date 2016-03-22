/*----------------------------------------------------------------------------
 *      Name:    MEMORY.C
 *      Purpose: USB Mass Storage Demo
 *      Version: V1.10
 *----------------------------------------------------------------------------
 *      This file is part of the uVision/ARM development tools.
 *      This software may only be used under the terms of a valid, current,
 *      end user licence from KEIL for a compatible version of KEIL software
 *      development tools. Nothing else gives you the right to use it.
 *
 *      Copyright (c) 2005-2007 Keil Software.
 *---------------------------------------------------------------------------*/

#include "LPC17xx.h"

#include "type.h"

#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"
#include "usbcore.h"
#include "mscuser.h"

#include "memory.h"
#include "sbl_iap.h"
#include "sbl_config.h"


extern uint8_t  Fat_RootDir[FAT_SIZE + ROOT_DIR_SIZE];  /* RAM to store the file allocation table */
extern uint8_t  RootDirEntry[DIR_ENTRY];                /* Root directry entry constants */
extern uint8_t  CRPEntry[DIR_ENTRY];
extern uint32_t  user_flash_erased;

extern uint32_t Image$$RW_IRAM1$$ZI$$Base;
extern uint32_t Image$$RW_IRAM1$$ZI$$Length;

extern uint32_t Load$$RW_IRAM1$$Base;
extern uint32_t Image$$RW_IRAM1$$RW$$Base;
extern uint32_t Image$$RW_IRAM1$$RW$$Length;

void enter_usb_isp(void);

void  Prepare_Zero( uint32_t * dst, uint32_t len )
{
    while(len)
    {
        *dst++ = 0;

        len -= 4;
    }
}

void Prepare_Copy( uint32_t * src, uint32_t * dst, uint32_t len )
{
    if(dst != src)
    {
        while(len)
        {
            *dst++ = *src++;

            len -= 4;
        }
    }
}

/* Main Program */

int main (void) {

  /* If CRP3 is enabled and user flash start sector is not blank, execute the user code */
  if( crp == CRP3 && user_code_present() )
  {
      execute_user_code();
  }

  /* check_isp_entry_pin() function does not return if isp entry is not requested */
  if( user_code_present() )
  {
      check_isp_entry_pin();
  }
  /* User code not present or isp entry requested */
  enter_usb_isp();
}

void enter_usb_isp(void)
{
  uint32_t n,m,next_cluster;

  SystemInit();

  /* Configure the NVIC Preemption Priority Bits */
  NVIC_SetPriorityGrouping(1);

  /* Set user_flash_erased flag to FALSE. This flag is used detect whether whole
     user flash needs to be erased or not when CRP2 or CRP3 is enabled */
  user_flash_erased = FALSE;
  /* Load$$RW_IRAM1$$Base = Code + RO-data Size; Image$$RW_IRAM1$$RW$$Base = IRAM1 Start Position; Image$$RW_IRAM1$$RW$$Length = RW-data Size; */
  Prepare_Copy((uint32_t *)&Load$$RW_IRAM1$$Base,(uint32_t *)&Image$$RW_IRAM1$$RW$$Base,(uint32_t)&Image$$RW_IRAM1$$RW$$Length);
  /* Image$$RW_IRAM1$$ZI$$Base = ZI-data Start Position; Image$$RW_IRAM1$$ZI$$Length = ZI-data Size; */
  Prepare_Zero((uint32_t *)&Image$$RW_IRAM1$$ZI$$Base,(uint32_t)&Image$$RW_IRAM1$$ZI$$Length);

  /* generate File Allocation Table to save Flash space */
  /* First Two FAT entries are reserved */
  Fat_RootDir[0]= 0xF8;
  Fat_RootDir[1]= 0xFF;
  Fat_RootDir[2]= 0xFF;
  /* Start cluster of a file is indicated by the Directory entry = 2 */
  m = 3;
  for ( n = 3;n < NO_OF_CLUSTERS+2;n+=2) 
  {
	if( n == (NO_OF_CLUSTERS-1) )
	{
	  next_cluster = 0xFFF;
	}
	else
	{
	  next_cluster = n + 1;
	}
  	Fat_RootDir[m] = (uint8_t)n & 0xFF;
  	Fat_RootDir[m+1] = (((uint8_t)next_cluster & 0xF) << 4) | ((uint8_t)(n>>8)&0xF);
  	Fat_RootDir[m+2] = (uint8_t)(next_cluster >> 4) & 0xFF;
    
	m = m+3;
  }
  /* Copy root directory entries */
  for (n = 0; n < DIR_ENTRY ; n++) {             /* Copy Initial Disk Image */
    Fat_RootDir[(FAT_SIZE+n)] = RootDirEntry[n];  /*   from Flash to RAM     */
  }

  /* Update the disk volume label as per the CRP status */
  if ( crp != NOCRP )
  {
      if ( crp == CRP1 )
      {
          Fat_RootDir[(FAT_SIZE+3)] = '1';
      }
      if ( crp == CRP2 )
      {
          Fat_RootDir[(FAT_SIZE+3)] = '2';
      }
      if ( crp == CRP3 )
      {
          Fat_RootDir[(FAT_SIZE+3)] = '3';
      }
	  for (n = 4; n < 11 ; n++)
	  {
          Fat_RootDir[(FAT_SIZE+n)] = CRPEntry[n-4];
	  }
  }	   

  /* Correct file size entry for file firmware.bin */
  Fat_RootDir[FAT_SIZE+60] = (uint8_t)(USER_FLASH_SIZE & 0xFF);
  Fat_RootDir[FAT_SIZE+61] = (uint8_t)(USER_FLASH_SIZE >> 8);
  Fat_RootDir[FAT_SIZE+62] = (uint8_t)(USER_FLASH_SIZE >> 16);
  Fat_RootDir[FAT_SIZE+63] = (uint8_t)(USER_FLASH_SIZE >> 24);

  USB_Init();                               /* USB Initialization */
  USB_Connect(TRUE);                        /* USB Connect */

  while (1);                                /* Loop forever */
}
