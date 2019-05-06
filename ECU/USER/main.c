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

#define Fault_Led  PCout(11)   //输出高 灯亮
#define ACTIVE_Led  PCout(10) //输出高 灯亮
#define POWER_KEY  PAout(2)  //电源控制口 长按POWER按键3秒后启动



unsigned char ippp[4];//调试
unsigned char check_g;  //tiaoshi
extern uint8_t tRxBuffer[];  //从slave读取数据
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

	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);
	uart_init(1200);	//串口初始化波特率为115200
	LED_Init();
	POWER_ONOFF_KEY();
	Adc_Init();
	//FRAM_Init();
	//TIM3_Config(5000-1,8400-1);	//定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数5000次为500ms   
	I2C2_Init();
}

//复制从机上传的数据
void GetJdq_Statu(void)
{
	int i;
	for(i=0;i<IICRecData_Len;i++)
	{
	
	}
}

void reboot(void)
{
	//系统重启
	//__get_FAULTMASK();
	//NVIC_SystemReset();
}

void CHECH_SUM(void)
{

}


//复制TCP接受内存r_data_buf到结构体acae_data
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
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			printf("\r\n您发送的消息为:\r\n");
			for(t=0;t<len;t++)
			{
				USART_SendData(USART1, USART_RX_BUF[t]);         //向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
			}
			printf("\r\n\r\n");//插入换行
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
	 
			 
			//CPAL_I2C_DeInit(&I2C_DevStructure); //不然写EEPROM死机
			//write_IPconfig_to_eeprom();
			//reboot();
		 }
		 
		 
		// else if(acae_data.Code[0]==0x02)
		{
			
		}
		// else if(acae_data.Code[0]==0x03) 
		{
			 //I2C3_Master_Service(); 放这个位置上电无法读取到功能子板的信息，除非上位机CODE上电默认赋值0x03
		}
	//	 else if(acae_data.Code[0]==0x04)
		{
			  //通道复位
		}
		
		 Txbuf.command[0]=0x33;	//命令	
	     Txbuf.t_FV1_PWM[0]=0x44;
		 Txbuf.t_FV1_PWM[1]=0x55;

		 I2C2_Master_Service(); 
		 usart_callback();

	}
}




























