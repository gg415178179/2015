#include "FRAM.h" 
#include "delay.h" 				 
#include "FRAM_IIC2.h"
//��ʼ��IIC�ӿ�
void FRAM_Init(void)
{
	IIC2_Init();//IIC��ʼ��
}
//ָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
u8 FRAM_ReadOneByte(u16 ReadAddr)
{				  
u8 temp=0;		  	    																 
    IIC2_Start();  
	IIC2_Send_Byte(0XA2);	   //����д����
	IIC2_Wait_Ack();
	IIC2_Send_Byte(ReadAddr>>8);//���͸ߵ�ַ	    	   
	IIC2_Wait_Ack(); 
    IIC2_Send_Byte(ReadAddr%256);   //���͵͵�ַ
	IIC2_Wait_Ack();	    
	IIC2_Start();  	 	   
	IIC2_Send_Byte(0XA3);           //�������ģʽ			   
	IIC2_Wait_Ack();	 
    temp=IIC2_Read_Byte(0);		   
    IIC2_Stop();//����һ��ֹͣ����	    
	return temp;
}
//ָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void FRAM_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC2_Start();  
	IIC2_Send_Byte(0XA2);	    //����д����
	IIC2_Wait_Ack();
	IIC2_Send_Byte(WriteAddr>>8);//���͸ߵ�ַ	  
	IIC2_Wait_Ack();	   
    IIC2_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	IIC2_Wait_Ack(); 	 										  		   
	IIC2_Send_Byte(DataToWrite);     //�����ֽ�							   
	IIC2_Wait_Ack();  		    	   
    IIC2_Stop();//����һ��ֹͣ���� 
	delay_us(1);	 
}
//ָ����ַ��ʼд�볤��ΪLen������
//�ú�������д��16bit����32bit������.
//WriteAddr  :��ʼд��ĵ�ַ  
//DataToWrite:���������׵�ַ
//Len        :Ҫд�����ݵĳ���2,4
void FRAM_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		FRAM_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

//ָ����ַ��ʼ��������ΪLen������
//�ú������ڶ���16bit����32bit������.
//ReadAddr   :��ʼ�����ĵ�ַ 
//����ֵ     :����
//Len        :Ҫ�������ݵĳ���2,4
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

//���һ����ַ(32767)���洢��־��.
//���������24Cϵ��,�����ַҪ�޸�
//����1:���ʧ��
//����0:���ɹ�
u8 FRAM_Check(void)
{
	
	u8 temp;
	temp=FRAM_ReadOneByte(32767);//����ÿ�ο�����дAT24CXX			   
	if(temp==0X55)return 0;		   
	else//�ų���һ�γ�ʼ�������
	{
		FRAM_WriteOneByte(32767,0X55);
	    temp=FRAM_ReadOneByte(32767);	  
		
		if(temp==0X55)return 0;
	}
	return 1;	

}


//ReadAddr :��ʼ�����ĵ�ַ 
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
void FRAM_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=FRAM_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  

//WriteAddr :��ʼд��ĵ�ַ 
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
void FRAM_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		FRAM_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}







