#include "stm32f4xx.h"
#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "main.h"
#include "usmart.h"
#include "rtc.h"
#include "adc.h"
#include "sram.h"
#include "timer.h"
#include "FRAM.h"
#include "FRAM_IIC2.h"
#include "GlobalVars.h"
#include "I2C3_Master.h" 

#define Fault_Led  PCout(11)   //����� ����
#define ACTIVE_Led  PCout(10) //����� ����
#define POWER_KEY  PAout(2)  //��Դ���ƿ� ����POWER����3�������



unsigned char ippp[4];//����
unsigned char check_g;  //tiaoshi
extern uint8_t tRxBuffer[];  //��slave��ȡ����
extern __IO uint16_t uhADCConvertedValue[];

void reboot(void);
void GetData(void);
void CHECH_SUM(void);


void LED_Init(void)
{
   	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	//FAULT ACTIVE 
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);	
	
	Fault_Led=0;
	ACTIVE_Led=1;
	
}

void POWER_ONOFF_KEY(void)
{
   	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	//FAULT ACTIVE 
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
	
	POWER_KEY=0;
	delay_ms(3000);
	POWER_KEY=1;
}


void Mysystem_Init(void)
{

	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);
	uart_init(1200);	//���ڳ�ʼ��������Ϊ115200
	LED_Init();
	POWER_ONOFF_KEY();
	Adc_Init();
	//FRAM_Init();
	//TIM3_Config(5000-1,8400-1);	//��ʱ��ʱ��84M����Ƶϵ��8400������84M/8400=10Khz�ļ���Ƶ�ʣ�����5000��Ϊ500ms   
	I2C2_Init();
}

//���ƴӻ��ϴ�������
void GetJdq_Statu(void)
{
	int i;
	for(i=0;i<IICRecData_Len;i++)
	{
	
	}
}

void reboot(void)
{
	//ϵͳ����
	//__get_FAULTMASK();
	//NVIC_SystemReset();
}

void CHECH_SUM(void)
{

}


//����TCP�����ڴ�r_data_buf���ṹ��acae_data
void GetData(void)
{	
//	int i;
//	uint8* pacae_data = (uint8*)&Txbuf;
//	for(i=0;i<SocketReveData_Len;i++)
//	{
//		pacae_data[i]=r_data_buf[i];
//	}	
}
	

void usart_callback(void)
{
	u8 len,t;
	
		if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			printf("\r\n�����͵���ϢΪ:\r\n");
			for(t=0;t<len;t++)
			{
				USART_SendData(USART1, USART_RX_BUF[t]);         //�򴮿�1��������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
			}
			printf("\r\n\r\n");//���뻻��
			USART_RX_STA=0;
		}else
		{
			
  
		}
	
	if(USART_RX_STA&0x8000) USART_RX_STA=0;
}

int main(void)
{

	
	Mysystem_Init();
	
	while(1)
	{	
		 
		//USART_Callback();
		delay_ms(100);
		//POWER_KEY=0;
		 //CHECH_SUM();
		// if(acae_data.Code[0]==0x01)
		 {
	 
			 
			//CPAL_I2C_DeInit(&I2C_DevStructure); //��ȻдEEPROM����
			//write_IPconfig_to_eeprom();
			//reboot();
		 }
		 
		 
		// else if(acae_data.Code[0]==0x02)
		{
			
		}
		// else if(acae_data.Code[0]==0x03) 
		{
			 //I2C3_Master_Service(); �����λ���ϵ��޷���ȡ�������Ӱ����Ϣ��������λ��CODE�ϵ�Ĭ�ϸ�ֵ0x03
		}
	//	 else if(acae_data.Code[0]==0x04)
		{
			  //ͨ����λ
		}
		
		 Txbuf.command[0]=0x33;	//����	
	     Txbuf.t_FV1_PWM[0]=0x44;
		 Txbuf.t_FV1_PWM[1]=0x55;

		 I2C2_Master_Service(); 
		 usart_callback();

	}
}




























