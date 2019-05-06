#ifndef _W5300_INIT_H
#define _W5300_INIT_H
#include "types.h"
#include "sram.h"
#include "sys.h"
#include "FRAM.h"
#include "stm32f4xx.h"

#define SocketReveData_Len  sizeof(acae_data)   //socket接收数据长度
#define SocketSendData_Len sizeof(ToPcBuf)                   //socket发送数据长度




#define SOCK_HTTPS            7

#define __PRINTF__ENABLE__			//获取MR寄存器值
#define W5300_ETHRST_HIGH  	GPIO_SetBits(GPIOA,GPIO_Pin_3)  	//硬件复位口
#define W5300_ETHRST_LOW  	GPIO_ResetBits(GPIOA,GPIO_Pin_3)    //硬件复位口   


#define IP_FROM_DEFINE	           	 			0                /*使用初始定义的IP信息*/
#define IP_FROM_DHCP	              			1                /*使用DHCP获取IP信息*/
#define IP_FROM_EEPROM	                  2						 /*使用EEPROM定义的IP信息*/

#define EEPROM_MSG_LEN        	sizeof(EEPROM_MSG)
	
extern unsigned char s_data_buf[];  // socket发送缓存数组
extern unsigned char r_data_buf[];  //socket接收缓存数组 有3个地方需要同时更改

extern uint8	ip_from;										 /*选择IP信息配置源*/

/*定义W5500可供配置的主要参数*/
typedef struct _CONFIG_MSG											
{
  uint8 mac[6];									/*MAC地址*/
  uint8 lip[4];								    /*local IP本地IP地址*/
  uint8 sub[4];									/*子网掩码*/
  uint8 gw[4];									/*网关*/	
  uint8 dns[4];									/*DNS服务器地址*/
  uint8 rip[4];									/*remote IP远程IP地址*/
  uint8 sw_ver[2];							    /*软件版本号*/

}CONFIG_MSG;

/*定义eeprom写入的几个变量，可修改*/
typedef struct _EEPROM_MSG	                    
{
  uint8 mac[6];									/*MAC地址*/
  uint8 lip[4];										/*local IP本地IP地址*/
  uint8 sub[4];										/*子网掩码*/
  uint8 gw[4];										/*网关*/
}EEPROM_MSG_STR;

/*定义结构体，接收上位机数据*/
typedef struct _ACAE_DATA
{
	uint8  Len[2]  ;     //数据长度
	uint8  Code[2] ;     //功能号
	uint8  ip[4];	     //设置IP  
	uint8  BoardNum[1];  //一个机箱板数量
	uint8  ReSetComd[1];  //相同参数下，再次发送，此命令取反，用于判断是否在执行上一次的设置
	uint8  Reserve1[2];
	uint8  SetBoardNum[2];		//注入板编号
	uint8  Reserve2[2];   //
	uint8  ChannelNum[2];  //总通道数  固定值6
	uint8  SetChannelNun[1];  //设置注入通道编号    
	uint8  Type[1];       //故障类型
	uint8  Res[2];        //串入阻值
	uint8  Frq[4];	      //串入频率
	uint8  SetTime[4];	  //故障时间
 	uint8  CheckSum[1];  
	
}ACAE_DATA ;

/*定义上传到上位机数据*/
typedef struct _TOPCBUF                 
{
	uint8 mac[6];		/*MAC地址*/
	uint8 lip[4];		/*local IP本地IP地址*/
	uint8 sub[4];		/*子网掩码*/
	uint8 gw[4];		/*网关*/
	uint8 sw_ver[2];	/*软件版本号*/
	uint8 statu1[25];   /*功能子板1继电器读取状态*///statu【16】包含：Type[1] /Res[2]/Frq[4]/SetTime[4]+每通道已执行的时间RunTime[4]
	uint8 statu2[25];   /*功能子板2继电器读取状态*///statu【16】包含：Type[1] /Res[2]/Frq[4]/SetTime[4]+每通道已执行的时间RunTime[4]
	uint8 statu3[25];   /*功能子板3继电器读取状态*///statu【16】包含：Type[1] /Res[2]/Frq[4]/SetTime[4]+每通道已执行的时间RunTime[4]
	uint8 statu4[25];   /*功能子板4继电器读取状态*///statu【16】包含：Type[1] /Res[2]/Frq[4]/SetTime[4]+每通道已执行的时间RunTime[4]
	uint8 statu5[25];   /*功能子板5继电器读取状态*///statu【16】包含：Type[1] /Res[2]/Frq[4]/SetTime[4]+每通道已执行的时间RunTime[4]
	uint8 statu6[25];   /*功能子板6继电器读取状态*///statu【16】包含：Type[1] /Res[2]/Frq[4]/SetTime[4]+每通道已执行的时间RunTime[4]
	uint8 reserve[4];   //备用
	uint8 CheckSum[1];  	
}TOPCBUF;

typedef struct _STU_DATA
{
	uint8  data[24]  ;   //数据长度

}STU_DATA ;

extern TOPCBUF ToPcBuf;
extern STU_DATA stu_data;
extern ACAE_DATA acae_data;
extern EEPROM_MSG_STR EEPROM_MSG;
extern CONFIG_MSG  	ConfigMsg;

void loopback_tcps(SOCKET s, uint16 port, uchar* S_buf, uchar* R_buf, uint16 mode);
void loopback_tcpc(SOCKET s, uint8* addr, uint16 port, uint8* buf,uint16 mode);
void loopback_udp(SOCKET s, uint16 port, uint8* buf, uint16 mode);

void w5300_init(void);			//W5300配置初始化
void W5300_GPIO_Init(void);		//W5300引脚初始化
void W5300_PHY_RESET(void);		//硬复位W5300

void w5300_ReadID(void);


void write_IPconfig_to_eeprom(void);											
void read_IPconfig_from_eeprom(void);												
void write_Stuconig_to_eeprom(void);
void read_Stuconig_to_eeprom(void);


#endif
