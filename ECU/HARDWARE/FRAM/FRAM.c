#include "FRAM.h" 
#include "delay.h" 				 
#include "FRAM_IIC2.h"
//初始化IIC接口
void FRAM_Init(void)
{
	IIC2_Init();//IIC初始化
}
//指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
u8 FRAM_ReadOneByte(u16 ReadAddr)
{				  
u8 temp=0;		  	    																 
    IIC2_Start();  
	IIC2_Send_Byte(0XA2);	   //发送写命令
	IIC2_Wait_Ack();
	IIC2_Send_Byte(ReadAddr>>8);//发送高地址	    	   
	IIC2_Wait_Ack(); 
    IIC2_Send_Byte(ReadAddr%256);   //发送低地址
	IIC2_Wait_Ack();	    
	IIC2_Start();  	 	   
	IIC2_Send_Byte(0XA3);           //进入接收模式			   
	IIC2_Wait_Ack();	 
    temp=IIC2_Read_Byte(0);		   
    IIC2_Stop();//产生一个停止条件	    
	return temp;
}
//指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void FRAM_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC2_Start();  
	IIC2_Send_Byte(0XA2);	    //发送写命令
	IIC2_Wait_Ack();
	IIC2_Send_Byte(WriteAddr>>8);//发送高地址	  
	IIC2_Wait_Ack();	   
    IIC2_Send_Byte(WriteAddr%256);   //发送低地址
	IIC2_Wait_Ack(); 	 										  		   
	IIC2_Send_Byte(DataToWrite);     //发送字节							   
	IIC2_Wait_Ack();  		    	   
    IIC2_Stop();//产生一个停止条件 
	delay_us(1);	 
}
//指定地址开始写入长度为Len的数据
//该函数用于写入16bit或者32bit的数据.
//WriteAddr  :开始写入的地址  
//DataToWrite:数据数组首地址
//Len        :要写入数据的长度2,4
void FRAM_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		FRAM_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

//指定地址开始读出长度为Len的数据
//该函数用于读出16bit或者32bit的数据.
//ReadAddr   :开始读出的地址 
//返回值     :数据
//Len        :要读出数据的长度2,4
u32 FRAM_ReadLenByte(u16 ReadAddr,u8 Len)
{  	
	u8 t;
	u32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=FRAM_ReadOneByte(ReadAddr+Len-t-1); 	 				   
	}
	return temp;												    
}

//最后一个地址(32767)来存储标志字.
//如果用其他24C系列,这个地址要修改
//返回1:检测失败
//返回0:检测成功
u8 FRAM_Check(void)
{
	
	u8 temp;
	temp=FRAM_ReadOneByte(32767);//避免每次开机都写AT24CXX			   
	if(temp==0X55)return 0;		   
	else//排除第一次初始化的情况
	{
		FRAM_WriteOneByte(32767,0X55);
	    temp=FRAM_ReadOneByte(32767);	  
		
		if(temp==0X55)return 0;
	}
	return 1;	

}


//ReadAddr :开始读出的地址 
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
void FRAM_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=FRAM_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  

//WriteAddr :开始写入的地址 
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
void FRAM_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		FRAM_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}







