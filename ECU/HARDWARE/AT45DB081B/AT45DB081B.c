//#include "AT45DB081B.h"
//#include "delay.h"
//#include "usart.h"

//void AT45DB081B_Init(void)
//{
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;		//FLASHƬѡ��ʼ��
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
//	GPIO_Init(GPIOA,&GPIO_InitStructure);
//	
//	AT45DB081B_CS=1;
//	SPI1_Init();
//	SPI1_SetSpeed(SPI_BaudRatePrescaler_2);		//����Ϊ42Mʱ��,����ģʽ 
//	//FLASH_TYPE=AT45DB081B_ReadID();	//��ȡFLASH ID
//}

////��ȡAT45DB081B��״̬�Ĵ���
////BIT7  6   5   4   3   2   1   0
////SPR   RV  TB BP2 BP1 BP0 WEL BUSY
////SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
////TB,BP2,BP1,BP0:FLASH����д��������
////WEL:дʹ������
////BUSY:æ���λ(1,æ;0,����)
////Ĭ��:0x00
//u8 AT45DB081B_ReadSR(void)   
//{  
//	u8 byte=0;   
//	AT45DB081B_CS=0;                            //ʹ������   
//	SPI1_ReadWriteByte(W25X_ReadStatusReg);    //���Ͷ�ȡ״̬�Ĵ�������    
//	byte=SPI1_ReadWriteByte(0Xff);             //��ȡһ���ֽ�  
//	AT45DB081B_CS=1;                            //ȡ��Ƭѡ     
//	return byte;   
//} 
////дAT45DB081B״̬�Ĵ���
////ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
//void AT45DB081B_Write_SR(u8 sr)   
//{   
//	AT45DB081B_CS=0;                            //ʹ������   
//	SPI1_ReadWriteByte(W25X_WriteStatusReg);   //����дȡ״̬�Ĵ�������    
//	SPI1_ReadWriteByte(sr);               //д��һ���ֽ�  
//	AT45DB081B_CS=1;                            //ȡ��Ƭѡ     	      
//}   
////AT45DB081Bдʹ��	
////��WEL��λ   
//void AT45DB081B_Write_Enable(void)   
//{
//	AT45DB081B_CS=0;                            //ʹ������   
//    SPI1_ReadWriteByte(W25X_WriteEnable);      //����дʹ��  
//	AT45DB081B_CS=1;                            //ȡ��Ƭѡ     	      
//} 
////AT45DB081Bд��ֹ	
////��WEL����  
//void AT45DB081B_Write_Disable(void)   
//{  
//	AT45DB081B_CS=0;                            //ʹ������   
//    SPI1_ReadWriteByte(W25X_WriteDisable);     //����д��ָֹ��    
//	AT45DB081B_CS=1;                            //ȡ��Ƭѡ     	      
//} 		
////��ȡоƬID
////����ֵ����:				   
////0XEF13,��ʾоƬ�ͺ�ΪW25Q80  
////0XEF14,��ʾоƬ�ͺ�ΪW25Q16    
////0XEF15,��ʾоƬ�ͺ�ΪW25Q32  
////0XEF16,��ʾоƬ�ͺ�ΪW25Q64 
////0XEF17,��ʾоƬ�ͺ�ΪW25Q128 	  
//u16 AT45DB081B_ReadID(void)
//{
//	u16 Temp = 0;	  
//	AT45DB081B_CS=0;				    
//	SPI1_ReadWriteByte(0x90);//���Ͷ�ȡID����	    
//	SPI1_ReadWriteByte(0x00); 	    
//	SPI1_ReadWriteByte(0x00); 	    
//	SPI1_ReadWriteByte(0x00); 	 			   
//	Temp|=SPI1_ReadWriteByte(0xFF)<<8;  
//	Temp|=SPI1_ReadWriteByte(0xFF);	 
//	AT45DB081B_CS=1;				    
//	return Temp;
//}   		    
////��ȡSPI FLASH  
////��ָ����ַ��ʼ��ȡָ�����ȵ�����
////pBuffer:���ݴ洢��
////ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
////NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
//void AT45DB081B_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
//{ 
// 	u16 i;   										    
//	AT45DB081B_CS=0;                            //ʹ������   
//    SPI1_ReadWriteByte(W25X_ReadData);         //���Ͷ�ȡ����   
//    SPI1_ReadWriteByte((u8)((ReadAddr)>>16));  //����24bit��ַ    
//    SPI1_ReadWriteByte((u8)((ReadAddr)>>8));   
//    SPI1_ReadWriteByte((u8)ReadAddr);   
//    for(i=0;i<NumByteToRead;i++)
//	{ 
//        pBuffer[i]=SPI1_ReadWriteByte(0XFF);   //ѭ������  
//    }
//	AT45DB081B_CS=1;  				    	      
//}  
////SPI��һҳ(0~65535)��д������256���ֽڵ�����
////��ָ����ַ��ʼд�����256�ֽڵ�����
////pBuffer:���ݴ洢��
////WriteAddr:��ʼд��ĵ�ַ(24bit)
////NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!	 
//void AT45DB081B_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
//{
// 	u16 i;  
//    AT45DB081B_Write_Enable();                  //SET WEL 
//	AT45DB081B_CS=0;                            //ʹ������   
//    SPI1_ReadWriteByte(W25X_PageProgram);      //����дҳ����   
//    SPI1_ReadWriteByte((u8)((WriteAddr)>>16)); //����24bit��ַ    
//    SPI1_ReadWriteByte((u8)((WriteAddr)>>8));   
//    SPI1_ReadWriteByte((u8)WriteAddr);   
//    for(i=0;i<NumByteToWrite;i++)SPI1_ReadWriteByte(pBuffer[i]);//ѭ��д��  
//	AT45DB081B_CS=1;                            //ȡ��Ƭѡ 
//	AT45DB081B_Wait_Busy();					   //�ȴ�д�����
//} 
////�޼���дSPI FLASH 
////����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
////�����Զ���ҳ���� 
////��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
////pBuffer:���ݴ洢��
////WriteAddr:��ʼд��ĵ�ַ(24bit)
////NumByteToWrite:Ҫд����ֽ���(���65535)
////CHECK OK
//void AT45DB081B_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
//{ 			 		 
//	u16 pageremain;	   
//	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���		 	    
//	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
//	while(1)
//	{	   
//		AT45DB081B_Write_Page(pBuffer,WriteAddr,pageremain);
//		if(NumByteToWrite==pageremain)break;//д�������
//	 	else //NumByteToWrite>pageremain
//		{
//			pBuffer+=pageremain;
//			WriteAddr+=pageremain;	

//			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
//			if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
//			else pageremain=NumByteToWrite; 	  //����256���ֽ���
//		}
//	};	    
//} 
////дSPI FLASH  
////��ָ����ַ��ʼд��ָ�����ȵ�����
////�ú�������������!
////pBuffer:���ݴ洢��
////WriteAddr:��ʼд��ĵ�ַ(24bit)						
////NumByteToWrite:Ҫд����ֽ���(���65535)   
//u8 AT45DB081B_BUFFER[4096];		 
//void AT45DB081B_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
//{ 
//	u32 secpos;
//	u16 secoff;
//	u16 secremain;	   
// 	u16 i;    
//	u8 * AT45DB081B_BUF;	  
//   	AT45DB081B_BUF=AT45DB081B_BUFFER;	     
// 	secpos=WriteAddr/4096;//������ַ  
//	secoff=WriteAddr%4096;//�������ڵ�ƫ��
//	secremain=4096-secoff;//����ʣ��ռ��С   
// 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//������
// 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
//	while(1) 
//	{	
//		AT45DB081B_Read(AT45DB081B_BUF,secpos*4096,4096);//������������������
//		for(i=0;i<secremain;i++)//У������
//		{
//			if(AT45DB081B_BUF[secoff+i]!=0XFF)break;//��Ҫ����  	  
//		}
//		if(i<secremain)//��Ҫ����
//		{
//			AT45DB081B_Erase_Sector(secpos);//�����������
//			for(i=0;i<secremain;i++)	   //����
//			{
//				AT45DB081B_BUF[i+secoff]=pBuffer[i];	  
//			}
//			AT45DB081B_Write_NoCheck(AT45DB081B_BUF,secpos*4096,4096);//д����������  

//		}else AT45DB081B_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
//		if(NumByteToWrite==secremain)break;//д�������
//		else//д��δ����
//		{
//			secpos++;//������ַ��1
//			secoff=0;//ƫ��λ��Ϊ0 	 

//		   	pBuffer+=secremain;  //ָ��ƫ��
//			WriteAddr+=secremain;//д��ַƫ��	   
//		   	NumByteToWrite-=secremain;				//�ֽ����ݼ�
//			if(NumByteToWrite>4096)secremain=4096;	//��һ����������д����
//			else secremain=NumByteToWrite;			//��һ����������д����
//		}	 
//	};	 
//}
////��������оƬ		  
////�ȴ�ʱ�䳬��...
//void AT45DB081B_Erase_Chip(void)   
//{                                   
//    AT45DB081B_Write_Enable();                  //SET WEL 
//    AT45DB081B_Wait_Busy();   
//  	AT45DB081B_CS=0;                            //ʹ������   
//    SPI1_ReadWriteByte(W25X_ChipErase);        //����Ƭ��������  
//	AT45DB081B_CS=1;                            //ȡ��Ƭѡ     	      
//	AT45DB081B_Wait_Busy();   				   //�ȴ�оƬ��������
//}   
////����һ������
////Dst_Addr:������ַ ����ʵ����������
////����һ��ɽ��������ʱ��:150ms
//void AT45DB081B_Erase_Sector(u32 Dst_Addr)   
//{  
//	//����falsh�������,������   
// 	printf("fe:%x\r\n",Dst_Addr);	  
// 	Dst_Addr*=4096;
//    AT45DB081B_Write_Enable();                  //SET WEL 	 
//    AT45DB081B_Wait_Busy();   
//  	AT45DB081B_CS=0;                            //ʹ������   
//    SPI1_ReadWriteByte(W25X_SectorErase);      //������������ָ�� 
//    SPI1_ReadWriteByte((u8)((Dst_Addr)>>16));  //����24bit��ַ    
//    SPI1_ReadWriteByte((u8)((Dst_Addr)>>8));   
//    SPI1_ReadWriteByte((u8)Dst_Addr);  
//	AT45DB081B_CS=1;                            //ȡ��Ƭѡ     	      
//    AT45DB081B_Wait_Busy();   				   //�ȴ��������
//}  
////�ȴ�����
//void AT45DB081B_Wait_Busy(void)   
//{   
//	while((AT45DB081B_ReadSR()&0x01)==0x01);   // �ȴ�BUSYλ���
//}  
////�������ģʽ
//void AT45DB081B_PowerDown(void)   
//{ 
//  	AT45DB081B_CS=0;                            //ʹ������   
//    SPI1_ReadWriteByte(W25X_PowerDown);        //���͵�������  
//	AT45DB081B_CS=1;                            //ȡ��Ƭѡ     	      
//    delay_us(3);                               //�ȴ�TPD  
//}   
////����
//void AT45DB081B_WAKEUP(void)   
//{  
//  	AT45DB081B_CS=0;                            //ʹ������   
//    SPI1_ReadWriteByte(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
//	AT45DB081B_CS=1;                            //ȡ��Ƭѡ     	      
//    delay_us(3);                               //�ȴ�TRES1
//} 
