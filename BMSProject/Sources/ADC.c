/*=======================================================================
 *Subsystem:   ���
 *File:        ADC.c
 *Author:      WenMing
 *Description: �ӿ�
               ��ѹ���PPAD0
               ������������PAD03
               ��������PAD10
               ����Ƶ�ʣ�2MHz
 ========================================================================
 * History:        // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸��߼��޸����ݼ���
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#include"TypeDefinition.h"
#include"MC9S12XEP100.h"
#include"derivative.h"
#include"ADC_Config.h"
#include"Filter_Function.h"
#include"ADC.h"

/*=======================================================================
 *������:      ADC_init
 *����:        ��ʼ��ADC
 *����:        ��         
                      
 *���أ�       ��
 *˵����       ADC ʱ��Ƶ�ʣ�2MHz
========================================================================*/
uint8 Init_ADC(void) 
{
  ATD0CTL2 = 0x40;  //����A/Dģ��,��������,��ֹ�ж�
  ATD0CTL1_SRES=2;  //ѡ��12λģ��ת��
  ATD0CTL3 = 0x88;   //ÿ��ֻת��һ��ͨ��
  ATD0CTL4 = 0xe1;   //ADģ��ʱ��Ƶ��Ϊ2MHz
  
  return(Init_Normal_ADC);
} 

/*=======================================================================
 *������:      ADC_Value()
 *����:        1·A/Dת����������ȡͨ��channel��ת�����
 *����:        channel��ͨ���ţ�0~15��         
 *���أ�       ADdata:channelͨ����ת����� 
 
 *˵����       ADC ʱ��Ƶ�ʣ�2MHz
========================================================================*/
uint16 ADC_Value(uint8 channel) 
{
  uint16 ADdata;
  ATD0CTL5 = channel;
  for(;;)
  {
     if((ATD0STAT0 & (1 << 7)) != 0)
     {          
        ADdata = ATD0DR0; 
        break;
     }
  }
  return ADdata;
}
/*=======================================================================
 *������:      ADC_Volt_Sys
 *����:        ��ذ��ܵ�ѹ�ļ��
 *����:        ��         
 *���أ�       Volt_Sys:channelͨ����ת�����
 
 *˵����       ADC ʱ��Ƶ�ʣ�2MHz
========================================================================*/
/*
float ADC_Volt_Sys(void) 
{
   uint16 Voltemp;
   float Volt_Sys;  
   Voltemp = ADC_Value(VoltageChannel9);  //�ǵ���ʾ
   Volt_Sys = (Voltemp / 4096.0) * Stand_Volt * Trans_Volt;  // Stand_Volt,Trans_VoltΪ�궨�壬��׼��ѹ����ѹת��
   return Volt_Sys;     
}  
*/

/*=======================================================================
 *������:      ADC_Current
 *����:        ��ذ����������������ļ��
 *����:        ��         
                      
 *���أ�       Current:����������ͨ����ת�����
 *˵����       ADC ʱ��Ƶ�ʣ�2MHz
========================================================================*/
float ADC_Current(void) 
{
  uint16 Currtemp;
  float Hall_voltage;
  Currtemp = ADC_Value(HallChannel); 
  Hall_voltage = (Currtemp/4095.0*Stand_Volt-2.5)*Hall_Range/2.0;  //���ݴ���������ת��
  return Hall_voltage;  
}
/*=======================================================================
 *������:      ADC_CC2_State
 *����:        ��ذ����������������ļ��
 *����:        ��         
                      
 *���أ�       Current:����������ͨ����ת�����
 *˵����       ADC ʱ��Ƶ�ʣ�2MHz
========================================================================*/
uint16 ADC_CC2_State(void) 
{
  uint16 cc2_state;
  cc2_state = ADC_Value(QCharge_CC2); 
  return cc2_state;  
}
