#include "w5300_init.h"
#include "w5300.h"
#include "socket.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"	
#include "sys.h"
#include "FRAM.h"


CONFIG_MSG  ConfigMsg;			//配置结构体
EEPROM_MSG_STR EEPROM_MSG;		//EEPROM存储信息结构体
ACAE_DATA acae_data;   //TCP接收缓存存储信息结构体
STU_DATA stu_data;		//继电器保存配置信息存储结构体
TOPCBUF ToPcBuf; //上传给上位机结构体

unsigned char s_data_buf[SocketSendData_Len];  // socket发送缓存数组
unsigned char r_data_buf[SocketReveData_Len];  //socket接收缓存数组 有3个地方需要同时更改

uint8	ip_from=IP_FROM_EEPROM;	  //IP配置方法选择


uint8 mac[6] = {0x00,0x08,0xDC,0x01,0x02,0x03};    // SHAR本机硬件地址	

//定义默认IP信息
uint8 ip[4] = {192,168,1,100};                   //设置本机IP地址 
uint8 gw[4] = {192,168,1,1};                     // 设置网关IP   
uint8 sn[4] = {255,255,255,0};                   //设置子网掩码 
uint8 sw_ver[2]={1,0};							//版本号
uint8 dns_server[4]={114,114,114,114};			//定义W5300默认DNS


uint8 tx_mem_conf[8] = {8,8,8,8,8,8,8,8};          //TX存储器配制寄存器设置     
uint8 rx_mem_conf[8] = {8,8,8,8,8,8,8,8};         //RX存储器配制寄存器设置 

void w5300_init(void)
{	

	W5300_GPIO_Init();		//W5300部分引脚初始化	
	W5300_PHY_RESET();	    //硬件复位
    FSMC_SRAM_Init();		//初始化FSMC总线
	iinchip_init();	       //软件复位 设置数据总线宽度，主机接口模式
	
	//复制定义的配置信息到配置结构体
	memcpy(ConfigMsg.mac,mac, 6);
	memcpy(ConfigMsg.lip,ip,4);
	memcpy(ConfigMsg.sub,sn,4);
	memcpy(ConfigMsg.gw, gw,4);
	memcpy(ConfigMsg.dns,dns_server,4);
	memcpy(ConfigMsg.sw_ver,sw_ver,4);
	

	
	setSHAR(mac);  //设置本机硬件地址
	
	//配置W5300内部接收、发送存储器大小
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
		
		//如果读取EEPROM中MAC信息,如果已配置，则可使用	
		if( *(EEPROM_MSG.mac)==0x00&& *(EEPROM_MSG.mac+1)==0x08&&*(EEPROM_MSG.mac+2)==0xdc)		
		{
			//复制EEPROM配置信息到配置的结构体变量
			memcpy(ConfigMsg.lip,EEPROM_MSG.lip, 4);				
			memcpy(ConfigMsg.sub,EEPROM_MSG.sub, 4);
			memcpy(ConfigMsg.gw, EEPROM_MSG.gw, 4);
			
			//IP初始化后赋值给发送上位机结构体
			memcpy(ToPcBuf.mac, mac, 6); 
			memcpy(ToPcBuf.lip, EEPROM_MSG.lip, 4);
			memcpy(ToPcBuf.sub, EEPROM_MSG.sub, 4);
			memcpy(ToPcBuf.gw, EEPROM_MSG.gw, 4);
			memcpy(ToPcBuf.sw_ver, sw_ver, 2);
		}
		else
		{
			write_IPconfig_to_eeprom();	//使用默认的IP信息，并初始化EEPROM中数据
		}
		
	}
	
	
	setSUBR(ConfigMsg.sub);
	setGAR(ConfigMsg.gw);
	setSIPR(ConfigMsg.lip);
                                  
	getSHAR(mac);   //读取本机硬件地址                                  
	getGAR(gw);     //读取网关IP                                      
	getSUBR(sn);    //读取子网掩码                                     
	getSIPR(ip);    //读取本机IP地址 
	
}


//TCP SERVER
void loopback_tcps(SOCKET s, uint16 port, uchar* S_buf, uchar* R_buf, uint16 mode)
{
	uint32 len;  
	switch(getSn_SSR(s))                
	{                                   
		case SOCK_ESTABLISHED:          
				if(getSn_IR(s) & Sn_IR_CON)   //检测Sn_IR 0位(CON)，判断是否与对端成功建立连接
				{
					//printf("%d : Connect OK\r\n",s);
					setSn_IR(s,Sn_IR_CON);     // 清除CON中断标志

					ka_tick_flag=0;										// KA定时器开始计时标志位清零
					ka_no_data_tick=0;								// 无数据传输时间计时器
					ka_send_flag=0;										// KA发送标志位清零
					ka_send_tick=0;										// KA发送定时器清零
				}
				if((len=getSn_RX_RSR(s)) > 0) //检测是否接受到对端发送来的数据长度，len可切换为Sn_RX_FIFOR
				{
					len = recv(s,R_buf,len);     // recv
					//if(len !=send(s,buf,len))  
					{
					//  printf("%d : Send Fail.len=%d\r\n",s,len);
					}
					
					//send(s,S_buf,50);  // send 调试用，用完后删除

					if(ka_tick_flag==0)				
					{
						ka_tick_flag=1;									// W5300同客户端进行了一次通信后，将KA定时器开始计时标志位置1，进入定时器中断，只要接下来在NO_DATA_PERIOD内没有数据通信，就开始发KA包
					}
					ka_no_data_tick=0;								// 无数据传输时间计时器清零
					ka_send_tick=0;										// KA发送定时器清零	
				}
				
				send(s,S_buf,SocketSendData_Len);  // send 调试用，用完后删除 // send 调试用，用完后删除
				
				if(ka_send_flag)
				{
					ka_send_flag=0;										// KA发送标志位清零
					ka_send_tick=0;										// KA发送定时器清零
					send_keepalive(s);								// W5300发KA包给客户端
					//printf("*");
				}
				break;

		case SOCK_CLOSE_WAIT:               // 收到对端断开连接请求
				if ((len = getSn_RX_RSR(0)) > 0)		// 如果此时仍有数据
				{	
					len=recv(s,R_buf,len);     // recv		
					send(s,S_buf,len);  // send	
				}

				disconnect(s);                // disconnect 
               // close(s);                        // close  SOCKET
				ka_tick_flag=0;											// KA定时器开始计时标志位清零
				ka_send_flag=0;											// KA发送标志位清零
				ka_no_data_tick=0;									// 无数据传输时间计时器清零
				ka_send_tick=0;											// KA发送定时器清零			
				break;

		case SOCK_CLOSED:                   // SOCKETn端口资源释放状态
				close(s);                        // close  SOCKET
				socket(s,Sn_MR_TCP,port,mode);   // open  SOCKET  
				break;

		case SOCK_INIT:                     //SOCKETn以TCP模式打开时的状态，建立TCP连接第一步
				listen(s);                       //TCP CLIENT
				ka_tick_flag=0;											// KA定时器开始计时标志位清零
				ka_send_flag=0;											// KA发送标志位清零
				ka_no_data_tick=0;									// 无数据传输时间计时器清零
				//printf("%d : LOOPBACK_TCPS(%d) Started.\r\n",s,port);
				break;
		default:
			    break;
      }
}


//TCP CLIENT模式
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
				connect(s, addr, port);             // connect命令设置客户端模式
				//printf("%d : LOOPBACK_TCPC(%d.%d.%d.%d:%d) Started.\r\n",s,addr[0],addr[1],addr[2],addr[3],port);
				break;
		default:
				break;
	}
}

//UDP模式
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
	//ETHBRDY0 ETHBRDY1 ETHBRDY2 ETHBRDY3  ETHLINK引脚初始化 
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	//ETHRST初始化
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd= GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//ETHINT引脚初始化
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	//ESPD EFDX ECOLL ERX ETX引脚初始化
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
}


//W5300硬件复位
void W5300_PHY_RESET(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);                                         
	delay_us(10); //必要的延时，最小2us
	GPIO_SetBits(GPIOA,GPIO_Pin_3);
	delay_ms(10);//必要的延时，最小50us，最大10ms
}


void w5300_ReadID(void)
{
	printf("W5300 ID is %04x\r\n",*((vuint16*)W5300_IDR));//获取W5300标
}

//从FRAM读取配置IP信息
void read_IPconfig_from_eeprom(void)
{
	u16 addr=0;
	FRAM_Read(addr,EEPROM_MSG.mac,(u16)EEPROM_MSG_LEN);
	delay_ms(10);
}

//往FRAM写入设置的IP信息
void write_IPconfig_to_eeprom(void)
{
	u16 addr=0;
	FRAM_Write(addr,ConfigMsg.mac,(u16)EEPROM_MSG_LEN);
	delay_ms(10);
}
//往FRAM写入保存的继电器配置信息
void write_Stuconig_to_eeprom(void)
{
	//u16 addr=500;
	//FRAM_Write(addr,acae_data.SetStatu,24);
}
//从FRAM读取配置的继电器信息，用于CAT9555初始化
void read_Stuconig_to_eeprom(void)
{
	//u16 addr=500;
	//FRAM_Read(addr,stu_data.data,24);
}

