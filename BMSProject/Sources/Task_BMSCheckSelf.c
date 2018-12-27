/*=======================================================================
 *Subsystem:   ���
 *File:        Task_BMSCheckSelf.C
 *Author:      WenYuhao
 *Description: ͨ�ţ�
               �ӿڣ�
               �����ʣ�
               ֻ���Լ�ɹ�����ܱպ���Ӧ�Ŀ���
/* ========================================================================
 * History:    �޸���ʷ��¼�б�
 * 1. Date:    
      Author:  
      Modification: 
========================================================================*/
#include "Task_BMSCheckSelf.h"  

BMSCheckSelf_T  BMSCheckSelf;
/*=======================================================================
 *������:      Checkself_BattState() 
 *����:        BMS�Լ�ǰ�����ݲɼ�������
 *����:        time:PIT�ж�����       
 *���أ�       ��
 *˵����       ����ʱ�䵽��־λ��1��10msִ��һ��
========================================================================*/
void Checkself_BattState(uint16 *time)
{
  switch(*time)
  {
    case 1:
      Task_OpenWireDetect();          //���߿�·
    break;  
    
    case 6:
      Task_VoltCMDSend();             //��ѹ�Ĵ�������
    break;
    
    case 8:
      Task_VoltCollect();             //��ѹ��ȡ����
    break;
    
    case 10:
      Task_TempCMDSend();             //�¶����㺯��
    break;
    
    case 12:
      Task_TempCollect();             //�¶Ȳɼ�����
    break;

    case 14:
      Task_InsulationDetect();        //��Ե����ж�
      *time = 0;
    break;
  }
}

/*=======================================================================
 *������:      �ŵ��Լ��ж��� 
 *����:        ��ʼ��������Լ�,�Լ�ֻҪһ��ͨ����պϼ̵���
 *����:        ���ո�������       
 *���أ�       ���ո�������
 *˵����       
========================================================================*/
/************************�ŵ�ϵͳ��ѹ����********************************/
static
uint32 CheckSelf_SysVoltLow_DischagTemp(uint8 Temp)
{
  if(Temp>=0+40)//����(>=0��)
  { 
    return ((uint32)(2850)*(SYS_SERIES_YiDongLi));//�ɻ��ɱ궨����(�ֱ��ʣ�0.001V/λ)
  }
  else //����
  {
    return ((uint32)(2700)*(SYS_SERIES_YiDongLi));//�ɻ��ɱ궨����(�ֱ��ʣ�0.001V/λ)
  }
}
uint8  CheckSelf_SysVoltLow_DisCharge(uint32 totalvolt, uint8 temp)
{
   static uint8 cnt=0;
   if(totalvolt > CheckSelf_SysVoltLow_DischagTemp(temp))
   {
      cnt = 0;
      return 0;  //�Լ�ɹ�
      
   }
   else
   {
      if(cnt > 2)
      {
        cnt = 2;
      }
      if(++cnt>2)
      {
        return 1; //�Լ����
      }
      
   }
   return 2;
}

/************************�ŵ絥���ѹ����********************************/
static
uint32 CheckSelf_CellVoltLow_Temp(uint8 Temp)
{
  if(Temp>=0+40)//����(>=0��)
  { 
    return ((uint32)(2800));//�ɻ��ɱ궨����(�ֱ��ʣ�0.001V/λ)
  }
  else //����
  {
    return ((uint32)(2500));//�ɻ��ɱ궨����(�ֱ��ʣ�0.001V/λ)
  }
}

uint8  CheckSelf_CellVoltLow_DisCharge(uint16 Voltmin, uint8 temp)
{
   static uint8 cnt=0;
   if(Voltmin > CheckSelf_CellVoltLow_Temp(temp))
   {
      cnt = 0;
      return 0;  //�Լ�ɹ�
   }
   else
   {
      if(cnt > 2)
      {
        cnt = 2;
      }
      if(++cnt>2)
      {
        return 1; //�Լ����
      }
      
   }
   return 2;
}

/************************�ŵ絥���¶ȹ���********************************/
uint8  CheckSelf_CellTempHigh_DisCharge(uint8 Temp)
{
   static uint8 cnt=0;
   if(Temp < 60+40)
   {
      cnt = 0;
      return 0;  //�Լ�ɹ�
   }
   else
   {
      if(cnt > 2)
      {
        cnt = 2;
      }
      if(++cnt>2)
      {
        return 1; //�Լ����
      }
      
   }
   return 2;
}

/************************�ŵ絥���¶ȹ���********************************/
uint8  CheckSelf_CellTempLow_DisCharge(uint8 Temp)
{
   static uint8 cnt=0;
   if(Temp > (-30+40))
   {
      cnt = 0;
      return 0;  //�Լ�ɹ�
   }
   else
   {
      if(cnt > 2)
      {
        cnt = 2;
      }
      if(++cnt>2)
      {
        return 1; //�Լ����
      }
      
   }
   return 2;
}

/************************�ŵ����������********************************/
uint8  CheckSelf_CurrentOver_DisCharge(float curr)
{
   static uint8 cnt=0;
   if(curr > 650)
   {
      cnt = 0;
      return 0;  //�Լ�ɹ�
   }
   else
   {
      if(cnt > 2)
      {
        cnt = 2;
      }
      if(++cnt>2)
      {
        return 1;//�Լ����
      }
      
   }
   return 2;
}

/************************�ŵ��Ե����********************************/
uint8  CheckSelf_InsulDetect_DisCharge(uint8 insul)
{
   static uint8 cnt=0;
   if(insul < 0.1*SYS_SERIES_YiDongLi*3.65) //0.1k��/V
   {
      cnt = 0;
      return 0;  //�Լ�ɹ�
   }
   else
   {
      if(cnt > 2)
      {
        cnt = 2;
      }
      if(++cnt>2)
      {
        return 1;//�Լ����
      }
      
   }
   return 2;
}

/*=======================================================================
 *������:      ����Լ��ж��� 
 *����:        ��ʼ��������Լ�,�Լ�ֻҪһ��ͨ����պϼ̵���
 *����:        ���ո�������       
 *���أ�       ���ո�������
 *˵����       
========================================================================*/
/************************���ϵͳ��ѹ����********************************/
uint8  CheckSelf_SysVoltHigh_Charge(uint32 totalvolt)
{
   static uint8 cnt=0;
   if(totalvolt < 37000*SYS_SERIES_YiDongLi)//��ֲʱע��ֱ��ʺ�uint16�Ƿ����
   {
      cnt = 0;
      return 0;  //�Լ�ɹ�
      
   }
   else
   {
      if(cnt > 2)
      {
        cnt = 2;
      }
      if(++cnt>2)
      {
        return 1;//�Լ����
      }
      
   }
   return 2;
}
/************************��絥���ѹ����********************************/
uint8  CheckSelf_CellVoltHigh_Charge(uint16 Voltmax)
{
   static uint8 cnt=0;
   if(Voltmax<37000)     //����ͬ��
   {
      cnt = 0;
      return 0;  //�Լ�ɹ�
   }
   else
   {
      if(cnt > 2)
      {
        cnt = 2;
      }
      if(++cnt>2)
      {
        return 1;//�Լ����
      }
      
   }
   return 2;
}
/********************************************************************/
uint8  CheckSelf_CellTempHigh_Charge(uint8 Temp)
{
   static uint8 cnt=0;
   if(Temp < 55+40)  //ע��ƫ����
   {
      cnt = 0;
      return 0;  //�Լ�ɹ�
   }
   else
   {
      if(cnt > 2)
      {
        cnt = 2;
      }
      if(++cnt>2)
      {
        return 1;//�Լ����
      }
   }
   return 2;
}
/********************************************************************/
uint8  CheckSelf_CellTempLow_Charge(uint8 Temp)
{
   static uint8 cnt=0;
   if(Temp > (-5+40)) //ע��ƫ����
   {
      cnt = 0;
      return 0;  //�Լ�ɹ�
   }
   else
   {
      if(cnt > 2)
      {
        cnt = 2;
      }
      if(++cnt>2)
      {
        return 1;
        //�Լ����
      }
      
   }
   return 2;
}

/********************************************************************/
uint8  CheckSelf_InsulDetect_Charge(uint8 insul)
{
   static uint8 cnt=0;
   if(insul < 0.1*SYS_SERIES_YiDongLi*3.65) //0.1k��/V
   {
      cnt = 0;
      return 0;  //�Լ�ɹ�
   }
   else
   {
      if(cnt > 2)
      {
        cnt = 2;
      }
      if(++cnt>2)
      {
        return 1;//�Լ����
      }
      
   }
   return 2;
}
/*=======================================================================
 *������:      CheckSelf_OpenWireDetect 
 *����:        ��Ե����״̬�ж�
 *����:        state:��Ե�������״̬       
 *���أ�       uint8:�Ƿ���ھ�Ե����,0:����;1:����
 *˵����       ��ʼ����Ϊ�Լ�ʹ��
========================================================================*/
uint8  CheckSelf_OpenWireDetect(uint8 state)
{
  if(state == 0)
  {
     return 0;  //�Լ�ɹ�
  }
  return 1;
}

/*=======================================================================
 *������:      CheckSelf_Discharge() 
 *����:        �ŵ�ʱ�Լ��״̬�ж�
 *����:        ��      
 *���أ�       uint8:�Ƿ�����Լ����,0:����;1:����
 *˵����       ��ʼ����Ϊ�Լ�ʹ��
========================================================================*/
uint8 CheckSelf_Discharge(void)
{
   uint8 state,temp;
   temp  = CheckSelf_SysVoltLow_DisCharge(DataColletInfo.SysVolt_Total, TempInfo.CellTemp_Ave);
   state = state|temp;
   temp  = CheckSelf_CellVoltLow_DisCharge(VoltInfo.CellVolt_Min, TempInfo.CellTemp_Ave);
   state = state|temp;
   temp  = CheckSelf_CellTempHigh_DisCharge(TempInfo.CellTemp_Max);
   state = state|temp;
   temp  = CheckSelf_CellTempLow_DisCharge(TempInfo.CellTemp_Min);
   state = state|temp;
   temp  = CheckSelf_CurrentOver_DisCharge(DataColletInfo.DataCollet_Current_Filter);
   state = state|temp;
   temp  = CheckSelf_InsulDetect_DisCharge(IsoDetect.insulation_resist);
   state = state|temp;
   temp  = CheckSelf_OpenWireDetect(OpenWireInfo.OpenWire_Status);
   state = state|temp;
   return state;
}

/*=======================================================================
 *������:      CheckSelf_Charge() 
 *����:        ���ʱ�Լ��״̬�ж�
 *����:        ��      
 *���أ�       uint8:�Ƿ�����Լ����,0:����;1:����
 *˵����       ��ʼ����Ϊ�Լ�ʹ��
========================================================================*/
uint8 CheckSelf_Charge(void)
{
   uint8 state,temp;
   temp  = CheckSelf_SysVoltHigh_Charge(DataColletInfo.SysVolt_Total);
   state = state|temp;
   temp  = CheckSelf_CellVoltHigh_Charge(VoltInfo.CellVolt_Max);
   state = state|temp;
   temp  = CheckSelf_CellTempHigh_Charge(TempInfo.CellTemp_Max);
   state = state|temp;
   temp  = CheckSelf_CellTempLow_Charge(TempInfo.CellTemp_Min);
   state = state|temp;
   temp  = CheckSelf_InsulDetect_Charge(IsoDetect.insulation_resist);
   state = state|temp;
   temp  = CheckSelf_OpenWireDetect(OpenWireInfo.OpenWire_Status);
   state = state|temp;
   return state;
}
/*=======================================================================
 *������:      BMS_CheckSelf() 
 *����:        �Լ��ܺ���
 *����:        ��      
 *���أ�       uint8:�Ƿ�����Լ����,0:����;1:����
 *˵����       ��ʼ����,���Լ�ɹ���ִ������,����һֱ�Լ�
========================================================================*/
uint8 BMS_CheckSelf(uint16 *time, uint8 workmode)
{
   Checkself_BattState(time);//�ж�ʱ�����
   switch(workmode)
   {
     case MODE_DISCHARGE :
       if((CheckSelf_Discharge() == 0) && (System_InitState.AllInit_State == 0)) //�����ж����ʼ��״̬0:����
       {
          
          *time = 0;//ʱ���Ϊ0,���Խ��к�������ִ��
          return 0;
       }
     break; 
     
     case MODE_CHARGE:
       if((CheckSelf_Charge() == 0) && (System_InitState.AllInit_State == 0)) //�����ж����ʼ��״̬0:����
       {
          *time = 0;//ʱ���Ϊ0,���Խ��к�������ִ��
          return 0;
       }
     break;                                                                 
   }
   return 1; 
}

