/*=======================================================================
 *Subsystem:   ���
 *File:        PIT.C
 *Author:      WENMING
 *Description: ͨ�ţ�
               �ӿڣ�
               �����ʣ�
 ========================================================================
 * History:    �޸���ʷ��¼�б�
 * 1. Date:
      Author:
      Modification:
========================================================================*/
#include  "PIT.h"
#include  "TypeDefinition.h"
#include  "MC9S12XEP100.h"
#include  "derivative.h"                 
 //��������ʵ��

 PIT_Time_10ms_T PIT_Time_10ms; 

//PITInit:��ʱ����ʼ������----------------------------------------------*
//��  ��:��ʱ����ʼ�����ж�һ��ʱ��Ϊ1/38��                             *
//��  ��:��                                                             *
//��  ��:��                                                             *
//----------------------------------------------------------------------*
/*  
void init_PIT0(void)
{
 
     PITMTLD0=249;     //Ϊ0ͨ��8λ��������ֵ
     PITLD0 = 1279;     //Ϊ0ͨ��16λ��������ֵ   //(249+1)*(1279+1)/32M=10ms
     PITMUX_PMUX0=0;   //��0ͨ��ʹ��΢������0
     PITCE_PCE0=1;     //��0ͨ������������ 
     PITCFLMT=0X80;    //ʹ��PITģ��
     PITINTE_PINTE0 = 0; //0ͨ����ʱ����ʱ�жϱ�ʹ��
}
  
void init_PIT1(void)
{
 
     PITMTLD1=249;     //Ϊ1ͨ��8λ��������ֵ
     PITLD1=63999;     //Ϊ1ͨ��16λ��������ֵ   //(249+1)*(63999+1)=16000000����������=0.5��
     PITMUX_PMUX1=1;   //��1ͨ��ʹ��΢������1
     PITCE_PCE1=1;     //��1ͨ������������ 
     PITCFLMT=0X80;    //ʹ��PITģ��
     PITINTE_PINTE1=1; //0ͨ����ʱ����ʱ�жϱ�ʹ��
}    
*/  
void PITInit(uint8 channel,uint8 MUXSEL,uint8 MTLD,uint16 LD)
{     
      if (channel >= 7)
      {
          channel = 7;
      }
      //��ֹPITģ��
      PITCFLMT_PITE = 0;
      // ʹ��PITͨ��channel
      PITCE |= 1<<channel;
      //��channelͨ��ʹ��΢������MUXSEL,��Ҫ���ֵ�����Ӧ��΢��ʱ���ؼĴ�����
      if (MUXSEL == 0) 
      {
        PITMUX &= ~(1<<channel);
        PITMTLD0 = MTLD;
      }
      else 
      {
        PITMUX |= 1<<channel;
        PITMTLD1 = MTLD;
      }
      // ��ʱ��һ���ж�ʱ�� = (PITMTLD + 1) * (PITLD + 1) / fBUS
      //                    =(249+1)*(63999+1)/32MHz=0.5s
      switch (channel)  
      {
        case 0:                                               
          PITLD0=LD;
          break;
        case 1:
          PITLD1=LD;
          break;
        case 2:
          PITLD2=LD;
          break;
        case 3:
          PITLD3=LD;
          break;
        case 4:
          PITLD4=LD;
          break;
        case 5:
          PITLD5=LD;
          break;
        case 6:
          PITLD6=LD;
          break;
        case 7:
          PITLD7=LD;
          break;
        default:
          break;    
      }
      // ʹ��PITģ�� 
      PITCFLMT_PITE = 1;
      // ��ͨ��0�����־,�����µļ�ʱʱ��
      PITTF|=1<<channel;
      // ��ֹPITͨ��0�ж� 
      PITINTE &= ~(1<<channel);       
}
    


uint8 Init_PIT(uint8 channel, uint8 perio)
{
  if(channel>7)
  {
    return (Init_Fault_PIT_Channel);
  }
  if((perio!=1)||(perio!=5)||(perio!=10))
  {
    return Init_Fault_PIT_Period;
  }
  switch (perio)
  {
    case 1:
      PITInit(channel, 1, 249, 129);
    break;
    
    case 5:
      PITInit(channel, 1, 249, 639);
    break;
    
    case 10:
      PITInit(channel, 1, 249, 1279);
    break;
      
  }
  return (Init_Normal_PIT);
}

#pragma CODE_SEG __NEAR_SEG NON_BANKED
//��ʱ�жϣ��ж�����Ϊ10ms
interrupt void isrPIT0()               //PIT0��ʱ�жϺ�����
{  
   if (PITTF_PTF0 ==1) 
   {   
     PITTF_PTF0 = 1;
   }
   
   //Task_PITCNT.Runtime_flag++;
         
   PIT_Time_10ms.Period_10ms++;                    
   PIT_Time_10ms.Period_50ms++;
   PIT_Time_10ms.Period_100ms++;
   PIT_Time_10ms.Period_200ms++;
   PIT_Time_10ms.Period_250ms++;
   PIT_Time_10ms.Period_500ms++;
   PIT_Time_10ms.Period_1s++;
   //Task_roll();
}
#pragma CODE_SEG DEFAULT


