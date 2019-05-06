
/*

//分步测试  测试TX/RX存储器
	//printf("%d : Sn_MR=0x%04x,Sn_PORTR=0x%04x(%d),Sn_SSR=%04x\r\n",0,IINCHIP_READ(Sn_MR(0)),IINCHIP_READ(Sn_PORTR(0)),IINCHIP_READ(Sn_PORTR(0)),getSn_SSR(0));//查询ssr初始状态
	/////////////////////////////////////////////////////////	
	IINCHIP_WRITE(W5300_MR,MR_MT); //允许测试内部存储器，MT测试完成后需置0
	socket(0,Sn_MR_TCP,5000,0);       // open the SOCKET0  
	socket(1,Sn_MR_TCP,5000,0);       // open the SOCKET1  
	socket(2,Sn_MR_TCP,5000,0);       // open the SOCKET2 
	for(i=0;i<8192/2;i++)
	{
	
		setSn_TX_FIFOR(0,test_data);
		test_data=test_data+0x1;
		//printf("test_data=0x%04x",test_data);
		//delay_ms(10);
		//IINCHIP_WRITE(Sn_TX_FIFOR(s),data);
	
	}
	
		test_data=0x0000;
		for(i=0;i<8192/2;i++)
	{
	
		setSn_TX_FIFOR(1,test_data);
		test_data=test_data+0x2;
		//printf("test_data=0x%04x",test_data);	

	}
		test_data=0x0000;
		for(i=0;i<8192/2;i++)
	{

		setSn_TX_FIFOR(2,test_data);
		test_data=test_data+0x3;
		//printf("test_data=0x%04x",test_data);	
	}
	
	for(i=0;i<8192/2;i++)
	{
	
		read_data=IINCHIP_READ(Sn_TX_FIFOR(0));
		//if(read_data!=test_data) 
		//{
		//	read_data=test_data+0x0101;
		//}
		printf("read_data=0x%04x\r\n",read_data);
		//delay_ms(10);
	}
//////////////////////////////////////////
	
*/