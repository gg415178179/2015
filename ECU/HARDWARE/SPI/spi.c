//#include "spi.h"

///***********************************************
//*SPI��ʼ�������ó�Ϊ����ģʽ
//************************************************/
//void SPI1_Init(void)
//{
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	SPI_InitTypeDef  SPI_InitStructure;
//	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
//	
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
//	GPIO_Init(GPIOA,&GPIO_InitStructure);
//	
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1);
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1);
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1);
//	
//	//ֻ���SPI�ڳ�ʼ��
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//��λSPI1
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//ֹͣ��λSPI1
//	
//	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI����Ϊ˫��˫��ȫ˫��
//	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
//	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
//	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
//	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
//	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ�����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
//	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
//	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
//	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
//	SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
// 
//	SPI_Cmd(SPI1,ENABLE); //ʹ��SPI����
//	SPI1_ReadWriteByte(0xff);//��������	
//}

///***********************************************
//*SPI1�ٶ����ú���
//*SPI�ٶ�=fAPB2/��Ƶϵ�� 
//*fAPB2ʱ��һ��Ϊ84Mhz��
//************************************************/
//void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
//{
//	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��
//	SPI1->CR1&=0XFFC7;//λ3-5���㣬�������ò�����
//	SPI1->CR1|=SPI_BaudRatePrescaler;	//����SPI1�ٶ� 
//	SPI_Cmd(SPI1,ENABLE); //ʹ��SPI1	
//}

///***********************************************
//*SPI1 ��дһ���ֽ�
//*TxData:Ҫд����ֽ�
//*����ֵ:��ȡ�����ֽ�
//************************************************/
//u8 SPI1_ReadWriteByte(u8 TxData)
//{
//	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}//�ȴ���������  
//		SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ��byte  ����
//	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){} //�ȴ�������һ��byte  
//	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����	
//}


