/**
  ******************************************************************************
  * @file    Project/STM32_CPAL_Examples/STM32_CPAL_I2C/Two_Boards_Listen_Mode/main..h 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    21-December-2012
  * @brief   Header file for main.c.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C3_H
#define __I2C3_H

/* Includes ------------------------------------------------------------------*/
#include "cpal_i2c.h"

#ifdef USE_STM322xG_EVAL 
 #include "stm32f2xx.h"
 #include "stm322xg_eval.h"
 #include "stm322xg_eval_lcd.h"
#elif defined USE_STM324xG_EVAL 
 #include "stm32f4xx.h"
 #include "stm324xg_eval.h"
 #include "stm324xg_eval_lcd.h"
#elif defined USE_STM3210C_EVAL 
 #include "stm32f10x.h"
 #include "stm3210c_eval.h"
 #include "stm3210c_eval_lcd.h"
#elif defined USE_STM32100E_EVAL
 #include "stm32f10x.h"
 #include "stm32100e_eval.h"
 #include "stm32100e_eval_lcd.h"
#elif defined USE_STM32L152_EVAL
 #include "stm32l1xx.h"
 #include "stm32l152_eval.h"
 #include "stm32l152_eval_lcd.h"
#elif defined USE_STM32L152D_EVAL
 #include "stm32l1xx.h"
 #include "stm32l152d_eval.h"
 #include "stm32l152d_eval_lcd.h"
#endif

#include <stdio.h>



#if defined (USE_STM32L152_EVAL) || defined (USE_STM32L152D_EVAL) || defined (USE_STM3210C_EVAL) || defined (USE_STM322xG_EVAL) || defined (USE_STM324xG_EVAL)
 #define I2C_DevStructure        I2C2_DevStructure
#endif

#ifdef USE_STM32100E_EVAL
 #define I2C_DevStructure        I2C2_DevStructure
#endif

#if defined (USE_STM32L152_EVAL) || defined (USE_STM32L152D_EVAL)
 #define ReceiveFromSlave_State 6
#endif

#define countof(a) (sizeof(a) / sizeof(*(a)))




#define OWN_ADDRESS             0x70 //I2C主机地址


#define I2C_SPEED               100000 

#define ACTION_NONE             0xFF
#define ACTION_DISABLED         0xFD
#define ACTION_PENDING          0xFE
#define ACTION_PERIODIC         0xFC

#define STATE_OFF               0
#define STATE_ON                1

#define MASTER                  0
#define SLAVE                   1

#define IICSenData_Len sizeof(Txbuf)  //iic主机发送给从机数据长度
#define IICRecData_Len sizeof(Rxbuf)    //iic主机从从机接收数据长度

	

typedef struct _RXBUF                 
{
	uint8_t NG[2];	
	uint8_t EGT[2];	
	uint8_t COOL[2];	
	uint8_t P0[2];		
	uint8_t START_TIME_SUM[4];	
	uint8_t WF_DEM[2];
	uint8_t STARTER_PWM[2];
	uint8_t IGNITOR_PWM[2];
	uint8_t FV1_PWM[2];	
	uint8_t FV2_PWM[2];		
	
	uint8_t CPU_Temperature[2];
	uint8_t uwVBAT_33V[2];
	uint8_t uwVCOOL[2];
	uint8_t uwVFIRE[2];
	uint8_t uwVSTARTER[2];
	uint8_t wVEGT[2]; //EGT差分端输入放大256倍
	uint8_t uwVBAT_7V[2];
	uint8_t uwVBAT_5V[2];
	uint8_t uwVFUEL_LOW[2];	
	
	uint8_t FAULT_WORD_1[2];
	uint8_t FAULT_WORD_2[2];
	uint8_t STATUS_WORD_1[2];
	uint8_t STATUS_WORD_2[2];
	uint8_t STATUS_WORD_3[2];
	
	uint8_t WF[2];
	uint8_t ENGINE_STATUS[1];
	uint8_t NGR[2];	
	uint8_t NG_ERR[2];		
	uint8_t NGDOT[2];	
	uint8_t IMODE[2];	

	uint8_t NVM_NG_FAULT; 	
	uint8_t NVM_T45_FAULT ;	
	uint8_t NVM_T1_FAULT;	
	uint8_t NVM_P0_FAULT;	
	uint8_t NVM_WF_FAULT;	
	uint8_t NVM_IGN_FAULT;	
	uint8_t NVM_GND_FAULT;	
	uint8_t NVM_CUR_ENG_TIMES[2];
	uint8_t NVM_MAX_NG[2];
	uint8_t NVM_MAX_NGR[2] ;
	uint8_t NVM_MAX_T45[2];
	uint8_t NVM_ENG_ST_TIMES[4];
	uint8_t NVM_ENGINE_TIMES[4];
	uint8_t NVM_EECU_PWR_TIMES[4];
	
	uint8_t CheckSum[2]; 
}RXBUF;

typedef struct _TXBUF                 
{
	uint8_t command[1];	//命令	
	uint8_t t_FV1_PWM[2];	//启动阀	测试设定值
	uint8_t t_FV2_PWM[2];		//主燃油阀测试设定值
	uint8_t t_START_WF[2];		//油泵测试设定值
	uint8_t t_STARTER_PWM[2];	 //起动机测试设定值
	uint8_t t_IGNITOR_PWM[2];  //点火器测试设定值
	
	//以下数据为初期调试用
	uint8_t IGN_S_PWM[10];		
	uint8_t ST_S_PWM[10];		
	uint8_t WF_S_PWM[10];	
    uint8_t Kp[4];	
	uint8_t Ki[4];
	uint8_t Kd[4];
	
	uint8_t sum[2];
}TXBUF;
extern TXBUF Txbuf;
extern RXBUF Rxbuf;

extern uint8_t IIC_RxBuffer[IICRecData_Len];
extern uint8_t IIC_TxBuffer[IICRecData_Len];

extern __IO uint32_t ActionState ;
extern __IO uint32_t TransmitMode ;
extern __IO uint32_t DeviceMode ;
extern __IO uint32_t LastMode;

extern uint8_t CH1_RxBuffer[];
extern uint8_t CH2_RxBuffer[];
extern uint8_t CH3_RxBuffer[];
extern uint8_t CH4_RxBuffer[];
extern uint8_t CH5_RxBuffer[];
extern uint8_t CH6_RxBuffer[];

extern uint8_t CH_RxBuffer[];

extern void I2C2_Init(void);
extern void I2C2_Master_Service(void);
extern void SetWriteAddr(void);
extern void SetReadAddr(void);
extern void CopySlaveData(void);

#endif 


