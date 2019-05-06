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
					  
u8 FRAM_ReadOneByte(u16 ReadAddr);							//ָ����ַ��ȡһ���ֽ�
void FRAM_WriteOneByte(u16 WriteAddr,u8 DataToWrite);		//ָ����ַд��һ���ֽ�
void FRAM_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len);//ָ����ַ��ʼд��ָ�����ȵ�����
u32 FRAM_ReadLenByte(u16 ReadAddr,u8 Len);					//ָ����ַ��ʼ��ȡָ����������
void FRAM_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);	//��ָ����ַ��ʼд��ָ�����ȵ�����
void FRAM_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   	//��ָ����ַ��ʼ����ָ�����ȵ�����



u8 FRAM_Check(void);  //�������
void FRAM_Init(void); //��ʼ��IIC
#endif
















