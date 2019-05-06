#include "socket.h"
#include "delay.h"
#include "usart.h"

uint16   iinchip_source_port;  
uint8    check_sendok_flag[MAX_SOCK_NUM]; // sendok SOCKET���
/*********************************************************
*������ uint8 socket(SOCKET s,uint8 protocol,uint16 port,uint16 flag)
*���ܣ� ����SOCKETn��Э�����ͻ�ѡ��
*port��	����Դ�˿ڵĶ˿ں�
*����ֵ��
*��ע��SOCKETn����TCP��UDPģʽ����Ч������ģʽ������
*socket(0,0X01,5000,0);   // open the SOCKET 
***********************************************************/
uint8 socket(SOCKET s,uint8 protocol,uint16 port,uint16 flag)
{
	IINCHIP_WRITE(Sn_MR(s),(uint16)(protocol|flag)); //����Sn_MR,����SOCKETn��Э������ѡ��
	if(port != 0) IINCHIP_WRITE(Sn_PORTR(s),port);	//����Դ�˿ڵĶ˿ں�
	else
	{
		iinchip_source_port++;
		IINCHIP_WRITE(Sn_PORTR(s),iinchip_source_port);
	}
	setSn_CR(s, Sn_CR_OPEN);      // ��SOCKET s 
	check_sendok_flag[s] = 1;     // sendok SOCKET ���
  
   #ifdef __DEF_IINCHIP_DBG__
      printf("%d : Sn_MR=0x%04x,Sn_PORTR=0x%04x(%d),Sn_SSR=%04x\r\n",s,IINCHIP_READ(Sn_MR(s)),IINCHIP_READ(Sn_PORTR(s)),IINCHIP_READ(Sn_PORTR(s)),getSn_SSR(s));
	  
   #endif
   return 1;
}

/*********************************************************
*������ void close(SOCKET s)
*���ܣ� 
*port��	
*����ֵ��
*��ע��
***********************************************************/
void close(SOCKET s)
{
   
	if( ((getSn_MR(s)& 0x0F) == Sn_MR_TCP) && (getSn_TX_FSR(s) != getIINCHIP_TxMAX(s)) )  //ֱ�ӵ�ַģʽ��MR 0bit��0����������if
   { 
      uint16 loop_cnt =0;
      while(getSn_TX_FSR(s) != getIINCHIP_TxMAX(s))
      {
         if(loop_cnt++ > 10)
         {
            uint8 destip[4];
            //getSIPR(destip);
            destip[0] = 0;destip[1] = 0;destip[2] = 0;destip[3] = 1;
            socket(s,Sn_MR_UDP,0x3000,0);
            sendto(s,(uint8*)"x",1,destip,0x3000); //��δ֪�˿ڷ�������(0.0.0.1).
            break;
         }
         delay_ms(10);
      }
   };
 
   setSn_IR(s ,0x00FF);          // ���IR�ж�λ
   setSn_CR(s ,Sn_CR_CLOSE);     // �ر�SOCKET s    
}

/*********************************************************
*������ void close(SOCKET s)
*���ܣ� 
*port��	
*����ֵ��
*��ע��
***********************************************************/
uint8 connect(SOCKET s, uint8* addr, uint16 port)
{
	uint8  sip1[4];	
	//uint8  serverip1[4] = {192,168,1,1};	//TCP SERVERģʽIP��ַ
   if
   (
      ((addr[0] == 0xFF) && (addr[1] == 0xFF) && (addr[2] == 0xFF) && (addr[3] == 0xFF)) ||
      ((addr[0] == 0x00) && (addr[1] == 0x00) && (addr[2] == 0x00) && (addr[3] == 0x00)) ||
      (port == 0x00) 
   )
   {
      #ifdef __DEF_IINCHIP_DBG__
         printf("%d :Error��Clintģʽ�·�������Ip,Portδ����]\r\n",s);
      #endif
      return 0;
   }
   
   
   // ���÷�����Ŀ�� IP 
   IINCHIP_WRITE((Sn_DIPR(s)),((uint16)addr[0]<<8)+(uint16)addr[1]);
   IINCHIP_WRITE((Sn_DIPR2(s)),((uint16)addr[2]<<8)+(uint16)addr[3]);

   getSn_DIPR(s,sip1);
   
   #ifdef __DEF_IINCHIP_DBG__
      printf("%d ��Clintģʽ�·�������ַΪ : %d.%d.%d.%d\r\n",s,sip1[0],sip1[1],sip1[2],sip1[3]);
   #endif   
   
   //���÷�����Ŀ�� port 
   IINCHIP_WRITE((Sn_DPORTR(s)),port);
   
   #ifdef __DEF_IINCHIP_DBG__
      printf("%d ��Clintģʽ�·������˿�Ϊ : %d\r\n",s,port);
   #endif  
   
  
    clearSUBR(); 

    getSUBR(sip1);
    //printf("%d ��Clintģʽ���������� : %d.%d.%d.%d\r\n",s,sip1[0],sip1[1],sip1[2],sip1[3]);
	
	//printf("%d : Sn_CR=0x%04x,Sn_IR=0x%04x(%d),Sn_SSR=%04x\r\n",s,IINCHIP_READ(Sn_CR(s)),IINCHIP_READ(Sn_IR(s)),IINCHIP_READ(Sn_IR(s)),getSn_SSR(s));//��ѯssr��ʼ״̬
    setSn_CR(s,Sn_CR_CONNECT); //�������������ָ����TCP������
    //printf("%d : Sn_CR=0x%04x,Sn_IR=0x%04x(%d),Sn_SSR=%04x\r\n",s,IINCHIP_READ(Sn_CR(s)),IINCHIP_READ(Sn_IR(s)),IINCHIP_READ(Sn_IR(s)),getSn_SSR(s));//��ѯssr��ʼ״̬
  // while(Sn_CR(s));
   
  // IINCHIP_WRITE(W5300_SUBR,0X00);
  // IINCHIP_WRITE(W5300_SUBR2,0X00);
   
	getSUBR(sip1);
    //printf("%d ��Clintģʽ���������� : %d.%d.%d.%d\r\n",s,sip1[0],sip1[1],sip1[2],sip1[3]);
	
   while(IINCHIP_READ(Sn_SSR(s)) != SOCK_SYNSENT) //����������SYN���ݰ������͵�TCP������
   {
		if(IINCHIP_READ(Sn_SSR(s)) == SOCK_ESTABLISHED )
		{
			#ifdef __DEF_IINCHIP_DBG__
				printf("Clint is SOCK_ESTABLISHED ]\r\n",s);
			#endif
			break;	
		}
		if(getSn_IR(s) & Sn_IR_TIMEOUT)  //��ʱ�жϣ�ARP��TCP��ʱ��
		{
			setSn_IR(s,(Sn_IR_TIMEOUT));
			#ifdef __DEF_IINCHIP_DBG__
				printf("%d : Fail[��ʱ�ж�]\r\n",s);
			#endif
			break;
		}
   }
	//ClearSubnet(); //������������
	applySUBR();
   return 1;
}


/*********************************************************
*������ void disconnect(SOCKET s)
*���ܣ� 
*port��	
*����ֵ����
*��ע��ֻ��TCPģʽ��Ч
***********************************************************/
void disconnect(SOCKET s)
{
	setSn_CR(s,Sn_CR_DISCON);
}

/*********************************************************
*������ uint8 listen(SOCKET s)
*���ܣ� 
*����ֵ��1
*��ע��SOCK_INIT ����TCP���ӵĵ�һ��
***********************************************************/
uint8 listen(SOCKET s)
{
   if (getSn_SSR(s) != SOCK_INIT)
   {
      #ifdef __DEF_IINCHIP_DBG__
         printf("%d : SOCKET is not created!\r\n",s);
      #endif
      return 0;
   }

   setSn_CR(s,Sn_CR_LISTEN);     // ����listen����
   #ifdef __DEF_IINCHIP_DBG__
       printf("%d : SOCKET IN LISTENLING !\r\n",s);
   #endif
   return 1;
}  


/*********************************************************
*������ uint32 send(SOCKET s, uint8 * buf, uint32 len)
*���ܣ� 
*port��	
*����ֵ��0 δ���ͳɹ�
*��ע��
***********************************************************/
uint32 send(SOCKET s, uint8 * buf, uint32 len)
{
	uint8 status=0;
	uint32 ret=0;
	uint32 freesize=0;
	#ifdef __DEF_IINCHIP_DBG__
		uint32 loopcnt = 0;
        #ifdef __DEF_IINCHIP_DBG__
			printf("%d : send()\r\n",s);
	    #endif
	#endif
   
	ret = len;
	if (len > getIINCHIP_TxMAX(s))   ret = getIINCHIP_TxMAX(s); // �������ݴ�С���ܳ���TX�洢����С
   
	do
	{
		freesize=getSn_TX_FSR(s); //SOCKET s TXʣ��洢���ռ���ֽ���
	    status=getSn_SSR(s);	//ָʾSOCKETs ״̬
		#ifdef __DEF_IINCHIP_DBG__
			printf("%d : freesize=%ld\r\n",s,freesize);
			if(loopcnt++ > 0x0010000)
			{
				printf("%d : freesize=%ld,status=%04x\r\n",s,freesize,status);
				printf("%d:Send Size=%08lx(%d)\r\n",s,ret,ret);
				printf("MR=%04x\r\n",*((vuint16*)W5300_MR));
				loopcnt = 0;
			}
	   	#endif
		if((status != SOCK_ESTABLISHED) &&	(status != SOCK_CLOSE_WAIT)) //SOCKETsδ��������
		return 0; 			
	}

	while(freesize<ret);//������2��ͬ����
	//while((freesize = getSn_TX_FSR(s))==0); 
    //ret = freesize;
   
	wiz_write_buf(s,buf,ret); 
	
	#ifdef __DEF_IINCHIP_DBG__
		loopcnt=0;
	#endif
   
	if(!check_sendok_flag[s]) 
	{
		while (!(getSn_IR(s) & Sn_IR_SENDOK)) //�ȴ�SOCKETs send��������
		{
			#ifdef __DEF_IINCHIP_DBG__
				if(loopcnt++ > 0x010000)
				{
					printf("%d:Sn_SSR(%04x)\r\n",s,status);
					printf("%d:Send Size=%08lx(%d)\r\n",s,ret,ret);
					printf("MR=%04x\r\n",*((vuint16*)W5300_MR));
					loopcnt = 0;
				}
			#endif
		
		if(getSn_SSR(s)==SOCK_CLOSED) //�ж��Ƿ�ʱ || ��ִ��DISCON��CLOSE����
			{
				#ifdef __DEF_IINCHIP_DBG__
					printf("%d : Send Fail. SOCK_CLOSED.\r\n",s);
				#endif
				return 0;
			}
		}
		setSn_IR(s,Sn_IR_SENDOK);//����ж�λ��sendok
		
	}
	else check_sendok_flag[s]=0;
	
   // send
   setSn_TX_WRSR(s,ret);   
   setSn_CR(s,Sn_CR_SEND);
   
   return ret;
}

/*********************************************************
*������uint32 recve(SOCKET s,uint8* buf,uint32 len)
*���ܣ� 
*port��	
*����ֵ��
*��ע��
***********************************************************/
uint32 recv(SOCKET s,uint8* buf,uint32 len)
{
	uint16 pack_size=0;
	uint16 mr = getMR();	
	
	#ifdef __DEF_IINCHIP_DBG__
		printf("%d : recv()\r\n",s);
	#endif
	
	if(IINCHIP_READ(Sn_MR(s)) & Sn_MR_ALIGN) //���ж���  ALIGN=1ʱ��TCP���ݰ�ֻ�����ݣ�û��PACK-INFO
	{
		wiz_read_buf(s, buf, (uint32)len); 
		setSn_CR(s,Sn_CR_RECV);
		return len;
	}
	
	wiz_read_buf(s,(uint8*)&pack_size,2);        // ��ȡ���ݰ�����
	
	if( mr & MR_FS )  //fifo������λ ��0��ֹ���� 1��������
	{
		pack_size = ((((pack_size << 8 ) & 0xFF00)) | ((pack_size >> 8)& 0x00FF));
	
		#ifdef __DEF_IINCHIP_DBG__
			printf("%d:pack_size=%d\r\n",s,pack_size);
		#endif
	}
	wiz_read_buf(s, buf, (uint32)pack_size);     
    setSn_CR(s,Sn_CR_RECV);                      // recv
	
	#if 0
	if(getSn_RX_RSR(s) == 0)                     // ��ⴰ�ڳߴ��Ƿ�Ϊ0��Ϊ0ʱ���Ͷ˲��ٷ�������
	{ 
		setSn_TX_WRSR(s,1);                       // size : 1 byte dummy size
		IINCHIP_WRITE(Sn_TX_FIFOR(s),0x0000);     // write dummy data into tx memory
		setSn_CR(s,Sn_CR_SEND);                   // send                         
		while(!(getSn_IR(s) & Sn_IR_SENDOK));     // wait SEND command completion 
		setSn_IR(s,Sn_IR_SENDOK);                 // clear Sn_IR_SENDOK bit       
	}    
	#endif
	
	return (uint32)pack_size;
}

/*********************************************************
*������uint32  sendto(SOCKET s, uint8 * buf, uint32 len, uint8 * addr, uint16 port)
*���ܣ� 
*port��	
*����ֵ��
*��ע��
***********************************************************/
uint32  sendto(SOCKET s, uint8 * buf, uint32 len, uint8 * addr, uint16 port)
{
   uint8 status=0;
   uint8 isr=0;
   uint32 ret=0;
   
   #ifdef __DEF_IINCHIP_DBG__
      printf("%d : sendto():%d.%d.%d.%d(%d), len=%d\r\n",s, addr[0], addr[1], addr[2], addr[3] , port, len);
   #endif
      
   if
   (
      ((addr[0] == 0x00) && (addr[1] == 0x00) && (addr[2] == 0x00) && (addr[3] == 0x00)) ||
      ((port == 0x00)) ||(len == 0)
   ) 
   {
      #ifdef __DEF_IINCHIP_DBG__
         printf("%d : Fail[%d.%d.%d.%d, %.d, %d]\r\n",s, addr[0], addr[1], addr[2], addr[3] , port, len);
      #endif
      return 0;
   }
   
   
   if (len > getIINCHIP_TxMAX(s)) ret = getIINCHIP_TxMAX(s); // check size not to exceed MAX size.
   else ret = len;
   
   // set destination IP address
   IINCHIP_WRITE(Sn_DIPR(s),(((uint16)addr[0])<<8) + (uint16) addr[1]);
   IINCHIP_WRITE(Sn_DIPR2(s),(((uint16)addr[2])<<8) + (uint16) addr[3]);
   // set destination port number
   IINCHIP_WRITE(Sn_DPORTR(s),port);
   
   wiz_write_buf(s, buf, ret);                              // copy data
   // send
   setSn_TX_WRSR(s,ret);
   //ApplySubnet();
   clearSUBR();
   setSn_CR(s, Sn_CR_SEND);
   
   while (!((isr = getSn_IR(s)) & Sn_IR_SENDOK))            // wait SEND command completion
   {
      status = getSn_SSR(s);                                // warning ---------------------------------------
      if ((status == SOCK_CLOSED) || (isr & Sn_IR_TIMEOUT)) // Sn_IR_TIMEOUT causes the decrement of Sn_TX_FSR
      {                                                     // -----------------------------------------------
         #ifdef __DEF_IINCHIP_DBG__
            printf("%d: send fail.status=0x%02x,isr=%02x\r\n",status,isr);
         #endif
         setSn_IR(s,Sn_IR_TIMEOUT);
         return 0;
      }
   }
   applySUBR();//ClearSubnet();
   setSn_IR(s, Sn_IR_SENDOK); // Clear Sn_IR_SENDOK
   
   
   #ifdef __DEF_IINCHIP_DBG__		
      printf("%d : send()end\r\n",s);
   #endif       
   
   return ret;   
}

/*******************************************************************************************************
*������uint32   recvfrom(SOCKET s, uint8 * buf, uint32 len, uint8 * addr, uint16  *port)
*���ܣ� 
*port��	
*����ֵ��
*��ע��
*********************************************************************************************************/
uint32   recvfrom(SOCKET s, uint8 * buf, uint32 len, uint8 * addr, uint16  *port)
{
   uint16 head[4];
   uint32 data_len=0;
   uint16 crc[2];
   
   #ifdef __DEF_IINCHIP_DBG__
      printf("recvfrom()\r\n");
   #endif
   
   if ( len > 0 )
   {
      switch (IINCHIP_READ(Sn_MR(s)) & 0x07)       // check the mode of s-th SOCKET
      {                                            // -----------------------------
         case Sn_MR_UDP :                          // UDP mode 
            wiz_read_buf(s, (uint8*)head, 8);      // extract the PACKET-INFO
            // read peer's IP address, port number.
            if(*((vuint16*)W5300_MR) & MR_FS)            // check FIFO swap bit
            {
               head[0] = ((((head[0] << 8 ) & 0xFF00)) | ((head[0] >> 8)& 0x00FF));
               head[1] = ((((head[1] << 8 ) & 0xFF00)) | ((head[1] >> 8)& 0x00FF));
               head[2] = ((((head[2] << 8 ) & 0xFF00)) | ((head[2] >> 8)& 0x00FF));
               head[3] = ((((head[3] << 8 ) & 0xFF00)) | ((head[3] >> 8)& 0x00FF));
            }
            addr[0] = (uint8)(head[0] >> 8);       // destination IP address
            addr[1] = (uint8)head[0];
            addr[2] = (uint8)(head[1]>>8);
            addr[3] = (uint8)head[1];
            *port = head[2];                       // destination port number
            data_len = (uint32)head[3];            // DATA packet length
            
            #ifdef __DEF_IINCHIP_DBG__
               printf("UDP msg arrived:%d(0x%04x)\r\n",data_len,data_len);
               printf("source Port : %d\r\n", *port);
               printf("source IP : %d.%d.%d.%d\r\n", addr[0], addr[1], addr[2], addr[3]);
            #endif
   
            wiz_read_buf(s, buf, data_len);        // data copy.
            break;
                                                   // -----------------------
         case Sn_MR_IPRAW :                        // IPRAW mode
            wiz_read_buf(s, (uint8*)head, 6);      // extract the PACKET-INFO 
            if(*((vuint16*)W5300_MR) & MR_FS)            // check FIFO swap bit
            {
               head[0] = ((((head[0] << 8 ) & 0xFF00)) | ((head[0] >> 8)& 0x00FF));
               head[1] = ((((head[1] << 8 ) & 0xFF00)) | ((head[1] >> 8)& 0x00FF));
               head[2] = ((((head[2] << 8 ) & 0xFF00)) | ((head[2] >> 8)& 0x00FF));
            }   			
            addr[0] = (uint8)(head[0] >> 8);       // destination IP address
            addr[1] = (uint8)head[0];
            addr[2] = (uint8)(head[1]>>8);
            addr[3] = (uint8)head[1];
            data_len = (uint32)head[2];            // DATA packet length
            
            #ifdef __DEF_IINCHIP_DBG__
               printf("IP RAW msg arrived\r\n");
               printf("source IP : %d.%d.%d.%d\r\n", addr[0], addr[1], addr[2], addr[3]);
            #endif
            
            wiz_read_buf(s, buf, data_len);        // data copy.
            break;                                 
                                                   // -----------------------
         case Sn_MR_MACRAW :                       // MACRAW mode
            wiz_read_buf(s,(uint8*)head,2);        // extract the PACKET-INFO
            if(*((vuint16*)W5300_MR) & MR_FS)            // check FIFO swap bit
               head[0] = ((((head[0] << 8 ) & 0xFF00)) | ((head[0] >> 8)& 0x00FF));
            data_len = (uint32)head[0];            // DATA packet length
            wiz_read_buf(s, buf, data_len);        // data copy.
            wiz_read_buf(s,(uint8*)crc, 4);        // extract CRC data and ignore it.

            break;
         default :
            break;
      }
      setSn_CR(s,Sn_CR_RECV);                      // recv
   }
   #ifdef __DEF_IINCHIP_DBG__
      printf("recvfrom() end ..\r\n");
   #endif
   
   return data_len;   
}
















