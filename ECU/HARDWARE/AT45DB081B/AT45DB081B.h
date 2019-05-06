#ifndef _AT45DB081B_H
#define _AT45DB081B_H
#include "spi.h"
#include "sys.h"

//W25X系列/Q系列芯片列表	   
//W25Q80  ID  0XEF13
//W25Q16  ID  0XEF14
//W25Q32  ID  0XEF15
//W25Q64  ID  0XEF16	
//W25Q128 ID  0XEF17	
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17

extern u16 AT45DB081B_TYPE;					//定义AT45DB081B芯片型号		   

#define	AT45DB081B_CS 		PAout(8)  		//AT45DB081B的片选信号

////////////////////////////////////////////////////////////////////////////////// 

#ifndef MAX_AT45_DEVICES
#define MAX_AT45_DEVICES        1
#endif

#ifndef MAX_AT45_CMDLEN
#define MAX_AT45_CMDLEN         8
#endif

#ifndef AT45_CONF_DFSPI
#define AT45_CONF_DFSPI         SPI0_BASE
#endif

#ifndef AT45_CONF_DFPCS
#define AT45_CONF_DFPCS         1
#endif

#ifndef AT45_ERASE_WAIT
#define AT45_ERASE_WAIT         3000
#endif

#ifndef AT45_CHIP_ERASE_WAIT
#define AT45_CHIP_ERASE_WAIT    50000
#endif

#ifndef AT45_WRITE_POLLS
#define AT45_WRITE_POLLS        1000
#endif

//指令表
#define DFCMD_CONT_READ_LF      0x03
#define DFCMD_CONT_READ_HF      0x0B
#define DFCMD_BLOCK_ERASE       0x50
#define DFCMD_SECTOR_ERASE      0x7C
#define DFCMD_PAGE_ERASE        0x81
#define DFCMD_BUF1_PROG         0x82
#define DFCMD_BUF1_FLASH        0x83
#define DFCMD_BUF1_WRITE        0x84
#define DFCMD_BUF2_PROG         0x85
#define DFCMD_BUF2_FLASH        0x86
#define DFCMD_BUF2_WRITE        0x87
#define DFCMD_BUF1_FLASH_NE     0x88
#define DFCMD_BUF2_FLASH_NE     0x89
#define DFCMD_CHIP_ERASE        0xC7
#define DFCMD_BUF1_READ_LF      0xD1
#define DFCMD_READ_PAGE         0xD2
#define DFCMD_BUF2_READ_LF      0xD3
#define DFCMD_BUF1_READ         0xD4
#define DFCMD_BUF2_READ         0xD6
#define DFCMD_READ_STATUS       0xD7
#define DFCMD_CONT_READ         0xE8

void AT45DB081B_Init(void);
u16  AT45DB081B_ReadID(void);  	    		//读取FLASH ID
u8	 AT45DB081B_ReadSR(void);        		//读取状态寄存器 
void AT45DB081B_Write_SR(u8 sr);  			//写状态寄存器
void AT45DB081B_Write_Enable(void);  		//写使能 
void AT45DB081B_Write_Disable(void);		//写保护
void AT45DB081B_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void AT45DB081B_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //读取flash
void AT45DB081B_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//写入flash
void AT45DB081B_Erase_Chip(void);    	  	//整片擦除
void AT45DB081B_Erase_Sector(u32 Dst_Addr);	//扇区擦除
void AT45DB081B_Wait_Busy(void);           	//等待空闲
void AT45DB081B_PowerDown(void);        	//进入掉电模式
void AT45DB081B_WAKEUP(void);				//唤醒
#endif

