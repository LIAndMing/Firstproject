  #include  "CAN.h"
  #include  "MC9S12XEP100.h"
  #include  "derivative.h"
    
/*=======================================================================
 *������:      INIT_CAN2(void)
 *����:        ��ʼ��CAN2
 *����:        
               sysclk��ʱ������ʱ��:32MHz
               baud:    
 *���أ�       ��
 *˵����       
========================================================================*/
uint8 INIT_CAN2(uint16 Baud_Rate) 
{
  uint8 Cnt[3];
  
  if((Baud_Rate != 125)||(Baud_Rate != 250)||(Baud_Rate != 500))
  {
    return(Init_Fault_CAN_BaudRate);
  }
  
  if(CAN2CTL0_INITRQ==0)      // ��ѯ�Ƿ�����ʼ��״̬   
  {
    CAN2CTL0_INITRQ =1;        // �����ʼ��״̬
  }
  else
  {
    return (Init_Fault_CAN_EnterState);
  }
  
  do
  {
    if(++Cnt[0]>=200)
    {
      Cnt[0] = 0;
      return(Init_Fault_CAN_Unready);
    }
  }
  while (CAN2CTL1_INITAK==0);  //�ȴ������ʼ��״̬
  Cnt[0] = 0;

  CAN2BTR0_SJW = 0;            //����ͬ��
  
  switch(Baud_Rate)
  {
    case 500:
      CAN2BTR0_BRP = 3;            //����Ԥ��Ƶֵ 
      CAN2BTR1 = 0x1c;     //����ʱ��1��ʱ��2��Tq���� 
      break;
    
    case 250:
      CAN2BTR0_BRP = 7;            //����Ԥ��Ƶֵ  
      CAN2BTR1 = 0x1c;     //����ʱ��1��ʱ��2��Tq���� 
      break;
      
    case 125:
      CAN2BTR0_BRP = 15;            //����Ԥ��Ƶֵ  
      CAN2BTR1 = 0x1c;     //����ʱ��1��ʱ��2��Tq���� 
      break;
  }
 
//�ر��˲���                                  
  CAN2IDMR0 = 0xFF;
  CAN2IDMR1 = 0xFF;
  CAN2IDMR2 = 0xFF;
  CAN2IDMR3 = 0xFF;
  CAN2IDMR4 = 0xFF;
  CAN2IDMR5 = 0xFF;
  CAN2IDMR6 = 0xFF;
  CAN2IDMR7 = 0xFF; 

  CAN2CTL1 = 0xC0;             //ʹ��MSCANģ��,����Ϊһ������ģʽ��ʹ������ʱ��Դ 

  CAN2CTL0 = 0x00;             //����һ��ģʽ����

  do
  {
    if(++Cnt[1]>=200)
    {
      Cnt[1] = 0;
      return(Init_Fault_CAN_Unready);
    }
  }
  while(CAN2CTL1_INITAK);      //�ȴ��ص�һ������ģʽ
  Cnt[1] = 0;
  
  do
  {
    if(++Cnt[2]>=200)
    {
      Cnt[2] = 0;
      return(Init_Fault_CAN_Unready);
    }
  }
  while(CAN2CTL0_SYNCH==0);    //�ȴ�����ʱ��ͬ��
  Cnt[2] = 0;
  
  CAN2RIER_RXFIE = 1;          //ʹ�ܽ����ж�
  
  return (Init_Normal_CAN);
}

/*=======================================================================
 *������:      SendMesg_CAN2
 *����:        CAN2��������
 *����:        ��չ֡
               sysclk��ʱ������ʱ��:32MHz
               baud:   
 *���أ�       ��
 *˵����       
========================================================================*/ 
uint8 SendMsg_CAN2(pCANFRAME sendFrame)
{
  uint8 send_buf,i;
  uint8 Cnt[1];
  
  // ������ݳ���
  if(sendFrame->m_dataLen > 8)
    return (SendMsg_Fault_Lenth);

  // �������ʱ��
  if(CAN2CTL0_SYNCH==0)
    return (SendMsg_Fault_Synch);

  send_buf = 0;
  do
  {
    // Ѱ�ҿ��еĻ�����
    CAN2TBSEL=CAN2TFLG;
    send_buf=CAN2TBSEL;
    if(++Cnt[0]>=200)
    {
      Cnt[0] = 0;
      return(SendMsg_Fault_NoEmptyNode);
    }
  } 
  while(!send_buf); 
  //д���ʶ��ID
  
  if (sendFrame->m_IDE == 0)  //����׼֡���ID
  {
    CAN2TXIDR0 = (uint8)(sendFrame->m_ID>>3);
    CAN2TXIDR1 = (uint8)(sendFrame->m_ID<<5);
  } 
  else  //����չ֡���ID
  {
    CAN2TXIDR0 = (uint8)(sendFrame->m_ID>>21);
    CAN2TXIDR1 = (((uint8)(sendFrame->m_ID>>13)) & 0xe0)|0x18|(((uint8)(sendFrame->m_ID>>15)) &0x07);
    CAN2TXIDR2 = (uint8)(sendFrame->m_ID>>7);
    CAN2TXIDR3 = (uint8)(sendFrame->m_ID<<1);
  }
  
  if(sendFrame->m_RTR==1)
  {     
      CAN2TXIDR1 |= 0x10;
  }
      
  for (i=0;i<sendFrame->m_dataLen;++i)  
  {
    *((&CAN2TXDSR0) + i) = sendFrame->m_data[i];
  } 
      
  // д�����ݳ���
  CAN2TXDLR = sendFrame->m_dataLen;
  
  // д�����ȼ�
  CAN2TXTBPR = sendFrame->m_priority;
  
  // �� TXx ��־ (������׼������)
  CAN2TFLG = send_buf;
  
  return(SendMsg_Normal);
  
}

/*=======================================================================
 *������:      GetMsg_CAN2(pCANFRAME receiveFrame)
 *����:        CAN2��������
 *����:        ��չ֡
               sysclk��ʱ������ʱ��:32MHz
               baud:  
 *���أ�       ��
 *˵����       
========================================================================*/ 
uint8 GetMsg_CAN2(pCANFRAME receiveFrame)
{
  
  uint8 i;
	  
	if (!(CAN2RFLG_RXF))                         // �����ձ�־λ��
		return (GetMsg_Fault_RFLG_RXF);
	 
	if (CAN2RXIDR1_IDE == 0)                     // �յ���׼֡��
	{
		receiveFrame->m_ID = (uint32)(CAN2RXIDR0<<3) | (uint32)(CAN2RXIDR1>>5);
		receiveFrame->m_RTR = (CAN2RXIDR1>>4) & 0x01;
    receiveFrame->m_IDE = 0;
	} 
	else                                         // �յ���չ֡��
	{
	 	receiveFrame->m_ID = (((uint32)CAN2RXIDR0)<<21)|((uint32)(CAN2RXIDR1&0xe0)<<13)|((uint32)(CAN2RXIDR1&0x07)<<15)|(((uint32)CAN2RXIDR2)<<7);
    receiveFrame->m_ID = receiveFrame->m_ID | ((uint32)(CAN2RXIDR3&0xfe)>>1);
	}
	 
  receiveFrame->m_dataLen = CAN2RXDLR&0X0F;
  
  for (i=0;i<receiveFrame->m_dataLen;i++)       // ��ȡ����
  {
	   receiveFrame->m_data[i] = *(&(CAN2RXDSR0)+i);
  }   

  CAN2RFLG_RXF = 1;

  return(GetMsg_Normal);
}


#pragma CODE_SEG __NEAR_SEG NON_BANKED
//CAN2������λ������
interrupt void Monitor_CAN2()            
{   
  uint8 cnt_fromCSSU = 0; 
  CANFRAME mgGet;
  //Task_Flag_Time.CAN2++;  
  if (GetMsg_CAN2(&mgGet))
  {   
      if(cnt_fromCSSU < 100) //��֤������ȫ���ղŽ������ݴ���
      {
      
          cnt_fromCSSU++;
          if(cnt_fromCSSU == 100)
          {
              PITINTE_PINTE0 = 1; //��������ȫ������֮������ʱ�ж�
          }
      }  
  }    
} 
#pragma CODE_SEG DEFAULT

















