#include "w5300_init.h"
#include "w5300.h"
#include "socket.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"	
#include "sys.h"
#include "FRAM.h"


CONFIG_MSG  ConfigMsg;			//���ýṹ��
EEPROM_MSG_STR EEPROM_MSG;		//EEPROM�洢��Ϣ�ṹ��
ACAE_DATA acae_data;   //TCP���ջ���洢��Ϣ�ṹ��
STU_DATA stu_data;		//�̵�������������Ϣ�洢�ṹ��
TOPCBUF ToPcBuf; //�ϴ�����λ���ṹ��

unsigned char s_data_buf[SocketSendData_Len];  // socket���ͻ�������
unsigned char r_data_buf[SocketReveData_Len];  //socket���ջ������� ��3���ط���Ҫͬʱ����

uint8	ip_from=IP_FROM_EEPROM;	  //IP���÷���ѡ��


uint8 mac[6] = {0x00,0x08,0xDC,0x01,0x02,0x03};    // SHAR����Ӳ����ַ	

//����Ĭ��IP��Ϣ
uint8 ip[4] = {192,168,1,100};                   //���ñ���IP��ַ 
uint8 gw[4] = {192,168,1,1};                     // ��������IP   
uint8 sn[4] = {255,255,255,0};                   //������������ 
uint8 sw_ver[2]={1,0};							//�汾��
uint8 dns_server[4]={114,114,114,114};			//����W5300Ĭ��DNS


uint8 tx_mem_conf[8] = {8,8,8,8,8,8,8,8};          //TX�洢�����ƼĴ�������     
uint8 rx_mem_conf[8] = {8,8,8,8,8,8,8,8};         //RX�洢�����ƼĴ������� 

void w5300_init(void)
{	

	W5300_GPIO_Init();		//W5300�������ų�ʼ��	
	W5300_PHY_RESET();	    //Ӳ����λ
    FSMC_SRAM_Init();		//��ʼ��FSMC����
	iinchip_init();	       //�����λ �����������߿�ȣ������ӿ�ģʽ
	
	//���ƶ����������Ϣ�����ýṹ��
	memcpy(ConfigMsg.mac,mac, 6);
	memcpy(ConfigMsg.lip,ip,4);
	memcpy(ConfigMsg.sub,sn,4);
	memcpy(ConfigMsg.gw, gw,4);
	memcpy(ConfigMsg.dns,dns_server,4);
	memcpy(ConfigMsg.sw_ver,sw_ver,4);
	

	
	setSHAR(mac);  //���ñ���Ӳ����ַ
	
	//����W5300�ڲ����ա����ʹ洢����С
	if(!sysinit(tx_mem_conf,rx_mem_conf))           
	{
      printf("MEMORY CONFIG ERR.\r\n");
      while(1);
	}
	
	
	if(ip_from == IP_FROM_DEFINE)
	{
		write_IPconfig_to_eeprom();	
	}
	if(ip_from == IP_FROM_EEPROM)
	{
		read_IPconfig_from_eeprom();	
		
		//�����ȡEEPROM��MAC��Ϣ,��������ã����ʹ��	
		if( *(EEPROM_MSG.mac)==0x00&& *(EEPROM_MSG.mac+1)==0x08&&*(EEPROM_MSG.mac+2)==0xdc)		
		{
			//����EEPROM������Ϣ�����õĽṹ�����
			memcpy(ConfigMsg.lip,EEPROM_MSG.lip, 4);				
			memcpy(ConfigMsg.sub,EEPROM_MSG.sub, 4);
			memcpy(ConfigMsg.gw, EEPROM_MSG.gw, 4);
			
			//IP��ʼ����ֵ��������λ���ṹ��
			memcpy(ToPcBuf.mac, mac, 6); 
			memcpy(ToPcBuf.lip, EEPROM_MSG.lip, 4);
			memcpy(ToPcBuf.sub, EEPROM_MSG.sub, 4);
			memcpy(ToPcBuf.gw, EEPROM_MSG.gw, 4);
			memcpy(ToPcBuf.sw_ver, sw_ver, 2);
		}
		else
		{
			write_IPconfig_to_eeprom();	//ʹ��Ĭ�ϵ�IP��Ϣ������ʼ��EEPROM������
		}
		
	}
	
	
	setSUBR(ConfigMsg.sub);
	setGAR(ConfigMsg.gw);
	setSIPR(ConfigMsg.lip);
                                  
	getSHAR(mac);   //��ȡ����Ӳ����ַ                                  
	getGAR(gw);     //��ȡ����IP                                      
	getSUBR(sn);    //��ȡ��������                                     
	getSIPR(ip);    //��ȡ����IP��ַ 
	
}


//TCP SERVER
void loopback_tcps(SOCKET s, uint16 port, uchar* S_buf, uchar* R_buf, uint16 mode)
{
	uint32 len;  
	switch(getSn_SSR(s))                
	{                                   
		case SOCK_ESTABLISHED:          
				if(getSn_IR(s) & Sn_IR_CON)   //���Sn_IR 0λ(CON)���ж��Ƿ���Զ˳ɹ���������
				{
					//printf("%d : Connect OK\r\n",s);
					setSn_IR(s,Sn_IR_CON);     // ���CON�жϱ�־

					ka_tick_flag=0;										// KA��ʱ����ʼ��ʱ��־λ����
					ka_no_data_tick=0;								// �����ݴ���ʱ���ʱ��
					ka_send_flag=0;										// KA���ͱ�־λ����
					ka_send_tick=0;										// KA���Ͷ�ʱ������
				}
				if((len=getSn_RX_RSR(s)) > 0) //����Ƿ���ܵ��Զ˷����������ݳ��ȣ�len���л�ΪSn_RX_FIFOR
				{
					len = recv(s,R_buf,len);     // recv
					//if(len !=send(s,buf,len))  
					{
					//  printf("%d : Send Fail.len=%d\r\n",s,len);
					}
					
					//send(s,S_buf,50);  // send �����ã������ɾ��

					if(ka_tick_flag==0)				
					{
						ka_tick_flag=1;									// W5300ͬ�ͻ��˽�����һ��ͨ�ź󣬽�KA��ʱ����ʼ��ʱ��־λ��1�����붨ʱ���жϣ�ֻҪ��������NO_DATA_PERIOD��û������ͨ�ţ��Ϳ�ʼ��KA��
					}
					ka_no_data_tick=0;								// �����ݴ���ʱ���ʱ������
					ka_send_tick=0;										// KA���Ͷ�ʱ������	
				}
				
				send(s,S_buf,SocketSendData_Len);  // send �����ã������ɾ�� // send �����ã������ɾ��
				
				if(ka_send_flag)
				{
					ka_send_flag=0;										// KA���ͱ�־λ����
					ka_send_tick=0;										// KA���Ͷ�ʱ������
					send_keepalive(s);								// W5300��KA�����ͻ���
					//printf("*");
				}
				break;

		case SOCK_CLOSE_WAIT:               // �յ��Զ˶Ͽ���������
				if ((len = getSn_RX_RSR(0)) > 0)		// �����ʱ��������
				{	
					len=recv(s,R_buf,len);     // recv		
					send(s,S_buf,len);  // send	
				}

				disconnect(s);                // disconnect 
               // close(s);                        // close  SOCKET
				ka_tick_flag=0;											// KA��ʱ����ʼ��ʱ��־λ����
				ka_send_flag=0;											// KA���ͱ�־λ����
				ka_no_data_tick=0;									// �����ݴ���ʱ���ʱ������
				ka_send_tick=0;											// KA���Ͷ�ʱ������			
				break;

		case SOCK_CLOSED:                   // SOCKETn�˿���Դ�ͷ�״̬
				close(s);                        // close  SOCKET
				socket(s,Sn_MR_TCP,port,mode);   // open  SOCKET  
				break;

		case SOCK_INIT:                     //SOCKETn��TCPģʽ��ʱ��״̬������TCP���ӵ�һ��
				listen(s);                       //TCP CLIENT
				ka_tick_flag=0;											// KA��ʱ����ʼ��ʱ��־λ����
				ka_send_flag=0;											// KA���ͱ�־λ����
				ka_no_data_tick=0;									// �����ݴ���ʱ���ʱ������
				//printf("%d : LOOPBACK_TCPS(%d) Started.\r\n",s,port);
				break;
		default:
			    break;
      }
}


//TCP CLIENTģʽ
void loopback_tcpc(SOCKET s,uint8* addr,uint16 port,uint8* buf,uint16 mode)
{
	uint32 len;
	static uint16 any_port = 1000;

	switch(getSn_SSR(s))                   
	{                                     
		case SOCK_ESTABLISHED:              // ESTABLISHED?
				if(getSn_IR(s) & Sn_IR_CON)      // check Sn_IR_CON bit
				{
					//printf("%d : Connect OK\r\n",s);
					setSn_IR(s,Sn_IR_CON);        // clear Sn_IR_CON
				}
				if((len=getSn_RX_RSR(s)) > 0)    // check the size of received data
				{
					len = recv(s,buf,len);        // recv
					if(len !=send(s,buf,len))     // send
					{
						//printf("%d : Send Fail.len=%d\r\n",s,len);
					}
				}
				break;
			
		case SOCK_CLOSE_WAIT:                  // PASSIVE CLOSED
				disconnect(s);                   // disconnect 
				break;

		case SOCK_CLOSED:                      // CLOSED
				close(s);                           // close the SOCKET
				socket(s,Sn_MR_TCP,any_port++,mode);// open SOCKEt
				break;

		case SOCK_INIT:                        // The SOCKET opened with TCP mode
				connect(s, addr, port);             // connect�������ÿͻ���ģʽ
				//printf("%d : LOOPBACK_TCPC(%d.%d.%d.%d:%d) Started.\r\n",s,addr[0],addr[1],addr[2],addr[3],port);
				break;
		default:
				break;
	}
}

//UDPģʽ
void loopback_udp(SOCKET s, uint16 port, uint8* buf, uint16 mode)
{
	uint32 len;
	uint8 destip[4];
	uint16 destport;

	switch(getSn_SSR(s))
	{
			
		case SOCK_UDP:                                  
				if((len=getSn_RX_RSR(s)) > 0)                  
				{
					len = recvfrom(s,buf,len,destip,&destport);  
					if(len !=sendto(s,buf,len,destip,destport))  
					{
						printf("%d : Sendto Fail.len=%d,",s,len);
						printf("%d.%d.%d.%d(%d)\r\n",destip[0],destip[1],destip[2],destip[3],destport);
					}
				}
				break;
		
		case SOCK_CLOSED:                                  
				close(s);                                       
				socket(s,Sn_MR_UDP,port,mode);                  
				break;
		default:
				break;
	}
}


void W5300_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOE, ENABLE);
	//ETHBRDY0 ETHBRDY1 ETHBRDY2 ETHBRDY3  ETHLINK���ų�ʼ�� 
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	//ETHRST��ʼ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd= GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//ETHINT���ų�ʼ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	//ESPD EFDX ECOLL ERX ETX���ų�ʼ��
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
}


//W5300Ӳ����λ
void W5300_PHY_RESET(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);                                         
	delay_us(10); //��Ҫ����ʱ����С2us
	GPIO_SetBits(GPIOA,GPIO_Pin_3);
	delay_ms(10);//��Ҫ����ʱ����С50us�����10ms
}


void w5300_ReadID(void)
{
	printf("W5300 ID is %04x\r\n",*((vuint16*)W5300_IDR));//��ȡW5300��
}

//��FRAM��ȡ����IP��Ϣ
void read_IPconfig_from_eeprom(void)
{
	u16 addr=0;
	FRAM_Read(addr,EEPROM_MSG.mac,(u16)EEPROM_MSG_LEN);
	delay_ms(10);
}

//��FRAMд�����õ�IP��Ϣ
void write_IPconfig_to_eeprom(void)
{
	u16 addr=0;
	FRAM_Write(addr,ConfigMsg.mac,(u16)EEPROM_MSG_LEN);
	delay_ms(10);
}
//��FRAMд�뱣��ļ̵���������Ϣ
void write_Stuconig_to_eeprom(void)
{
	//u16 addr=500;
	//FRAM_Write(addr,acae_data.SetStatu,24);
}
//��FRAM��ȡ���õļ̵�����Ϣ������CAT9555��ʼ��
void read_Stuconig_to_eeprom(void)
{
	//u16 addr=500;
	//FRAM_Read(addr,stu_data.data,24);
}

