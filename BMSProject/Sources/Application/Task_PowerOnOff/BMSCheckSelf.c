/*=======================================================================
 *Subsystem:   ���
 *File:        BMSCheckSelf.C
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
#include  "BMSCheckSelf.h"  
#include  "WorkModeJudge.h"
#include  "BattInfoConfig.h"
#include  "FltLevcfg.h"
#include  "Task_DataProcess.h"
#include  "Task_InsulDetect.h"
#include  "Task_UpMonitor.h"
#include  "Init_Sys.h"
#include  "PIT.h"
#include  "Task_VoltTempCollect.h"

BMSCheckSelf_T  BMSCheckSelf;
/*=======================================================================
 *������:      Checkself_BattState() 
 *����:        BMS�Լ�ǰ�����ݲɼ�������
 *����:        time:PIT�ж�����       
 *���أ�       ��
 *˵����       ����ʱ�䵽��־λ��1��10msִ��һ��
========================================================================*/
static
void Checkself_BattState(uint16 time)
{
  switch(time)
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
      memset(&PIT_TimePeriod, 0x00, sizeof(PIT_TimePeriod_T));
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
  if(Temp>=NTEMP_BOUNDARY)//����(>=0��)
  { 
    return (g_BMSMonitor_Volt.Volt_Sys_Low2*1000);
  }
  else //����
  {
    return (g_BMSMonitor_New_LT.Voll_Sys_Low1_LT*1000);
  }
}
static
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
uint16 CheckSelf_CellVoltLow_Temp(uint8 Temp)
{
  if(Temp>=NTEMP_BOUNDARY)//����(>=0��)
  { 
    return (g_BMSMonitor_Volt.Volt_Cell_Low2);//�ɻ��ɱ궨����(�ֱ��ʣ�0.001V/λ)
  }
  else //����
  {
    return (g_BMSMonitor_New_LT.Volt_Cell_Low2_LT);//�ɻ��ɱ궨����(�ֱ��ʣ�0.001V/λ)
  }
}
static
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
static
uint8  CheckSelf_CellTempHigh_DisCharge(uint8 Temp)
{
   static uint8 cnt=0;
   if(Temp < g_BMSMonitor_Temp.Temp_DisCharge_High2)
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
static
uint8  CheckSelf_CellTempLow_DisCharge(uint8 Temp)
{
   static uint8 cnt=0;
   if(Temp > g_BMSMonitor_Temp.Temp_DisCharge_Low2)
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
static
uint8  CheckSelf_CurrentOver_DisCharge(float curr)
{
   static uint8 cnt=0;
   if(curr < (g_BMSMonitor_Curr.Current_DisCharge_High2*0.1-750))
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
static
uint8  CheckSelf_InsulDetect_DisCharge(uint8 insul)
{
   static uint8 cnt=0;
   if(insul > (g_BMSMonitor_Insul.Insulation_Resis2*0.1)) //0.1k��/V
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
static
uint8  CheckSelf_SysVoltHigh_Charge(uint32 totalvolt)
{
   static uint8 cnt=0;
   if(totalvolt < g_BMSMonitor_Volt.Volt_Sys_High2*1000)//��ֲʱע��ֱ��ʺ�uint16�Ƿ����
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
static
uint8  CheckSelf_CellVoltHigh_Charge(uint16 Voltmax)
{
   static uint8 cnt=0;
   if(Voltmax< g_BMSMonitor_Volt.Volt_Cell_High2)     //����ͬ��
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
static
uint8  CheckSelf_CellTempHigh_Charge(uint8 Temp)
{
   static uint8 cnt=0;
   if(Temp < g_BMSMonitor_Temp.Temp_Charge_High2)  //ע��ƫ����
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
static
uint8  CheckSelf_CellTempLow_Charge(uint8 Temp)
{
   static uint8 cnt=0;
   if(Temp > g_BMSMonitor_Temp.Temp_Charge_Low2) //ע��ƫ����
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
static
uint8  CheckSelf_InsulDetect_Charge(uint8 insul)
{
   static uint8 cnt=0;
   if(insul < g_BMSMonitor_Insul.Insulation_Resis2*0.1) 
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
static
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
static
uint8 CheckSelf_Discharge(void)
{
   uint8 state,temp;
   temp  = CheckSelf_SysVoltLow_DisCharge(g_DataColletInfo.SysVolt_Total, g_TempInfo.CellTemp_Ave);
   state = state|temp;
   temp  = CheckSelf_CellVoltLow_DisCharge(g_VoltInfo.CellVolt_Min, g_TempInfo.CellTemp_Ave);
   state = state|temp;
   temp  = CheckSelf_CellTempHigh_DisCharge(g_TempInfo.CellTemp_Max);
   state = state|temp;
   temp  = CheckSelf_CellTempLow_DisCharge(g_TempInfo.CellTemp_Min);
   state = state|temp;
   temp  = CheckSelf_CurrentOver_DisCharge(g_DataColletInfo.DataCollet_Current_Filter);
   state = state|temp;
   temp  = CheckSelf_InsulDetect_DisCharge(g_IsoDetect.insulation_resist);
   state = state|temp;
   temp  = CheckSelf_OpenWireDetect(g_OpenWireInfo.OpenWire_Status);
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
static
uint8 CheckSelf_Charge(void)
{
   uint8 state,temp;
   temp  = CheckSelf_SysVoltHigh_Charge(g_DataColletInfo.SysVolt_Total);
   state = state|temp;
   temp  = CheckSelf_CellVoltHigh_Charge(g_VoltInfo.CellVolt_Max);
   state = state|temp;
   temp  = CheckSelf_CellTempHigh_Charge(g_TempInfo.CellTemp_Max);
   state = state|temp;
   temp  = CheckSelf_CellTempLow_Charge(g_TempInfo.CellTemp_Min);
   state = state|temp;
   temp  = CheckSelf_InsulDetect_Charge(g_IsoDetect.insulation_resist);
   state = state|temp;
   temp  = CheckSelf_OpenWireDetect(g_OpenWireInfo.OpenWire_Status);
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
static
uint8 CheckSelf_Process(uint16 time, uint8 workmode)
{
   Checkself_BattState(time);//�ж�ʱ�����
   switch(workmode)
   {
     case MODE_DISCHARGE :
       if((CheckSelf_Discharge() == 0) && (g_SysInitState.AllInit_State == 0)) //�����ж����ʼ��״̬0:����
       {
          
          memset(&PIT_TimePeriod, 0x00, sizeof(PIT_TimePeriod_T));
          return 0;
       }
     break; 
     
     case MODE_CHARGE:
       if((CheckSelf_Charge() == 0) && (g_SysInitState.AllInit_State == 0)) //�����ж����ʼ��״̬0:����
       {
          memset(&PIT_TimePeriod, 0x00, sizeof(PIT_TimePeriod_T));
          return 0;
       }
     break;                                                                 
   }
   return 1; 
}

/*=======================================================================
 *������:      BMS_CheckSelf() 
 *����:        �Լ캯��
 *����:        time:����ʱ��
               mode:����ģʽ
 *���أ�       ��
 *˵����       BMS�Լ�����У�ֻҪBMS����״̬�ı���ô��֤�Լ�ֻ����1��
========================================================================*/
static
void BMS_CheckselfOnce(uint16 time, uint8 mode)
{
   static uint8 workmode = 1;
   if(workmode != mode)
   {
      while(CheckSelf_Process(time, g_WorkStateJudge.WorkState)!=0);
      workmode = mode; 
   }
}
/*=======================================================================
 *������:      Task_BMSWorkModeCheckself() 
 *����:        �Լ켰����״̬���ж�
 *����:        ��      
 *���أ�       ��
 *˵����       BMS�Լ�����У�ֻҪBMS����״̬�ı���ô��֤�Լ�ֻ����1��
========================================================================*/
void BMS_WorkModeCheckself(void)
{
  BMS_CheckselfOnce(PIT_TimePeriod.T500ms, g_WorkStateJudge.WorkState);
  g_WorkStateJudge.WorkState = WokeModeJudgment();
}

