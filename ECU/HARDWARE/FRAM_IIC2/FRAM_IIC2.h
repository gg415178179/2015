#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h" 

   	   		   
//IO��������
#define SDA2_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PB11����ģʽ
#define SDA2_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //PB11���ģʽ
//IO��������	 
#define IIC2_SCL    PBout(8) //SCL
#define IIC2_SDA    PBout(9) //SDA	 
#define READ2_SDA   PBin(9)  //����SDA 


//IIC���в�������
void IIC2_Init(void);                //��ʼ��IIC��IO��				 
void IIC2_Start(void);				//����IIC��ʼ�ź�
void IIC2_Stop(void);	  			//����IICֹͣ�ź�
void IIC2_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC2_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC2_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC2_Ack(void);					//IIC����ACK�ź�
void IIC2_NAck(void);				//IIC������ACK�ź� 
#endif
















