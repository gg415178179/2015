#ifndef _W5300_INIT_H
#define _W5300_INIT_H
#include "types.h"
#include "sram.h"
#include "sys.h"
#include "FRAM.h"
#include "stm32f4xx.h"

#define SocketReveData_Len  sizeof(acae_data)   //socket�������ݳ���
#define SocketSendData_Len sizeof(ToPcBuf)                   //socket�������ݳ���




#define SOCK_HTTPS            7

#define __PRINTF__ENABLE__			//��ȡMR�Ĵ���ֵ
#define W5300_ETHRST_HIGH  	GPIO_SetBits(GPIOA,GPIO_Pin_3)  	//Ӳ����λ��
#define W5300_ETHRST_LOW  	GPIO_ResetBits(GPIOA,GPIO_Pin_3)    //Ӳ����λ��   


#define IP_FROM_DEFINE	           	 			0                /*ʹ�ó�ʼ�����IP��Ϣ*/
#define IP_FROM_DHCP	              			1                /*ʹ��DHCP��ȡIP��Ϣ*/
#define IP_FROM_EEPROM	                  2						 /*ʹ��EEPROM�����IP��Ϣ*/

#define EEPROM_MSG_LEN        	sizeof(EEPROM_MSG)
	
extern unsigned char s_data_buf[];  // socket���ͻ�������
extern unsigned char r_data_buf[];  //socket���ջ������� ��3���ط���Ҫͬʱ����

extern uint8	ip_from;										 /*ѡ��IP��Ϣ����Դ*/

/*����W5500�ɹ����õ���Ҫ����*/
typedef struct _CONFIG_MSG											
{
  uint8 mac[6];									/*MAC��ַ*/
  uint8 lip[4];								    /*local IP����IP��ַ*/
  uint8 sub[4];									/*��������*/
  uint8 gw[4];									/*����*/	
  uint8 dns[4];									/*DNS��������ַ*/
  uint8 rip[4];									/*remote IPԶ��IP��ַ*/
  uint8 sw_ver[2];							    /*����汾��*/

}CONFIG_MSG;

/*����eepromд��ļ������������޸�*/
typedef struct _EEPROM_MSG	                    
{
  uint8 mac[6];									/*MAC��ַ*/
  uint8 lip[4];										/*local IP����IP��ַ*/
  uint8 sub[4];										/*��������*/
  uint8 gw[4];										/*����*/
}EEPROM_MSG_STR;

/*����ṹ�壬������λ������*/
typedef struct _ACAE_DATA
{
	uint8  Len[2]  ;     //���ݳ���
	uint8  Code[2] ;     //���ܺ�
	uint8  ip[4];	     //����IP  
	uint8  BoardNum[1];  //һ�����������
	uint8  ReSetComd[1];  //��ͬ�����£��ٴη��ͣ�������ȡ���������ж��Ƿ���ִ����һ�ε�����
	uint8  Reserve1[2];
	uint8  SetBoardNum[2];		//ע�����
	uint8  Reserve2[2];   //
	uint8  ChannelNum[2];  //��ͨ����  �̶�ֵ6
	uint8  SetChannelNun[1];  //����ע��ͨ�����    
	uint8  Type[1];       //��������
	uint8  Res[2];        //������ֵ
	uint8  Frq[4];	      //����Ƶ��
	uint8  SetTime[4];	  //����ʱ��
 	uint8  CheckSum[1];  
	
}ACAE_DATA ;

/*�����ϴ�����λ������*/
typedef struct _TOPCBUF                 
{
	uint8 mac[6];		/*MAC��ַ*/
	uint8 lip[4];		/*local IP����IP��ַ*/
	uint8 sub[4];		/*��������*/
	uint8 gw[4];		/*����*/
	uint8 sw_ver[2];	/*����汾��*/
	uint8 statu1[25];   /*�����Ӱ�1�̵�����ȡ״̬*///statu��16��������Type[1] /Res[2]/Frq[4]/SetTime[4]+ÿͨ����ִ�е�ʱ��RunTime[4]
	uint8 statu2[25];   /*�����Ӱ�2�̵�����ȡ״̬*///statu��16��������Type[1] /Res[2]/Frq[4]/SetTime[4]+ÿͨ����ִ�е�ʱ��RunTime[4]
	uint8 statu3[25];   /*�����Ӱ�3�̵�����ȡ״̬*///statu��16��������Type[1] /Res[2]/Frq[4]/SetTime[4]+ÿͨ����ִ�е�ʱ��RunTime[4]
	uint8 statu4[25];   /*�����Ӱ�4�̵�����ȡ״̬*///statu��16��������Type[1] /Res[2]/Frq[4]/SetTime[4]+ÿͨ����ִ�е�ʱ��RunTime[4]
	uint8 statu5[25];   /*�����Ӱ�5�̵�����ȡ״̬*///statu��16��������Type[1] /Res[2]/Frq[4]/SetTime[4]+ÿͨ����ִ�е�ʱ��RunTime[4]
	uint8 statu6[25];   /*�����Ӱ�6�̵�����ȡ״̬*///statu��16��������Type[1] /Res[2]/Frq[4]/SetTime[4]+ÿͨ����ִ�е�ʱ��RunTime[4]
	uint8 reserve[4];   //����
	uint8 CheckSum[1];  	
}TOPCBUF;

typedef struct _STU_DATA
{
	uint8  data[24]  ;   //���ݳ���

}STU_DATA ;

extern TOPCBUF ToPcBuf;
extern STU_DATA stu_data;
extern ACAE_DATA acae_data;
extern EEPROM_MSG_STR EEPROM_MSG;
extern CONFIG_MSG  	ConfigMsg;

void loopback_tcps(SOCKET s, uint16 port, uchar* S_buf, uchar* R_buf, uint16 mode);
void loopback_tcpc(SOCKET s, uint8* addr, uint16 port, uint8* buf,uint16 mode);
void loopback_udp(SOCKET s, uint16 port, uint8* buf, uint16 mode);

void w5300_init(void);			//W5300���ó�ʼ��
void W5300_GPIO_Init(void);		//W5300���ų�ʼ��
void W5300_PHY_RESET(void);		//Ӳ��λW5300

void w5300_ReadID(void);


void write_IPconfig_to_eeprom(void);											
void read_IPconfig_from_eeprom(void);												
void write_Stuconig_to_eeprom(void);
void read_Stuconig_to_eeprom(void);


#endif
