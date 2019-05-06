#include "w5300.h"
#include "delay.h"
/********************************************
*IO操作
*********************************************/
static uint8 SUBN_VAR[4]; 	
static uint8 IP_VAR[4];

uint8 SOCK_INT[MAX_SOCK_NUM];	//SOCKET中断变量

uint32 TXMEM_SIZE[MAX_SOCK_NUM];	//TX
uint32 RXMEM_SIZE[MAX_SOCK_NUM];	//RX

uint16 IINCHIP_READ(uint32 addr)
{
	addr=((addr&0xFFFF0000)|((addr&0xFFFF)<<1));
	return (*((vuint16*)addr));
	//      *((uint16*)(buf+idx))); 
}

void IINCHIP_WRITE(uint32 addr,uint16 data)
{
	addr=((addr&0xFFFF0000)|((addr&0xFFFF)<<1));;
	(*((vuint16*)addr))=data;
}

//MR模式寄存器
uint16 getMR(void)
{
	//return *((vint16*)W5300_MR);
	return IINCHIP_READ(W5300_MR);
}
void setMR(uint16 val)
{
	//return *((vint16*)W5300_MR);
	IINCHIP_WRITE(W5300_MR,val);
}

//IR中断寄存器
uint16 getIR(void)
{
	return IINCHIP_READ(W5300_IR);
}
void setIR(uint16 val)
{
	IINCHIP_WRITE(W5300_IR,val);
}

//IMR中断屏蔽寄存器 page61
uint16 getIMR(void)
{
	return IINCHIP_READ(W5300_IMR);
}
void setIMR(uint16 mask)
{
	IINCHIP_WRITE(W5300_IMR,mask);
}

//硬件地址寄存器
void getSHAR(uint8* addr)
{
	addr[0]=(uint8)(IINCHIP_READ(W5300_SHAR)>>8);
	addr[1]=(uint8)IINCHIP_READ(W5300_SHAR);
	addr[2]=(uint8)(IINCHIP_READ(W5300_SHAR2)>>8);
	addr[3]=(uint8)IINCHIP_READ(W5300_SHAR2);
	addr[4]=(uint8)(IINCHIP_READ(W5300_SHAR4)>>8);
	addr[5]=(uint8)IINCHIP_READ(W5300_SHAR4);
}
void setSHAR(uint8* addr)
{
	
	//IINCHIP_WRITE(W5300_SHAR, (((uint16)addr[0])<<8)+addr[1]);
	//IINCHIP_WRITE(W5300_SHAR2,(((uint16)addr[2])<<8)+addr[3]); 
	//IINCHIP_WRITE(W5300_SHAR4,(((uint16)addr[4])<<8)+addr[5]);   
	
   IINCHIP_WRITE(W5300_SHAR, ((uint16)addr[0]<<8)+addr[1]);
   IINCHIP_WRITE(W5300_SHAR2,((uint16)addr[2]<<8)+addr[3]);
   IINCHIP_WRITE(W5300_SHAR4,((uint16)addr[4]<<8)+addr[5]);	
}

//网关IP地址寄存器
void getGAR(uint8* addr)
{
	addr[0]=(uint8)(IINCHIP_READ(W5300_GAR)>>8);
	addr[1]=(uint8)(IINCHIP_READ(W5300_GAR));
	addr[2]=(uint8)(IINCHIP_READ(W5300_GAR2)>>8);
	addr[3]=(uint8)(IINCHIP_READ(W5300_GAR2));
}
void setGAR(uint8* addr)
{
	IINCHIP_WRITE(W5300_GAR, ((uint16)addr[0]<<8)+(uint16)addr[1]);
	IINCHIP_WRITE(W5300_GAR2,((uint16)addr[2]<<8)+(uint16)addr[3]);
}


//子网掩码寄存器
void  getSUBR(uint8* addr)
{
	addr[0] = (uint8)SUBN_VAR[0];
	addr[1] = (uint8)SUBN_VAR[1];
	addr[2] = (uint8)SUBN_VAR[2];
	addr[3] = (uint8)SUBN_VAR[3];
}
void  setSUBR(uint8* addr)
{
	SUBN_VAR[0] = addr[0];
	SUBN_VAR[1] = addr[1];
	SUBN_VAR[2] = addr[2];
	SUBN_VAR[3] = addr[3];
	applySUBR();
}
void applySUBR(void)
{
	IINCHIP_WRITE(W5300_SUBR, ((uint16)SUBN_VAR[0]<<8)+(uint16)SUBN_VAR[1]);
	IINCHIP_WRITE(W5300_SUBR2,((uint16)SUBN_VAR[2]<<8)+(uint16)SUBN_VAR[3]);   
	
  #ifdef __DEF_IINCHIP_DBG__
      printf("%d ：ARP请求服务器IP地址是2 : %d.%d.%d.%d\r\n",6,SUBN_VAR[0],SUBN_VAR[1],SUBN_VAR[2],SUBN_VAR[3]);
   #endif 
}
void clearSUBR(void)
{
    getSIPR(IP_VAR);
	if(IP_VAR[0] ==0 &&IP_VAR[1] ==0 &&IP_VAR[2] ==0&&IP_VAR[3] ==0)
    {   
        IINCHIP_WRITE(W5300_SUBR, ((uint16)0));
        IINCHIP_WRITE(W5300_SUBR2,((uint16)0));   
    }
}


//SIPR本机IP地址寄存器
void getSIPR(uint8* addr)
{
	addr[0]=(uint8)(IINCHIP_READ(W5300_SIPR)>>8);
	addr[1]=(uint8)(IINCHIP_READ(W5300_SIPR));
	addr[2]=(uint8)(IINCHIP_READ(W5300_SIPR2)>>8);
	addr[3]=(uint8)(IINCHIP_READ(W5300_SIPR2));
   #ifdef __DEF_IINCHIP_DBG__
      printf("%d ：ARP请求服务器IP地址是1 : %d.%d.%d.%d\r\n",6,addr[0],addr[1],addr[2],addr[3]);
   #endif 
	
}
void setSIPR(uint8* addr)
{
	IINCHIP_WRITE(W5300_SIPR, ((uint16)addr[0]<<8)+(uint16)addr[1]);
	IINCHIP_WRITE(W5300_SIPR2,((uint16)addr[2]<<8)+(uint16)addr[3]);  
	
	//IINCHIP_WRITE(W5300_GAR,  ((uint16)addr[0]<<8)+(uint16)addr[1]);
	//IINCHIP_WRITE(W5300_GAR2, ((uint16)addr[2]<<8)+(uint16)addr[3]);	
}

//RTR 重传超时寄存器
uint16 getRTR(void)
{
	return IINCHIP_READ(W5300_RTR);
}
void setRTR(uint16 timeout)
{
	IINCHIP_WRITE(W5300_RTR,timeout);
}

//RCR重传重新计数寄存器
uint8 getRCR(void)
{
	return (uint8)IINCHIP_READ(W5300_RCR);
}
void setRCR(uint8 retry)
{
	IINCHIP_WRITE(W5300_RCR,retry);
}
	
//PATR(PPPoE认证类型寄存器)
uint16 getPATR(void)
{
	return IINCHIP_READ(W5300_PATR);
}

//PTIMER(PPP连接控制协议请求定时寄存器)
uint8 getPTIMER(void)
{
	return (uint8)IINCHIP_READ(W5300_PTIMER);
}
void setPTIMER(uint8 time)
{
	IINCHIP_WRITE(W5300_PTIMER,time);
}

//PMAGICR(PPP连接控制协议的魔术数寄存器)
uint8 getPMAGICR(void)
{
   return (uint8)IINCHIP_READ(W5300_PMAGICR);
}
void setPMAGICR(uint8 magic)
{
   IINCHIP_WRITE(W5300_PMAGICR,magic);
}

//PSIDR(PPPoE回话 ID寄存器)
uint16 getPSIDR(void)
{
   return IINCHIP_READ(W5300_PSIDR);
}

//PDHAR(PPPoE目的硬件地址寄存器)
void getPDHAR(uint8* addr)
{
   addr[0] = (uint8)(IINCHIP_READ(W5300_PDHAR) >> 8);
   addr[1] = (uint8)IINCHIP_READ(W5300_PDHAR);
   addr[2] = (uint8)(IINCHIP_READ(W5300_PDHAR2) >> 8);
   addr[3] = (uint8)IINCHIP_READ(W5300_PDHAR2);
   addr[4] = (uint8)(IINCHIP_READ(W5300_PDHAR4) >> 8);
   addr[5] = (uint8)IINCHIP_READ(W5300_PDHAR4);
}

//当W5300试着向一个未打开的端口发送UDP数据时，5300会接收到ICMP,
//并通过UIPR UPORTR打包
void     getUIPR(uint8* addr)
{
   addr[0] = (uint8)(IINCHIP_READ(W5300_UIPR) >> 8);
   addr[1] = (uint8)IINCHIP_READ(W5300_UIPR);
   addr[2] = (uint8)(IINCHIP_READ(W5300_UIPR2) >> 8);
   addr[3] = (uint8)IINCHIP_READ(W5300_UIPR2);   
}
uint16 getUPORTR(void)
{
   return IINCHIP_READ(W5300_UPORTR);
}

//FMTUR
uint16 getFMTUR(void)
{
   return IINCHIP_READ(W5300_FMTUR);
}

uint8 getPn_BRDYR(uint8 p)
{
   return (uint8)IINCHIP_READ(W5300_Pn_BRDYR(p));
}
void setPn_BRDYR(uint8 p, uint8 cfg)
{
   IINCHIP_WRITE(W5300_Pn_BRDYR(p),cfg);   
}


uint16 getPn_BDPTHR(uint8 p)
{
   return IINCHIP_READ(W5300_Pn_BDPTHR(p));   
}
void setPn_BDPTHR(uint8 p, uint16 depth)
{
   IINCHIP_WRITE(W5300_Pn_BDPTHR(p),depth);
}

//读取W5300标识
uint16 getIDR(void)
{
	return IINCHIP_READ(W5300_IDR);
}

//
uint16 getSn_MR(SOCKET s)
{
	return IINCHIP_READ(Sn_MR(s));
}
void setSn_MR(SOCKET s,uint16 mode)
{
	IINCHIP_WRITE(Sn_MR(s),mode);
}

//
uint8 getSn_CR(SOCKET s)
{
   return IINCHIP_READ(Sn_CR(s));
}
void setSn_CR(SOCKET s, uint16 com)
{
   IINCHIP_WRITE(Sn_CR(s),com);
   while(IINCHIP_READ(Sn_CR(s))); //W5300接到任何命令时，CR自动清零 
}

//SCOKET 中断屏蔽寄存器
uint8 getSn_IMR(SOCKET s)
{
   return (uint8)IINCHIP_READ(Sn_IMR(s));
}
void setSn_IMR(SOCKET s, uint8 mask)
{
   IINCHIP_WRITE(Sn_IMR(s),mask);
}

//中断寄存器 
uint8 getSn_IR(SOCKET s)
{
   #ifdef __DEF_IINCHIP_INT__    
      return SOCK_INT[s];
   #else                        
      return (uint8)IINCHIP_READ(Sn_IR(s));
   #endif   
}
void setSn_IR(SOCKET s, uint8 ir)
{
   #ifdef __DEF_IINCHIP_INT__    // In case of using ISR routine of iinchip
      SOCK_INT[s] = SOCK_INT[s] & ~(ir);
   #else                         // In case of processing directly
      IINCHIP_WRITE(Sn_IR(s),ir);
   #endif   
}


//指示SOCKETs 状态
uint8 getSn_SSR(SOCKET s)
{
	uint8 ssr,ssr1;
	ssr=(uint8)IINCHIP_READ(Sn_SSR(s));
	while(1)
	{
		ssr1=(uint8)IINCHIP_READ(Sn_SSR(s));
		if(ssr==ssr1) break;	//如果两次ssr相等，为有效
		ssr=ssr1;
	}
	return ssr;
}


void getSn_DHAR(SOCKET s,uint8* addr)
{
   addr[0] = (uint8)(IINCHIP_READ(Sn_DHAR(s))>>8);
   addr[1] = (uint8)IINCHIP_READ(Sn_DHAR(s));
   addr[2] = (uint8)(IINCHIP_READ(Sn_DHAR2(s))>>8);
   addr[3] = (uint8)IINCHIP_READ(Sn_DHAR2(s));
   addr[4] = (uint8)(IINCHIP_READ(Sn_DHAR4(s))>>8);
   addr[5] = (uint8)IINCHIP_READ(Sn_DHAR4(s));
}
void setSn_DHAR(SOCKET s,uint8* addr)
{
   IINCHIP_WRITE(Sn_DHAR(s),  ((uint16)(addr[0]<<8)) + addr[1]);
   IINCHIP_WRITE(Sn_DHAR2(s), ((uint16)(addr[2]<<8)) + addr[3]);
   IINCHIP_WRITE(Sn_DHAR4(s), ((uint16)(addr[4]<<8)) + addr[5]);
}

//SOCKET目的端口寄存器
void setSn_DPORTR(SOCKET s, uint16 port)
{
   IINCHIP_WRITE(Sn_DPORTR(s),port);
}

void getSn_DIPR(SOCKET s, uint8* addr)
{
   addr[0] = (uint8)(IINCHIP_READ(Sn_DIPR(s))>>8);
   addr[1] = (uint8)IINCHIP_READ(Sn_DIPR(s));
   addr[2] = (uint8)(IINCHIP_READ(Sn_DIPR2(s))>>8);   
   addr[3] = (uint8)IINCHIP_READ(Sn_DIPR2(s));   
	
 
}
void setSn_DIPR(SOCKET s, uint8* addr)
{
   IINCHIP_WRITE(Sn_DIPR(s),  ((uint16)(addr[0]<<8)) + addr[1]);
   IINCHIP_WRITE(Sn_DIPR2(s), ((uint16)(addr[2]<<8)) + addr[3]);  
}

uint16 getSn_MSSR(SOCKET s)
{
	return IINCHIP_READ(Sn_MSSR(s));
}
void setSn_MSSR(SOCKET s,uint16 mss)
{
	IINCHIP_WRITE(Sn_MSSR(s),mss);
}

uint8 getSn_KPALVTR(SOCKET s)
{
   return (uint8)(IINCHIP_READ(Sn_KPALVTR(s)) >> 8);
}

void setSn_KPALVTR(SOCKET s, uint8 time)
{
   uint16 keepalive=0;
   keepalive = (IINCHIP_READ(Sn_KPALVTR(s)) & 0x00FF) + ((uint16)time<<8);
   IINCHIP_WRITE(Sn_KPALVTR(s),keepalive);
}
uint32 getSn_TX_WRSR(SOCKET s)
{
   uint32 tx_write_size=0;
   tx_write_size = IINCHIP_READ(Sn_TX_WRSR(s));
   tx_write_size = (tx_write_size << 16) + IINCHIP_READ(Sn_TX_WRSR2(s));
   return tx_write_size;
}

//SOCKETs发送数据字节长度寄存器
void setSn_TX_WRSR(SOCKET s, uint32 size)
{
   IINCHIP_WRITE(Sn_TX_WRSR(s), (uint16)(size >> 16));
   IINCHIP_WRITE(Sn_TX_WRSR2(s), (uint16)size);
}

//获取SOCKET s TX剩余存储器空间的字节数
uint32   getSn_TX_FSR(SOCKET s)
{
   uint32 free_tx_size=0;
   uint32 free_tx_size1=0;
   while(1)
   {
      free_tx_size = IINCHIP_READ(Sn_TX_FSR(s));                           // read                                       
      free_tx_size = (free_tx_size << 16) + IINCHIP_READ(Sn_TX_FSR2(s));                                                       
      if(free_tx_size == free_tx_size1) break;                             // if first == sencond, Sn_TX_FSR value is valid.                                                          
      free_tx_size1 = free_tx_size;                                        // save second value into firs                                                    
   }                                                                       
   return free_tx_size;                                                    
}

//SOCKETn接受数据的字节长度寄存器
uint32   getSn_RX_RSR(SOCKET s)
{
   uint32 received_rx_size=0;
   uint32 received_rx_size1=1;
   while(1)
   {
      received_rx_size = IINCHIP_READ(Sn_RX_RSR(s));
      received_rx_size = (received_rx_size << 16) + IINCHIP_READ(Sn_RX_RSR2(s));                                   
      if(received_rx_size == received_rx_size1) break;                                                                         
      received_rx_size1 = received_rx_size;                                     
   }                                                                                       
   return received_rx_size;   
}
void setSn_TX_FIFOR(SOCKET s, uint16 data)
{
   IINCHIP_WRITE(Sn_TX_FIFOR(s),data);
}

uint16   getSn_RX_FIFOR(SOCKET s)
{
   return IINCHIP_READ(Sn_RX_FIFOR(s));
}

uint8 getSn_PROTOR(SOCKET s)
{
   return (uint8)IINCHIP_READ(Sn_PROTOR(s));
}
void setSn_PROTOR(SOCKET s, uint8 pronum)
{
   uint16 protocolnum;
   protocolnum = IINCHIP_READ(Sn_PROTOR(s)) & 0xFF00 + pronum;
   IINCHIP_WRITE(Sn_PROTOR(s),protocolnum);
}

uint8 getSn_TOSR(SOCKET s)
{
   return (uint8)IINCHIP_READ(Sn_TOSR(s));
}
void setSn_TOSR(SOCKET s, uint8 tos)
{
   IINCHIP_WRITE(Sn_TOSR(s),tos);
}

uint8 getSn_TTLR(SOCKET s)
{
   return (uint8)IINCHIP_READ(Sn_TTLR(s));
}
void     setSn_TTLR(SOCKET s, uint8 ttl)
{
   IINCHIP_WRITE(Sn_TTLR(s),ttl);
}



uint8    getSn_FRAGR(SOCKET s)
{
   return (uint8)IINCHIP_READ(Sn_FRAGR(s));
}

void     setSn_FRAGR(SOCKET s, uint8 frag)
{
   IINCHIP_WRITE(Sn_FRAGR(s),frag);
}



/****************************************************************/
//软件复位
void iinchip_init(void)
{
	*((uint16*)W5300_MR)=MR_RST;//软件复位
	
	
	delay_ms(10);	//等PLL稳定
	setMR(getMR()|MR_FS); //STM32是小端模式，设置MR_FS
	
	#ifdef __DEF_IINCHIP_DBG__
	printf("MR value is %04x\r\n",*((vuint16*)W5300_MR));
	#endif
}


#ifdef __DEF_IINCHIP_INT__ 
/**
 * \todo You should do implement your interrupt request routine instead of this function.
 *       If you want to use ISR, this function should be mapped in your ISR handler.
 */
void     iinchip_irq(void)
{
   uint16 int_val;
   uint16 idx; 
   IINCHIP_CRITICAL_SECTION_ENTER();
   //M_01052008 : replaced '==' with '='.
   //while(int_val == IINCHIP_READ(IR))  // process all interrupt 
   while((int_val = IINCHIP_READ(IR)))  
   {          
      for(idx = 0 ; idx < MAX_SOCK_NUM ; idx++)
      {
         if (int_val & IR_SnINT(idx))  // check the SOCKETn interrupt
         {
            SOCK_INT[idx] |= (uint8)IINCHIP_READ(Sn_IR(idx)); // Save the interrupt stauts to SOCK_INT[idx]
            IINCHIP_WRITE(Sn_IR(idx),(uint16)SOCK_INT[idx]);  // Clear the interrupt status bit of SOCKETn
         }
      }
      
      if (int_val & (IR_IPCF << 8))    // check the IP conflict interrupt
      {
         printf("IP conflict : %04x\r\n", int_val);
      }
      if (int_val & (IR_DPUR << 8))    // check the unreachable destination interrupt
      {
         printf("INT Port Unreachable : %04x\r\n", int_val);
         printf("UIPR : %d.%d.%d.%d\r\n", (uint8)(IINCHIP_READ(UIPR)>>8),
                                          (uint8)IINCHIP_READ(UIPR),
                                          (uint8)(IINCHIP_READ(UIPR2)>>8),
                                          (uint8)IINCHIP_READ(UIPR2));
         printf("UPORTR : %04x\r\n", IINCHIP_READ(UPORTR));
      }
      IINCHIP_WRITE(IR, int_val & 0xFF00);
   }
   IINCHIP_CRITICAL_SECTION_EXIT();
}
#endif 

//配置W5300内部接收、发送存储器大小
uint8 sysinit(uint8* tx_size, uint8* rx_size)
{
	uint16 i;
	uint16 ssum=0,rsum=0;
	uint mem_cfg=0;	
	
	for(i=0;i<MAX_SOCK_NUM;i++)
	{
		if(tx_size[i]>64)	//每个SOCKET存储器分配范围为0-64kb，复位时每个分配8kb
		{
			#ifdef __DEF_IINCHIP_DBG__
			printf("Error Channel(%d) TX Memory Size.\r\n",i);
			#endif
			return 0;
		}
	
		if(rx_size[i]>64)
		{
			#ifdef __DEF_IINCHIP_DBG__
			printf("Error Channel(%d) RX Memory Size.\r\n",i);
			#endif
			return 0;
		}
		
		ssum += (uint16)tx_size[i];	
		rsum += (uint16)rx_size[i];
		TXMEM_SIZE[i]=((uint32)tx_size[i])<<10;
		RXMEM_SIZE[i]=((uint32)rx_size[i])<<10;
		
	}
		if((ssum%8)||((ssum+rsum)!=128))//TX存储器的总和应为8的倍数，TX+RX存储器的总和为128K字节
		{
			#ifdef __DEF_IINCHIP_DBG__
			printf("Illegal Memory Allocation\r\n");
			#endif
			return 0;
		}	
	

	IINCHIP_WRITE(W5300_TMS01R,(((uint16)tx_size[0])<<8)+(uint16)tx_size[1]);
	IINCHIP_WRITE(W5300_TMS23R,(((uint16)tx_size[2])<<8)+(uint16)tx_size[3]);
	IINCHIP_WRITE(W5300_TMS45R,(((uint16)tx_size[4])<<8)+(uint16)tx_size[5]);
	IINCHIP_WRITE(W5300_TMS67R,(((uint16)tx_size[6])<<8)+(uint16)tx_size[7]);
	
	IINCHIP_WRITE(W5300_RMS01R,(((uint16)rx_size[0])<<8)+(uint16)rx_size[1]);
	IINCHIP_WRITE(W5300_RMS23R,(((uint16)rx_size[2])<<8)+(uint16)rx_size[3]);
	IINCHIP_WRITE(W5300_RMS45R,(((uint16)rx_size[4])<<8)+(uint16)rx_size[5]);
	IINCHIP_WRITE(W5300_RMS67R,(((uint16)rx_size[6])<<8)+(uint16)rx_size[7]);
	
	for(i=0;i<ssum/8;i++)
	{
		mem_cfg<<=1;
		mem_cfg|=1;
	}
	IINCHIP_WRITE(W5300_MTYPER,mem_cfg);//写入存储器类型寄存器
	
	#ifdef __DEF_IINCHIP_DBG__
	  printf("Total TX Memory Size = %dKB\r\n",ssum);
      printf("Total RX Memory Size = %dKB\r\n",rsum);
      printf("Ch : TX SIZE : RECV SIZE\r\n");
      printf("%02d : %07dKB : %07dKB \r\n", 0, (uint8)(IINCHIP_READ(W5300_TMS01R)>>8),(uint8)(IINCHIP_READ(W5300_RMS01R)>>8));
      printf("%02d : %07dKB : %07dKB \r\n", 1, (uint8)IINCHIP_READ(W5300_TMS01R),     (uint8)IINCHIP_READ(W5300_RMS01R));
      printf("%02d : %07dKB : %07dKB \r\n", 2, (uint8)(IINCHIP_READ(W5300_TMS23R)>>8),(uint8)(IINCHIP_READ(W5300_RMS23R)>>8));
      printf("%02d : %07dKB : %07dKB \r\n", 3, (uint8)IINCHIP_READ(W5300_TMS23R),     (uint8)IINCHIP_READ(W5300_RMS23R));
      printf("%02d : %07dKB : %07dKB \r\n", 4, (uint8)(IINCHIP_READ(W5300_TMS45R)>>8),(uint8)(IINCHIP_READ(W5300_RMS45R)>>8));
      printf("%02d : %07dKB : %07dKB \r\n", 5, (uint8)IINCHIP_READ(W5300_TMS45R),     (uint8)IINCHIP_READ(W5300_RMS45R));
      printf("%02d : %07dKB : %07dKB \r\n", 6, (uint8)(IINCHIP_READ(W5300_TMS67R)>>8),(uint8)(IINCHIP_READ(W5300_RMS67R)>>8));
      printf("%02d : %07dKB : %07dKB \r\n", 7, (uint8)IINCHIP_READ(W5300_TMS67R),     (uint8)IINCHIP_READ(W5300_RMS67R));
      printf("\r\nW5300_MTYPER=%04x\r\n",IINCHIP_READ(W5300_MTYPER)); //存储器配置类型 0xffff TX RX各位64kb
	#endif
	
	return 1;
}



uint32   wiz_write_buf(SOCKET s,uint8* buf,uint32 len)
{
#if (__DEF_IINCHIP_ADDRESS_MODE__ == __DEF_IINCHIP_DIRECT_MODE__)
   #if (__DEF_IINCHIP_BUF_OP__ == __DEF_C__)
      uint32 idx=0;
      
      for (idx = 0; idx < len; idx+=2)
         IINCHIP_WRITE(Sn_TX_FIFOR(s),*((uint16*)(buf+idx))); 
   #endif

/********************************************************************
//间接地址模式未使用		
#elif (__DEF_IINCHIP_ADDRESS_MODE__ == __DEF_IINCHIP_INDIRECT_MODE__)
   #if (__DEF_IINCHIP_BUF_OP__ == __DEF_C__)
      uint32 idx=0;
      IINCHIP_CRITICAL_SECTION_ENTER();
      *((vuint16*)IDM_AR) = Sn_TX_FIFOR(s);
      for (idx = 0; idx < len; idx+=2)
          *((vuint16*)IDM_DR) = *((uint16*)(buf+idx));
      IINCHIP_CRITICAL_SECTION_EXIT();

  #elif (__DEF_IINCHIP_BUF_OP__ == __DEF_MCU_DEP_INLINE_ASM__)
      register uint16 data=0;
      register uint16 loop;
      loop = len;
      IINCHIP_CRITICAL_SECTION_ENTER();    
      *((vuint16*)IDM_AR) = Sn_TX_FIFOR(s);
      __asm
      {
      fifo_write_loop:
        // ARM version
        LDRH    data, [buf], #2
        STRH    data, [IDM_DR]
        SUBS    loop,loop, #2
        BGT     fifo_write_loop      
      }
      IINCHIP_CRITICAL_SECTION_EXIT();
   #elif (__DEF_IINCHIP_BUF_OP__ == __DEF_MCU_DEP_DMA__)
      #error "You should do implement this block."
   #else
      #error "Undefined __DEF_IINCHIP_BUF_OP__"
   #endif 
#else
   #error "Undefined __DEF_IINCHIP_ADDRESS_MODE__"  
*********************************************************************/   
#endif
    return len;   
}



/****************************************************************************
*函数： uint32 wiz_read_buf(SOCKET s,uint8* buf,uint32 len)
*功能： 
*返回值：
*备注：在使用recv命令前，主机需要通过Sn_RX_FIFOR寄存器从RX存储器读取接收的数据
******************************************************************************/
uint32   wiz_read_buf(SOCKET s, uint8* buf,uint32 len)
{

#if (__DEF_IINCHIP_ADDRESS_MODE__ == __DEF_IINCHIP_DIRECT_MODE__)
   #if (__DEF_IINCHIP_BUF_OP__ == __DEF_C__)
      uint32 idx;
      for (idx = 0; idx < len; idx+=2)
         *((uint16*)(buf+idx)) = IINCHIP_READ(Sn_RX_FIFOR(s));
   #else
      #error "Undefined __DEF_IINCHIP_BUF_OP__"
   #endif 
	
/********************************************************************
//间接地址模式未使用	
#elif (__DEF_IINCHIP_ADDRESS_MODE__ == __DEF_IINCHIP_INDIRECT_MODE__)
   #if (__DEF_IINCHIP_BUF_OP__ == __DEF_C__)
      uint32 idx=0;
      IINCHIP_CRITICAL_SECTION_ENTER();
      *((vuint16*)IDM_AR) = Sn_RX_FIFOR(s);
      for (idx = 0; idx < len; idx+=2)
         *((uint16*)(buf+idx)) = *((vuint16*)IDM_DR);
      IINCHIP_CRITICAL_SECTION_EXIT();

   #elif (__DEF_IINCHIP_BUF_OP__ == __DEF_MCU_DEP_INLINE_ASM__)
      register uint16 data=0;
      register int16 loop;
      loop = (int16)len;
      IINCHIP_CRITICAL_SECTION_ENTER();      
      *((vuint16*)IDM_AR) = Sn_RX_FIFOR(s);
      __asm
      {
      fifo_read_loop:
        // ARM version
        LDRH    data, [IDM_DR]
        STRH    data, [buf], #2
        SUBS    loop,loop, #2
        BGT     fifo_read_loop      
      }
      IINCHIP_CRITICAL_SECTION_EXIT();
   #elif (__DEF_IINCHIP_BUF_OP__ == __DEF_MCU_DEP_DMA__)
      #error "You should do implement this block."
   #else
      #error "Undefined __DEF_IINCHIP_BUF_OP__"
   #endif 
#else
   #error "Undefined __DEF_IINCHIP_ADDRESS_MODE__"   
********************************************************************/

#endif
    return len;
}


uint32 getIINCHIP_TxMAX(SOCKET s)
{
	return TXMEM_SIZE[s];
}

uint32 getIINCHIP_RxMAX(SOCKET s)
{
   return RXMEM_SIZE[s];
}

void send_keepalive(uint8 s)
{
  IINCHIP_WRITE(Sn_CR(s),Sn_CR_SEND_KEEP);		// 设置Sn_CR寄存器为SEND_KEEP 0x22，KA包完成发送
  while( IINCHIP_READ(Sn_CR(s)) );
}





















