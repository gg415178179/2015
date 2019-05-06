#ifndef __FRAM_H
#define __FRAM_H
#include "FRAM_IIC2.h"   


#define FM24C01		127
#define FM24C02		255
#define FM24C04		511
#define FM24C08		1023
#define FM24C16		2047
#define FM24C32		4095
#define FM24C64	    8191
#define FM24C128	16383
#define FM24C256	32767  


#define EE_TYPE FM24C256
					  
u8 FRAM_ReadOneByte(u16 ReadAddr);							//指定地址读取一个字节
void FRAM_WriteOneByte(u16 WriteAddr,u8 DataToWrite);		//指定地址写入一个字节
void FRAM_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len);//指定地址开始写入指定长度的数据
u32 FRAM_ReadLenByte(u16 ReadAddr,u8 Len);					//指定地址开始读取指定长度数据
void FRAM_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);	//从指定地址开始写入指定长度的数据
void FRAM_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   	//从指定地址开始读出指定长度的数据



u8 FRAM_Check(void);  //检查器件
void FRAM_Init(void); //初始化IIC
#endif
















