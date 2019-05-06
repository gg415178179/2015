#include "adc.h"
#include "delay.h"		 
														   

#define ADC1_DR_ADDRESS          ((uint32_t)0x4001204C)
__IO uint16_t uhADCConvertedValue[5];


/*********************************************************************************************************
*	�� �� ��: Adc_Init
*	����˵��: ADC��ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************/

void  Adc_Init(void)
{
	ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
	GPIO_InitTypeDef      GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOA| RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	DMA_InitStructure.DMA_Channel = DMA_Channel_0;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_ADDRESS;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&uhADCConvertedValue;;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 6;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);

	DMA_Cmd(DMA2_Stream0, ENABLE);
    
/****************************************************************************   
	  PCLK2 = HCLK / 2 
	  ����ѡ�����2��Ƶ
	  ADCCLK = PCLK2 /8 = HCLK / 8 = 168 / 8 = 21M
      ADC����Ƶ�ʣ� Sampling Time + Conversion Time = 480 + 12 cycles = 492cyc
                    Conversion Time = 21MHz / 492cyc = 42.6ksps. 
*****************************************************************************/
    
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 6;
    ADC_Init(ADC1, &ADC_InitStructure);
    
    ADC_DMACmd(ADC1, ENABLE);

   

    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_144Cycles);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 2, ADC_SampleTime_144Cycles);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 3, ADC_SampleTime_144Cycles);   
    ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 4, ADC_SampleTime_144Cycles);   
    ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 5, ADC_SampleTime_144Cycles);   
    ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 6, ADC_SampleTime_144Cycles);
    //ADC_VBATCmd(ENABLE); 
    
    ADC_TempSensorVrefintCmd(ENABLE); 

    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

    ADC_Cmd(ADC1, ENABLE);
    
    ADC_SoftwareStartConv(ADC1);
}				  



float GetTemp(uint16_t advalue)
{
    float Vtemp_sensor;
    float  Current_Temp;
    
    Vtemp_sensor = advalue * 3.3f/ 4095;  				           
    Current_Temp = (Vtemp_sensor - 0.76f)/0.0025f + 25;  //�¶�ת��
    
    return Current_Temp;
}

/*
#include "adc.h"
#include "delay.h"		 
														   
void  Adc_Init(void)
{    
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
	
	//MAD0 MAD1 MAD2���ų�ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);		 

	ADC_TempSensorVrefintCmd(ENABLE);//ʹ���ڲ��¶ȴ�����
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
	ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ɨ��ģʽ	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
	ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
	ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��

	
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_480Cycles);		//CPU�ڲ��¶ȴ�����
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_480Cycles);		//MAD0
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_480Cycles);		//MAD1
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_480Cycles);		//MAD2

	ADC_Cmd(ADC1, ENABLE);//����ADת����	
}				  

u16 Get_Adc(u8 ch)   
{
	 
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//480������		    
	ADC_SoftwareStartConv(ADC1);		//ʹ��ָ����ADC1�����ת����������	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}

//��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//ch:ͨ�����
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 


short Get_Temprate(void)
{
	u32 adcx;
	short result;
 	double temperate;
	adcx=Get_Adc(ADC_Channel_16);	
	temperate=(float)adcx*(2.5/4096);		//��ѹֵ
	temperate=(temperate-0.76)/0.0025 + 25; //ת��Ϊ�¶�ֵ 
	result=temperate*=100;					//����100��.
	return result;
}

*/










