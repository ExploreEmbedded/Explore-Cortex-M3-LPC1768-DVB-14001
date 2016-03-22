//-----------------------------------------------------------------------------
// Software that is described herein is for illustrative purposes only  
// which provides customers with programming information regarding the  
// products. This software is supplied "AS IS" without any warranties.  
// NXP Semiconductors assumes no responsibility or liability for the 
// use of the software, conveys no license or title under any patent, 
// copyright, or mask work right to the product. NXP Semiconductors 
// reserves the right to make changes in the software without 
// notification. NXP Semiconductors also make no representation or 
// warranty that such application will be suitable for the specified 
// use without further testing or modification. 
//-----------------------------------------------------------------------------

//#include "type.h"
//#include "nvic.h"
#include "LPC17xx.h"
#include "sbl_iap.h"
#include "sbl_config.h"

const uint32_t crp __attribute__((section(".ARM.__at_0x1FC"))) = CRP;

const uint32_t sector_start_map[MAX_FLASH_SECTOR] = {SECTOR_0_START,             \
SECTOR_1_START,SECTOR_2_START,SECTOR_3_START,SECTOR_4_START,SECTOR_5_START,      \
SECTOR_6_START,SECTOR_7_START,SECTOR_8_START,SECTOR_9_START,SECTOR_10_START,     \
SECTOR_11_START,SECTOR_12_START,SECTOR_13_START,SECTOR_14_START,SECTOR_15_START, \
SECTOR_16_START,SECTOR_17_START,SECTOR_18_START,SECTOR_19_START,SECTOR_20_START, \
SECTOR_21_START,SECTOR_22_START,SECTOR_23_START,SECTOR_24_START,SECTOR_25_START, \
SECTOR_26_START,SECTOR_27_START,SECTOR_28_START,SECTOR_29_START					 };

const uint32_t sector_end_map[MAX_FLASH_SECTOR] = {SECTOR_0_END,SECTOR_1_END,    \
SECTOR_2_END,SECTOR_3_END,SECTOR_4_END,SECTOR_5_END,SECTOR_6_END,SECTOR_7_END,   \
SECTOR_8_END,SECTOR_9_END,SECTOR_10_END,SECTOR_11_END,SECTOR_12_END,             \
SECTOR_13_END,SECTOR_14_END,SECTOR_15_END,SECTOR_16_END,SECTOR_17_END,           \
SECTOR_18_END,SECTOR_19_END,SECTOR_20_END,SECTOR_21_END,SECTOR_22_END,           \
SECTOR_23_END,SECTOR_24_END,SECTOR_25_END,SECTOR_26_END,                         \
SECTOR_27_END,SECTOR_28_END,SECTOR_29_END										 };

uint32_t param_table[5];
uint32_t result_table[5];

uint32_t cclk = CCLK;
uint8_t flash_buf[FLASH_BUF_SIZE];

uint32_t * flash_address = 0;
uint32_t byte_ctr = 0;


void write_data(uint32_t cclk,uint32_t flash_address,uint32_t * flash_data_buf, uint32_t count);
void find_erase_prepare_sector(uint32_t cclk, uint32_t flash_address);
void erase_sector(uint32_t start_sector,uint32_t end_sector,uint32_t cclk);
void prepare_sector(uint32_t start_sector,uint32_t end_sector,uint32_t cclk);
void iap_entry(uint32_t param_tab[],uint32_t result_tab[]);
void enable_interrupts(uint32_t interrupts);
void disable_interrupts(uint32_t interrupts);

uint32_t write_flash(uint32_t * dst, uint8_t * src, uint32_t no_of_bytes)
{
  uint32_t i;

    if (flash_address == 0)
	{
	  /* Store flash start address */
	  flash_address = (uint32_t *)dst;
	}
	for( i = 0;i<no_of_bytes;i++ )
	{
	  flash_buf[(byte_ctr+i)] = *(src+i);
    }
	byte_ctr = byte_ctr + no_of_bytes;

	if( byte_ctr == FLASH_BUF_SIZE)
	{
	  /* We have accumulated enough bytes to trigger a flash write */
	  find_erase_prepare_sector(cclk, (uint32_t)flash_address);
      if(result_table[0] != CMD_SUCCESS)
      {
        while(1); /* No way to recover. Just let Windows report a write failure */
      }
      write_data(cclk,(uint32_t)flash_address,(uint32_t *)flash_buf,FLASH_BUF_SIZE);
      if(result_table[0] != CMD_SUCCESS)
      {
        while(1); /* No way to recover. Just let Windows report a write failure */
      }
	  /* Reset byte counter and flash address */
	  byte_ctr = 0;
	  flash_address = 0;
	}
    return(CMD_SUCCESS);
}

void find_erase_prepare_sector(uint32_t cclk, uint32_t flash_address)
{
    uint32_t i;
    uint32_t interrupts;


	disable_interrupts(interrupts);

    for(i=USER_START_SECTOR;i<=MAX_USER_SECTOR;i++)
    {
        if(flash_address < sector_end_map[i])
        {
            if( flash_address == sector_start_map[i])
            {
                prepare_sector(i,i,cclk);
                erase_sector(i,i,cclk);
            }
            prepare_sector(i,i,cclk);
            break;
        }
    }
	enable_interrupts(interrupts);
}

void write_data(uint32_t cclk,uint32_t flash_address,uint32_t * flash_data_buf, uint32_t count)
{
    uint32_t interrupts;

	disable_interrupts(interrupts);
    param_table[0] = COPY_RAM_TO_FLASH;
    param_table[1] = flash_address;
    param_table[2] = (uint32_t)flash_data_buf;
    param_table[3] = count;
    param_table[4] = cclk;
    iap_entry(param_table,result_table);
	enable_interrupts(interrupts);
}

void erase_sector(uint32_t start_sector,uint32_t end_sector,uint32_t cclk)
{
    param_table[0] = ERASE_SECTOR;
    param_table[1] = start_sector;
    param_table[2] = end_sector;
    param_table[3] = cclk;
    iap_entry(param_table,result_table);
}

void prepare_sector(uint32_t start_sector,uint32_t end_sector,uint32_t cclk)
{
    param_table[0] = PREPARE_SECTOR_FOR_WRITE;
    param_table[1] = start_sector;
    param_table[2] = end_sector;
    param_table[3] = cclk;
    iap_entry(param_table,result_table);
}

void iap_entry(uint32_t param_tab[],uint32_t result_tab[])
{
    void (*iap)(uint32_t [],uint32_t []);

    iap = (void (*)(uint32_t [],uint32_t []))IAP_ADDRESS;
    iap(param_tab,result_tab);
}

void enable_interrupts(uint32_t interrupts)
{
    __enable_irq();
}

void disable_interrupts(uint32_t interrupts)
{
    __disable_irq();
}

__asm void boot_jump( uint32_t address ){
   LDR SP, [R0]		;Load new stack pointer address
   LDR PC, [R0, #4]	;Load new program counter address
}

void execute_user_code(void)
{
	/* Change the Vector Table to the USER_FLASH_START 
	in case the user application uses interrupts */
	SCB->VTOR = USER_FLASH_START & 0x1FFFFF80;

	boot_jump(USER_FLASH_START);
}

uint32_t user_code_present(void)
{
    param_table[0] = BLANK_CHECK_SECTOR;
    param_table[1] = USER_START_SECTOR;
    param_table[2] = USER_START_SECTOR;
    iap_entry(param_table,result_table);
	if( result_table[0] == CMD_SUCCESS )
	{
	    return (0);
	}
	else
	{
	    return (1);
	}
}

void check_isp_entry_pin(void)
{
    if( (*(volatile uint32_t *)ISP_ENTRY_GPIO_REG) & (0x1<<ISP_ENTRY_PIN) )
	{
	    execute_user_code();
	}
	else
	{
	    // Enter ISP mode
	}
}

void erase_user_flash(void)
{
    prepare_sector(USER_START_SECTOR,MAX_USER_SECTOR,cclk);
    erase_sector(USER_START_SECTOR,MAX_USER_SECTOR,cclk);
	if(result_table[0] != CMD_SUCCESS)
    {
      while(1); /* No way to recover. Just let Windows report a write failure */
    }
}
