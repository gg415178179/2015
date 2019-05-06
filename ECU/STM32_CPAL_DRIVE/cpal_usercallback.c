
/* Includes ------------------------------------------------------------------*/
#include "I2C3_Master.h"
int TEST_CHECK;
extern const uint8_t tStateSignal[];
extern const uint8_t tSignal[];
extern CPAL_TransferTypeDef  sRxStructure, sTxStructure;
extern uint8_t tRxBuffer[];
extern uint32_t BufferSize;
extern __IO uint32_t ActionState;
extern __IO uint32_t LastMode;
extern __IO uint32_t DeviceMode;

extern I2C_TypeDef* CPAL_I2C_DEVICE[];

extern uint8_t Buffer_Check(uint8_t* pBuffer, uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);



uint32_t CPAL_TIMEOUT_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{
  I2C_DevStructure.CPAL_State = CPAL_STATE_READY;
  I2C_DevStructure.wCPAL_DevError = CPAL_I2C_ERR_NONE ;        
  I2C_DevStructure.wCPAL_Timeout  = CPAL_I2C_TIMEOUT_DEFAULT;  
 
  CPAL_I2C_DeInit(&I2C_DevStructure);  
  
  CPAL_I2C_Init(pDevInitStruct);    
  
  ActionState = ACTION_NONE; 
  
  return CPAL_PASS;  
}


/*=========== Error UserCallback ===========*/

void CPAL_I2C_ERR_UserCallback(CPAL_DevTypeDef pDevInstance, uint32_t DeviceError)
{
  if (I2C_DevStructure.wCPAL_DevError == CPAL_I2C_ERR_AF )
  {
    //Slave Not yet Ready
  } 
  else
  {
    //Device Err occurred 
  }
  
  I2C_DevStructure.CPAL_State = CPAL_STATE_READY;
  I2C_DevStructure.wCPAL_DevError = CPAL_I2C_ERR_NONE ;        
  
  CPAL_I2C_DeInit(&I2C_DevStructure);
  
  CPAL_I2C_Init(&I2C_DevStructure);    
  
  ActionState = ACTION_NONE;
}


void CPAL_I2C_TXTC_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{

  ActionState  = ACTION_NONE;
  LastMode = MASTER;
}
 
void CPAL_I2C_RXTC_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
{
	int i=0;
	uint16_t checksum=0;

	//for(i=0;i<(IICRecData_Len-2);i++)
	{
	//	checksum += IIC_RxBuffer[i];
	}

	//if(((uint8_t)checksum == IIC_RxBuffer[IICRecData_Len-2])&&((uint8_t)(checksum>>8))== IIC_RxBuffer[IICRecData_Len-1])
	{
		CopySlaveData();	
	}
	//else
	{
		
	}
	
  ActionState  = ACTION_NONE;
  LastMode = MASTER;
}




