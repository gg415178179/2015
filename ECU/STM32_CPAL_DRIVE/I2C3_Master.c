#include "I2C3_Master.h"
#include "sys.h"
extern  unsigned char r_data_buf[];  // buffer for loopack data
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;


RXBUF Rxbuf;
TXBUF Txbuf;

uint8_t IIC_RxBuffer[IICRecData_Len];
uint8_t IIC_TxBuffer[IICRecData_Len];

__IO uint32_t ActionState = ACTION_NONE;
__IO uint32_t TransmitMode = STATE_OFF;
__IO uint32_t DeviceMode = SLAVE;
__IO uint32_t LastMode = SLAVE;


CPAL_TransferTypeDef  sRxStructure, sTxStructure;

uint8_t Buffer_Check(uint8_t* pBuffer, uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
TestStatus Buffer_Compare(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
void SetWriteAddr(void);
void SetReadAddr(void);
void I2C2_Init(void);
void I2C2_Master_Service(void);
void CopySlaveData(void);


void I2C2_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(  RCC_AHB1Periph_GPIOB,ENABLE);
	
//��ʼ��GPIO�� IC2���ù���֮ǰ �Ƚ�GPIO����Ϊ��� ���ø�star
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_10);
	GPIO_SetBits(GPIOB,GPIO_Pin_11);
//��ʼ��GPIO�� IC2���ù���֮ǰ �Ƚ�GPIO����Ϊ��� ���ø�end
	
	
  sRxStructure.wNumData = IICRecData_Len;       
  sRxStructure.pbBuffer = IIC_RxBuffer;       
//  sRxStructure.wAddr1 = SLAVE_ADDRESS;       
//  sRxStructure.wAddr2 = 0;    

  sTxStructure.wNumData = IICSenData_Len;     
  sTxStructure.pbBuffer = (uint8_t*)&Txbuf;    
//  sTxStructure.wAddr1 = SLAVE_ADDRESS;       
//  sTxStructure.wAddr2 = 0;                 

  CPAL_I2C_StructInit(&I2C_DevStructure);     
  I2C_DevStructure.CPAL_Mode = CPAL_MODE_SLAVE;
#ifdef CPAL_I2C_DMA_PROGMODEL
  I2C_DevStructure.wCPAL_Options =  CPAL_OPT_NO_MEM_ADDR | CPAL_OPT_I2C_NACK_ADD;
  I2C_DevStructure.CPAL_ProgModel = CPAL_PROGMODEL_DMA;
#elif defined (CPAL_I2C_IT_PROGMODEL)
  I2C_DevStructure.wCPAL_Options =  CPAL_OPT_NO_MEM_ADDR | CPAL_OPT_I2C_NACK_ADD;
  I2C_DevStructure.CPAL_ProgModel = CPAL_PROGMODEL_INTERRUPT;
#else
#endif

  I2C_DevStructure.pCPAL_I2C_Struct->I2C_ClockSpeed = I2C_SPEED;
  I2C_DevStructure.pCPAL_I2C_Struct->I2C_OwnAddress1 = OWN_ADDRESS;
  I2C_DevStructure.pCPAL_TransferRx = &sRxStructure;
  I2C_DevStructure.pCPAL_TransferTx = &sTxStructure;
  CPAL_I2C_Init(&I2C_DevStructure);  
}

//����IIC����д��ַ
void SetWriteAddr(void)
{
	sRxStructure.wAddr1 = 0xb1;  //slave1��ַ
	sTxStructure.wAddr1 = 0xb1; 
	ActionState=SendToSlave_State;  
}


//����IIC��������ַ
void SetReadAddr(void)
{
	sRxStructure.wAddr1 = 0xb1;  //slave1��ַ
	sTxStructure.wAddr1 = 0xb1;    
	ActionState=ReadFromSlave_State;  
}


void I2C2_Master_Service(void)
{
	static unsigned char flag=1;
	if(((ActionState == ACTION_NONE) && (DeviceMode == MASTER)) || ((LastMode == SLAVE) && (DeviceMode == SLAVE)))
	{
		if(flag==1)
		{
			SetWriteAddr();
			flag=0;
		}
		else
		{
			SetReadAddr();
			flag=1;
		}
   }	
	//���action�Ƿ�������������
    if ((ActionState != ACTION_PENDING) && (ActionState != ACTION_NONE))
    {
      //�ж��豸�Ƿ�����д����
      if (((DeviceMode == SLAVE) && (LastMode == SLAVE))||
         (((I2C_DevStructure.CPAL_State == CPAL_STATE_READY) ||
          (I2C_DevStructure.CPAL_State == CPAL_STATE_DISABLED)) && (DeviceMode == MASTER)))
      {  
        if (LastMode == SLAVE)
        {    
			//ת��ΪMASTERʱ����ֹCPAL_OPT_I2C_NACK_ADD
          I2C_DevStructure.wCPAL_Options &= (~CPAL_OPT_I2C_NACK_ADD);
        }
     
        LastMode = MASTER;        
        sRxStructure.wNumData = IICRecData_Len;

		
        sTxStructure.wNumData = IICSenData_Len;
	
        switch (ActionState)
        {
          
         case SendToSlave_State:   //�������ݵ�Slave
          TransmitMode = STATE_ON;
          sTxStructure.pbBuffer = (uint8_t*)&Txbuf; 
          break;
          
          case ReadFromSlave_State:  //��Slave��ȡ����
          TransmitMode = STATE_OFF;          
          sRxStructure.pbBuffer = IIC_RxBuffer;        
          break;
		  
		  default:
          break;            
        } 
        
        I2C_DevStructure.CPAL_Mode = CPAL_MODE_MASTER;
        I2C_DevStructure.CPAL_State = CPAL_STATE_READY;
        
        ActionState = ACTION_PENDING; //�����һ��ACTIONδ���ʱ����ֹ����action���С�
        DeviceMode = MASTER;
       
        if(TransmitMode == STATE_ON)
        {
        
          if (CPAL_I2C_Write(&I2C_DevStructure) == CPAL_PASS)
          {
          }
        }
        else
        {
          
          if (CPAL_I2C_Read(&I2C_DevStructure) == CPAL_PASS)
          {
          }
        }
        
      }      
    }   
}



//����TCP������Buffer�ṹ��acae_data
void CopySlaveData(void)
{	
	int i;
	uint8_t* p_data = (uint8_t*)&Rxbuf;
	for(i=0;i<IICRecData_Len;i++)
	{
		p_data[i]=IIC_RxBuffer[i];
	}	
}
 

 
TestStatus Buffer_Compare(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }
    
    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;  
}


uint8_t Buffer_Check(uint8_t* pBuffer, uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{ 
  if (Buffer_Compare(pBuffer, pBuffer1, BufferLength) == PASSED)
  {
    return 0;
  }
  else if (Buffer_Compare(pBuffer, pBuffer2, BufferLength) == PASSED)
  {
    return 1;
  }
  else
  {
    return 0xFF;
  }
}


void assert_failed(uint8_t* file, uint32_t line)
{ 

  while (1)
  {
  }
}
