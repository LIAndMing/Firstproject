/*=======================================================================
 *Subsystem:   ���
 *File:        Task_FaultLevelJudge.c
 *Author:      Wenming
 *Description: ͨ�ţ�
               �ӿڣ�  CAN2
               �����ʣ�250k
               ���ԣ�
 ========================================================================
 * History:    �޸���ʷ��¼�б��޸ļ�¼�����޸����ڡ��޸��߼��޸�����
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#include  "TypeDefinition.h"
#include  "BattInfoConfig.h"
#include  "Task_FaultLevelJudge.h"
#include  "FaultLevelConfig.h"        
#include  "Roll_Task.h"  
#include  "BattInfoParaStructure.h" 
#include  "Task_DataProcess.h"                                  //�����޸�
#include  "WorkModeJudge.h"
/*=======================================================================
                                  ����
========================================================================*/ 
Fault_DisCharge_T Fault_DisCharge; 
Fault_Charge_T    Fault_Charge;

VoltInfo_T VoltInfo;
TempInfo_T TempInfo;
State_Offline_T State_Offline;

FltThreshold_DisChg_T FltThreshold_DisChg;
FltThreshold_Charge_T FltThreshold_Charge;

 //�ŵ�����ж�
 static uint8 Fault_DisChg_VoltS(uint32 Volt,uint8 Temp); 
 static uint8 Fault_DisChg_VoltC(uint16 Volt,uint8 Temp);
 static uint8 Fault_DisChg_VoltCD(uint16 V_max,uint16 V_min);
 static uint8 Fault_DisChg_TempH(uint8 Temp);
 static uint8 Fault_DisChg_TempL(uint8 Temp);
 static uint8 Fault_DisChg_TempD(uint8 T_max,uint8 T_min);
 static uint8 Fault_DisChg_CurrH(float Current);
 //�������ж�
 static uint8 Fault_VoltSys_Charge(uint32 Volt);
 static uint8 Fault_VoltCell_High_Charge(uint16 Volt);
 static uint8 Fault_VoltCell_Low_Charge(uint16 Volt,uint8 Temp);
 static uint8 Fault_VoltCell_Diff_Charge(uint16 V_max,uint16 V_min);
 static uint8 Fault_Temp_High_Charge(uint8 Temp);
 static uint8 Fault_Temp_Low_Charge(uint8 Temp);
 static uint8 Fault_Temp_Diff_Charge(uint8 T_max,uint8 T_min);
 static uint8 Fault_Current_Charge(float Current);

/*=======================================================================
 *������1:     FltThresholdGet(void)
 *����:        ��ȡ������ֵ
 *����:        ��    
 *���أ�       ��
 
 *˵����       
========================================================================*/ 
/*static
void FltThresholdGet(void)
{
  //�ŵ�
  FltThreshold_DisChg.F2_DisChg_VoltSL_NT = ;
  FltThreshold_DisChg.F1_DisChg_VoltSL_NT = ;
  FltThreshold_DisChg.F2_DisChg_VoltSL_LT = ;
  FltThreshold_DisChg.F1_DisChg_VoltSL_LT = ;
  
  FltThreshold_DisChg.F2_DisChg_VoltCL_NT = ;
  FltThreshold_DisChg.F1_DisChg_VoltCL_NT = ;
  FltThreshold_DisChg.F2_DisChg_VoltCL_LT = ;
  FltThreshold_DisChg.F1_DisChg_VoltCL_LT = ;
  
  FltThreshold_DisChg.F1_DisChg_VoltCD = ;
  
  FltThreshold_DisChg.F2_DisChg_TempH = ;
  FltThreshold_DisChg.F1_DisChg_TempH = ;
  
  FltThreshold_DisChg.F2_DisChg_TempL = ;
  FltThreshold_DisChg.F1_DisChg_TempL = ;
  
  FltThreshold_DisChg.F1_DisChg_TempD = ;
  
  FltThreshold_DisChg.F2_DisChg_CurrH = ;
  FltThreshold_DisChg.F1_DisChg_CurrH = ;
  
  FltThreshold_DisChg.F2_DisChg_Insul = ;
//���
  FltThreshold_Charge.F2_Charge_VoltSL = ;
  FltThreshold_Charge.F1_Charge_VoltSL = ;
  
  FltThreshold_Charge.F2_Charge_VoltCL = ;
  FltThreshold_Charge.F1_Charge_VoltCL = ;
  
  FltThreshold_Charge.F1_Charge_VoltCD = ;
  
  FltThreshold_Charge.F2_Charge_TempH = ;
  FltThreshold_Charge.F1_Charge_TempH = ;
  
  FltThreshold_Charge.F2_Charge_TempL = ;
  FltThreshold_Charge.F1_Charge_TempL = ;
  
  FltThreshold_Charge.F1_Charge_TempD = ;
  
  FltThreshold_Charge.F2_Charge_CurrH = ;
  FltThreshold_Charge.F1_Charge_CurrH = ;
  
  FltThreshold_Charge.F2_Charge_Insul = ;
} */

/*=======================================================================
                              �����жϺ���0x00
 ========================================================================
 *������1:     Task_FaultLevelJudge(uint8 workstate)
 *����:        ����ϵͳ��ѹ���¶ȡ������жϹ��ϵȼ�
 *����:        ��    
 *���أ�       ��
 
 *˵����       
========================================================================*/ 
void Task_FaultLevelJudge(uint8 workstate)
{
   switch(workstate)
   {
    case MODE_DISCHARGE: //�ŵ�״̬
      //������״̬����
      Fault_Charge.Level_Charge_SwitchOff_flag = 0; 
      //�жϷŵ�״̬����
      Fault_DisCharge.Level_Volt_Sys_Low           = Fault_DisChg_VoltS(DataColletInfo.SysVolt_Total, TempInfo.CellTemp_Ave);
      Fault_DisCharge.Level_Volt_Cell_Low          = Fault_DisChg_VoltC(VoltInfo.CellVolt_Min, TempInfo.CellTemp_Ave);
      Fault_DisCharge.Level_Volt_Cell_Diff_High    = Fault_DisChg_VoltCD(VoltInfo.CellVolt_Max, VoltInfo.CellVolt_Min);
      Fault_DisCharge.Level_Temp_High              = Fault_DisChg_TempH(TempInfo.CellTemp_Max);
      Fault_DisCharge.Level_Temp_Low               = Fault_DisChg_TempL(TempInfo.CellTemp_Min);
      Fault_DisCharge.Level_Temp_Diff_High         = Fault_DisChg_TempD(TempInfo.CellTemp_Max, TempInfo.CellTemp_Min);
      Fault_DisCharge.Level_Current_DisCharge_High = Fault_DisChg_CurrH(DataColletInfo.DataCollet_Current_Filter);
      
      //�Ͽ��̵����Ķ������ϱ��
      if((Fault_DisCharge.Level_Volt_Sys_Low==2) || \
         (Fault_DisCharge.Level_Volt_Cell_Low == 2)||\
         (Fault_DisCharge.Level_Temp_High == 2)||\
         (Fault_DisCharge.Level_Temp_Low == 2) ||\
         (Fault_DisCharge.Level_Current_DisCharge_High == 2))
      {
        Fault_DisCharge.Level_DisCharge_SwitchOff_flag = 1;  
      }
      
    break;
    
    case MODE_CHARGE:   //���״̬
      //����ŵ�״̬����
      Fault_DisCharge.Level_Volt_Sys_Low           = 0;
      Fault_DisCharge.Level_Volt_Cell_Low          = 0;
      Fault_DisCharge.Level_DisCharge_SwitchOff_flag = 0;
      //�жϳ��״̬����
      Fault_Charge.Level_Volt_Sys_High        = Fault_VoltSys_Charge(DataColletInfo.SysVolt_Total);
      Fault_Charge.Level_Volt_Cell_High       = Fault_VoltCell_High_Charge(VoltInfo.CellVolt_Max);
      Fault_Charge.Level_Volt_Cell_Diff_High  = Fault_VoltCell_Diff_Charge(VoltInfo.CellVolt_Max, VoltInfo.CellVolt_Min);
      Fault_Charge.Level_Temp_High            = Fault_Temp_High_Charge(TempInfo.CellTemp_Max);
      Fault_Charge.Level_Temp_Low             = Fault_Temp_Low_Charge(TempInfo.CellTemp_Min);
      Fault_Charge.Level_Temp_Diff_High       = Fault_Temp_Diff_Charge(TempInfo.CellTemp_Max, TempInfo.CellTemp_Min);
      Fault_Charge.Level_Current_Charge_High  = Fault_Current_Charge(DataColletInfo.DataCollet_Current_Filter);
      
      //�Ͽ��̵����Ķ������ϱ��
      if((Fault_Charge.Level_Volt_Sys_High==2) || \
         (Fault_Charge.Level_Volt_Cell_High==2) || \
         (Fault_Charge.Level_Volt_Cell_Low == 2)||\
         (Fault_Charge.Level_Temp_High == 2)||\
         (Fault_Charge.Level_Temp_Low == 2))
      {
        Fault_Charge.Level_Charge_SwitchOff_flag = 1;  
      }
    break;
   }
}
/*============================�����жϺ���===============================*/

/*=======================================================================
                              �ŵ���ѹ��0x01
 ========================================================================
 *������1:     Fault1_VoltSys_DisCharge(uint8 Temp)
 *����:        �����¶�ѡ��ŵ���ѹ1��������ֵ
 *����:        Temp (�����¶�  ����ƫ����:-40)       
 *���أ�       �ȼ�1��ѹ��ֵ 
 
 *������2:     Fault2_VoltSys_DisCharge(uint8 Temp)
 *����:        �����¶�ѡ��ŵ���ѹ2��������ֵ
 *����:        Temp (�����¶�  ����ƫ����:-40)       
 *���أ�       �ȼ�2��ѹ��ֵ 
 
 *������3:     Recover1_VoltSys_DisCharge(uint8 Temp)
 *����:        �����¶�ѡ��ŵ���ѹ1���ָ�������ֵ
 *����:        Temp (�����¶�  ����ƫ����:-40)       
 *���أ�       �ȼ�1��ѹ��ֵ 
 
 *������4:     Fault_DisChg_VoltS(uint32 Volt,uint8 Temp)
 *����:        ����ϵͳ��ѹ���¶��жϹ��ϵȼ�
 *����:        Volt(ϵͳ��ѹ)
               Temp (�����¶�  ����ƫ����:-40)       
 *���أ�       �ŵ���ѹ�͵Ĺ��ϵȼ�
 
 *˵����       
========================================================================*/ 
//-------------------�����¶�ȷ�����ϵȼ��Լ��ָ��ȼ���ֵ-----------------
//1�����Ϻ���
static
uint32 Fault1_VoltSys_DisCharge(uint8 Temp)
{
  if(Temp>=NTEMP_BOUNDARY)//����(>=0��)
  { 
    return (F1_DISCHG_VOLTSL_NT);//�ɻ��ɱ궨����(�ֱ��ʣ�0.001V/λ)
  }
  else //����
  {
    return (F1_DISCHG_VOLTSL_LT);//�ɻ��ɱ궨����(�ֱ��ʣ�0.001V/λ)
  }
}
//2�����Ϻ���
static
uint32 Fault2_VoltSys_DisCharge(uint8 Temp)
{
  if(Temp>=NTEMP_BOUNDARY)//����(>=0��)
  { 
    return (F2_DISCHG_VOLTSL_NT);//�ɻ��ɱ궨����(�ֱ��ʣ�0.001V/λ)
  }
  else //����
  {
    return (F2_DISCHG_VOLTSL_LT);//�ɻ��ɱ궨����(�ֱ��ʣ�0.001V/λ)
  }
}
//1�ָ���0
static
uint32 Recover1_VoltSys_DisCharge(uint8 Temp)
{
  if(Temp>=NTEMP_BOUNDARY)//����(>=0��)
  { 
    return (R1_DISCHG_VOLTSL_NT);//�ɻ��ɱ궨����(�ֱ��ʣ�0.001V/λ)
  }
  else //����
  {
    return (R1_DISCHG_VOLTSL_LT);//�ɻ��ɱ궨����(�ֱ��ʣ�0.001V/λ)
  }
}

//-----------------���������ϵͳ��ѹ�ж����յĹ��ϵȼ�------------------
static uint8 Fault_DisChg_VoltS(uint32 Volt,uint8 Temp)  //����ϵͳ��ѹ�ͻ����¶�
{
  static uint8 cnt[4];      //ʱ�����
  static uint8 FltL;
  
  if(FltL==0)           //0������
  {
    if(Volt<=Fault1_VoltSys_DisCharge(Temp))  //0��1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0;
    }
    
    if(Volt<=Fault2_VoltSys_DisCharge(Temp))  //0��2
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[2] = 0; 
    cnt[3] = 0;
  }
  else if(FltL == 1) //1������
  {
    if(Volt <= Fault2_VoltSys_DisCharge(Temp))      //1��2
    {
      if(++cnt[2]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
         cnt[2] = 0;
         FltL = 2;
      }
    }
    else
    {
      cnt[2] = 0;
    }
    
    if(Volt >= Recover1_VoltSys_DisCharge(Temp))    //1��0
    {
      if(++cnt[3]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[3] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[3] = 0;
    }
    cnt[0] = 0; 
    cnt[1] = 0;
  }
  else   //2������
  {
    cnt[0] = 0; 
    cnt[1] = 0;
    cnt[2] = 0;
    cnt[3] = 0; 
  }
  return(FltL);
}
/*============================�ŵ���ѹ��===============================*/

/*=======================================================================
                              �ŵ絥���ѹ��0x02
 ========================================================================
 *������1:     Fault1_VoltCell_DisCharge(uint8 Temp)
 *����:        �����¶�ѡ��ŵ絥ѹ1��������ֵ
 *����:        Temp (�����¶�  ����ƫ����:-40)       
 *���أ�       �ȼ�1��ѹ��ֵ 
 
 *������2:     Fault2_VoltCell_DisCharge(uint8 Temp)
 *����:        �����¶�ѡ��ŵ絥ѹ2��������ֵ
 *����:        Temp (�����¶�  ����ƫ����:-40)       
 *���أ�       �ȼ�2��ѹ��ֵ 
 
 *������3:     Recover1_VoltCell_DisCharge(uint8 Temp)
 *����:        �����¶�ѡ��ŵ絥ѹ1���ָ�������ֵ
 *����:        Temp (�����¶�  ����ƫ����:-40)       
 *���أ�       �ȼ�1��ѹ��ֵ 
  
 *������4:     Fault_DisChg_VoltC(uint16 Volt,uint8 Temp)
 *����:        ���ݵ����ѹ���¶��жϻָ�����ϵȼ�
 *����:        Volt(�����ѹ)
               Temp (�����¶�  ����ƫ����:-40)       
 *���أ�       �ŵ絥ѹ�͵Ĺ��ϵȼ�
 
 *˵����       
========================================================================*/ 
//1�����Ϻ���
static
uint16 Fault1_VoltCell_DisCharge(uint8 Temp)
{
  if(Temp>=NTEMP_BOUNDARY)//����(>=0��)
  { 
    return (F1_DISCHG_VOLTCL_NT);//�ɻ��ɱ궨����(�ֱ��ʣ�0.001V/λ)
  }
  else //����
  {
    return (F1_DISCHG_VOLTCL_LT);//�ɻ��ɱ궨����(�ֱ��ʣ�0.001V/λ)
  }
}
//2�����Ϻ���
static
uint16 Fault2_VoltCell_DisCharge(uint8 Temp)
{
  if(Temp>=NTEMP_BOUNDARY)//����(>=0��)
  { 
    return (F2_DISCHG_VOLTCL_NT);//�ɻ��ɱ궨����(�ֱ��ʣ�0.001V/λ)
  }
  else //����
  {
    return (F2_DISCHG_VOLTCL_LT);//�ɻ��ɱ궨����(�ֱ��ʣ�0.001V/λ)
  }
}
//�ָ���0
static
uint16 Recover1_VoltCell_DisCharge(uint8 Temp)
{
  if(Temp>=NTEMP_BOUNDARY)//����(>=0��)
  { 
    return (R1_DISCHG_VOLTCL_NT);//�ɻ��ɱ궨����(�ֱ��ʣ�0.001V/λ)
  }
  else //����
  {
    return (R1_DISCHG_VOLTCL_LT);//�ɻ��ɱ궨����(�ֱ��ʣ�0.001V/λ)
  }
}

//�жϹ��ϵȼ�
static uint8 Fault_DisChg_VoltC(uint16 Volt,uint8 Temp)  //���뵥���ѹ�ͻ����¶�
{
  static uint8 cnt[4];      //ʱ�����
  static uint8 FltL;
  
  if(FltL==0)           //0������
  {
    if(Volt<=Fault1_VoltCell_DisCharge(Temp))  //0��1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0;
    }
    
    if(Volt<=Fault2_VoltCell_DisCharge(Temp))  //0��2
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[2] = 0; 
    cnt[3] = 0;
  }
  else if(FltL == 1) //1������
  {
    if(Volt <= Fault2_VoltCell_DisCharge(Temp))      //1��2
    {
      if(++cnt[2]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
         cnt[2] = 0;
         FltL = 2;
      }
    }
    else
    {
      cnt[2] = 0;
    }
    
    if(Volt >= Recover1_VoltCell_DisCharge(Temp))    //1��0
    {
      if(++cnt[3]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[3] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[3] = 0;
    }
    cnt[0] = 0; 
    cnt[1] = 0;
  }
  else   //2������
  {
    cnt[0] = 0; 
    cnt[1] = 0;
    cnt[2] = 0;
    cnt[3] = 0; 
  }
  return(FltL);
}
/*============================�ŵ絥���ѹ��===========================*/

/*=======================================================================
                              �ŵ絥��ѹ��0x03
 ========================================================================
 *������1:     Fault_DisChg_VoltCD(uint16 V_max,uint16 V_min)
 *����:        �������/�͵����ѹ�ж�ѹ����ϵȼ�
 *����:        V_max(������ߵ�ѹ)
               V_min(������͵�ѹ)       
 *���أ�       �ŵ�ѹ��Ĺ��ϵȼ�
 
 *˵����       
========================================================================*/ 
//�жϹ��ϵȼ�
static uint8 Fault_DisChg_VoltCD(uint16 V_max,uint16 V_min)  //���뵥�����/�͵�ѹ
{
  static uint16 V_Diff;
  static uint8 cnt[2];      //ʱ�����
  static uint8 FltL;
  
  V_Diff = V_max - V_min;   //�����ѹ��
  
  //�жϹ��ϵȼ�
  if(FltL==0)           //0������
  {
    if(V_Diff>=F1_DISCHG_VOLTCD)  //0��1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0; 
    }
    cnt[1] = 0; 
  }
  else     //1������
  {
    if(V_Diff<=R1_DISCHG_VOLTCD)    //1��0
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[0] = 0; 
  }
  return(FltL);
}
/*============================�ŵ絥��ѹ��============================*/

/*=======================================================================
                                �ŵ����0x04
 ========================================================================
 *������1:     Fault_DisChg_TempH(uint8 Temp)
 *����:        �����¶��жϸ��¹��ϵȼ�
 *����:        Temp (�����¶�  ����ƫ����:-40)       
 *���أ�       �ŵ絥ѹ�͵Ĺ��ϵȼ�
 
 *˵����       
========================================================================*/ 
//�жϹ��ϵȼ�
static uint8 Fault_DisChg_TempH(uint8 Temp)  //�����¶�
{
  static uint8 cnt[4];      //ʱ�����
  static uint8 FltL;
  
  if(FltL==0)           //0������
  {
    if(Temp>=F1_DISCHG_TEMPH)  //0��1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0;
    }
    
    if(Temp>=F2_DISCHG_TEMPH)  //0��2
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[2] = 0; 
    cnt[3] = 0;
  }
  else if(FltL == 1) //1������
  {
    if(Temp>=F2_DISCHG_TEMPH)      //1��2
    {
      if(++cnt[2]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
         cnt[2] = 0;
         FltL = 2;
      }
    }
    else
    {
      cnt[2] = 0;
    }
    
    if(Temp<=R1_DISCHG_TEMPH)    //1��0
    {
      if(++cnt[3]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[3] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[3] = 0;
    }
    cnt[0] = 0; 
    cnt[1] = 0;
  }
  else   //2������
  {
    cnt[0] = 0; 
    cnt[1] = 0;
    cnt[2] = 0;
    cnt[3] = 0; 
  }
  return(FltL);
}
/*==============================�ŵ����==============================*/

/*=======================================================================
                                �ŵ����0x05
 ========================================================================
 *������1:     Fault_DisChg_TempL(uint8 Temp)
 *����:        �����¶��жϵ��¹��ϵȼ�
 *����:        Temp (�����¶�  ����ƫ����:-40)       
 *���أ�       �ŵ絥ѹ�͵Ĺ��ϵȼ�
 
 *˵����       
========================================================================*/ 
//�жϹ��ϵȼ�
static uint8 Fault_DisChg_TempL(uint8 Temp)  //�����¶�
{
  static uint8 cnt[4];      //ʱ�����
  static uint8 FltL;
  
  if(FltL==0)           //0������
  {
    if(Temp<=F1_DISCHG_TEMPL)  //0��1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0;
    }
    
    if(Temp<=F2_DISCHG_TEMPL)  //0��2
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[2] = 0; 
    cnt[3] = 0;
  }
  else if(FltL == 1) //1������
  {
    if(Temp<=F2_DISCHG_TEMPL)      //1��2
    {
      if(++cnt[2]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
         cnt[2] = 0;
         FltL = 2;
      }
    }
    else
    {
      cnt[2] = 0;
    }
    
    if(Temp>=R1_DISCHG_TEMPL)    //1��0
    {
      if(++cnt[3]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[3] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[3] = 0;
    }
    cnt[0] = 0; 
    cnt[1] = 0;
  }
  else   //2������
  {
    cnt[0] = 0; 
    cnt[1] = 0;
    cnt[2] = 0;
    cnt[3] = 0; 
  }
  return(FltL);
}
/*==============================�ŵ����==============================*/

/*=======================================================================
                              �ŵ��²�0x06
 ========================================================================
 *������1:     Fault_DisChg_TempD(uint8 T_max,uint8 T_min)
 *����:        �������/���¶��ж��²���ϵȼ�
 *����:        V_max(��������¶�)
               V_min(��������¶�)       
 *���أ�       �ŵ��²�Ĺ��ϵȼ�
 
 *˵����       
========================================================================*/ 
//�жϹ��ϵȼ�
static uint8 Fault_DisChg_TempD(uint8 T_max,uint8 T_min)  //���뵥�����/�͵�ѹ
{
  static uint8 T_Diff;
  static uint8 cnt[2];      //ʱ�����
  static uint8 FltL;
  
  T_Diff = T_max - T_min;   //�����ѹ��
  
  //�жϹ��ϵȼ�
  if(FltL==0)           //0������
  {
    if(T_Diff>=F1_DISCHG_TEMPD)  //0��1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0; 
    }
    cnt[1] = 0; 
  }
  else     //1������
  {
    if(T_Diff<=R1_DISCHG_TEMPD)    //1��0
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[0] = 0; 
  }
  return(FltL);
}
/*============================�ŵ絥���²�============================*/

/*=======================================================================
                                �ŵ����0x07
 ========================================================================
 *������1:     Fault_DisChg_CurrH(float Current)
 *����:        ���ݵ����жϹ�ѹ���ϵȼ�
 *����:        Current (�����¶�  ����ƫ����:-40)       
 *���أ�       �ŵ��ѹ�Ĺ��ϵȼ�
 
 *˵����       
========================================================================*/ 
//�жϹ��ϵȼ�
static uint8 Fault_DisChg_CurrH(float Current)//�������
{
  static uint8 cnt[4];      //ʱ�����
  static uint8 FltL;
  
  if(FltL==0)           //0������
  {
    if(Current>=F1_DISCHG_CURRH)  //0��1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0;
    }
    
    if(Current>=F2_DISCHG_CURRH)  //0��2
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[2] = 0; 
    cnt[3] = 0;
  }
  else if(FltL == 1) //1������
  {
    if(Current>=F2_DISCHG_CURRH)      //1��2
    {
      if(++cnt[2]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
         cnt[2] = 0;
         FltL = 2;
      }
    }
    else
    {
      cnt[2] = 0;
    }
    
    if(Current<=R1_DISCHG_CURRH)    //1��0
    {
      if(++cnt[3]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[3] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[3] = 0;
    }
    cnt[0] = 0; 
    cnt[1] = 0;
  }
  else   //2������
  {
    cnt[0] = 0; 
    cnt[1] = 0;
    cnt[2] = 0;
    cnt[3] = 0; 
  }
  return(FltL);
}
/*================================�ŵ����================================*/

/*=======================================================================
                                �ŵ��Ե����0x08
 ========================================================================
 *������1:     Fault_DisChg_Insul(uint16 Insul)
 *����:        ���ݵ����жϹ�ѹ���ϵȼ�
 *����:        Insul (�����Ե����)       
 *���أ�       �ŵ��Ե���ϵȼ�
 
 *˵����       
========================================================================*/ 
//�жϹ��ϵȼ�
static Fault_DisChg_Insul(uint16 Insul)
{
  static uint8 cnt[4];      //ʱ�����
  static uint8 FltL;
  
  if(FltL==0)           //0������
  {
    
    if(Insul<=F2_DISCHG_INSUL)  //0��2
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[0] = 0;
    }
  }
  return(FltL);
}

/*================================�ŵ��Ե����================================*/


/*=======================================================================
                              �����ѹ��0x11
 ========================================================================
 *������1:     Fault_VoltSys_Charge(uint32 Volt)
 *����:        ����ϵͳ��ѹ�жϹ��ϵȼ�
 *����:        Volt(ϵͳ��ѹ)
 *���أ�       �����ѹ�ߵĹ��ϵȼ�
 
 *˵����       
========================================================================*/ 
//�жϹ��ϵȼ�
static uint8 Fault_VoltSys_Charge(uint32 Volt)  //����ϵͳ��ѹ
{
  static uint8 cnt[4];      //ʱ�����
  static uint8 FltL;
  
  if(FltL==0)           //0������
  {
    if(Volt>=F1_CHARGE_VOLTSH)  //0��1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0;
    }
    
    if(Volt>=F2_CHARGE_VOLTSH)  //0��2
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[2] = 0; 
    cnt[3] = 0;
  }
  else if(FltL == 1) //1������
  {
    if(Volt>=F2_CHARGE_VOLTSH)      //1��2
    {
      if(++cnt[2]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
         cnt[2] = 0;
         FltL = 2;
      }
    }
    else
    {
      cnt[2] = 0;
    }
    
    if(Volt<=R1_CHARGE_VOLTSH)    //1��0
    {
      if(++cnt[3]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[3] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[3] = 0;
    }
    cnt[0] = 0; 
    cnt[1] = 0;
  }
  else   //2������
  {
    cnt[0] = 0; 
    cnt[1] = 0;
    cnt[2] = 0;
    cnt[3] = 0; 
  }
  return(FltL);
}
/*============================�����ѹ��===============================*/

/*=======================================================================
                              ��絥���ѹ��0x12
 ========================================================================
 *������1:     Fault_VoltCell_High_Charge(uint16 Volt)
 *����:        ���ݵ����ѹ�жϻָ�����ϵȼ�
 *����:        Volt(�����ѹ)
 *���أ�       ��絥���ѹ�ߵĹ��ϵȼ�
 
 *˵����       
========================================================================*/ 
//�жϹ��ϵȼ�
static uint8 Fault_VoltCell_High_Charge(uint16 Volt)  //���뵥���ѹ�ͻ����¶�
{
  static uint8 cnt[4];      //ʱ�����
  static uint8 FltL;
  
  if(FltL==0)           //0������
  {
    if(Volt>=F1_CHARGE_VOLTCH)  //0��1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0;
    }
    
    if(Volt>=F2_CHARGE_VOLTCH)  //0��2
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[2] = 0; 
    cnt[3] = 0;
  }
  else if(FltL == 1) //1������
  {
    if(Volt>=F2_CHARGE_VOLTCH)      //1��2
    {
      if(++cnt[2]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
         cnt[2] = 0;
         FltL = 2;
      }
    }
    else
    {
      cnt[2] = 0;
    }
    
    if(Volt<=R1_CHARGE_VOLTCH)    //1��0
    {
      if(++cnt[3]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[3] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[3] = 0;
    }
    cnt[0] = 0; 
    cnt[1] = 0;
  }
  else   //2������
  {
    cnt[0] = 0; 
    cnt[1] = 0;
    cnt[2] = 0;
    cnt[3] = 0; 
  }
  return(FltL);
}
/*============================��絥���ѹ��===========================*/

/*=======================================================================
                              ��絥��ѹ��0x13
 ========================================================================
 *������1:     Fault_VoltCell_Diff_Charge(uint16 V_max,uint16 V_min)
 *����:        ���ݵ����ѹ�жϳ�絥���ѹ�͹��ϵȼ�
 *����:        V_max(������ߵ�ѹ)
               V_min(������͵�ѹ)      
 *���أ�       ��絥��ѹ��Ĺ��ϵȼ�
 
 *˵����       
========================================================================*/ 
//�жϹ��ϵȼ�
static uint8 Fault_VoltCell_Diff_Charge(uint16 V_max,uint16 V_min)  //���뵥�����/�͵�ѹ
{
  static uint16 V_Diff;
  static uint8 cnt[2];      //ʱ�����
  static uint8 FltL;
  
  V_Diff = V_max - V_min;   //�����ѹ��
  
  //�жϹ��ϵȼ�
  if(FltL==0)           //0������
  {
    if(V_Diff>=F1_CHARGE_VOLTCD)  //0��1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0; 
    }
    cnt[1] = 0; 
  }
  else     //1������
  {
    if(V_Diff<=R1_CHARGE_VOLTCD)    //1��0
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[0] = 0; 
  }
  return(FltL);
}
/*=============================��絥��ѹ��=============================*/

/*=======================================================================
                                ������0x14
 ========================================================================
 *������1:     Fault_Temp_High_Charge(uint8 Temp)
 *����:        �����¶��жϸ��¹��ϵȼ�
 *����:        Temp (�����¶�  ����ƫ����:-40)       
 *���أ�       �����µĹ��ϵȼ�

 *˵����       
========================================================================*/ 
//�жϹ��ϵȼ�
static uint8 Fault_Temp_High_Charge(uint8 Temp)  //�����¶�
{
  static uint8 cnt[4];      //ʱ�����
  static uint8 FltL;
  
  if(FltL==0)           //0������
  {
    if(Temp>=F1_CHARGE_TEMPH)  //0��1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0;
    }
    
    if(Temp>=F2_CHARGE_TEMPH)  //0��2
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[2] = 0; 
    cnt[3] = 0;
  }
  else if(FltL == 1) //1������
  {
    if(Temp>=F2_CHARGE_TEMPH)      //1��2
    {
      if(++cnt[2]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
         cnt[2] = 0;
         FltL = 2;
      }
    }
    else
    {
      cnt[2] = 0;
    }
    
    if(Temp<=R1_CHARGE_TEMPH)    //1��0
    {
      if(++cnt[3]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[3] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[3] = 0;
    }
    cnt[0] = 0; 
    cnt[1] = 0;
  }
  else   //2������
  {
    cnt[0] = 0; 
    cnt[1] = 0;
    cnt[2] = 0;
    cnt[3] = 0; 
  }
  return(FltL);
}
/*==============================������==============================*/

/*=======================================================================
                                ������0x15
 ========================================================================
 *������1:     Fault_Temp_Low_Charge(uint8 Temp)
 *����:        �����¶��жϵ��¹��ϵȼ�
 *����:        Temp (�����¶�  ����ƫ����:-40)       
 *���أ�       �����µĹ��ϵȼ�
 
 *˵����       
========================================================================*/ 
//�жϹ��ϵȼ�
static uint8 Fault_Temp_Low_Charge(uint8 Temp)  //�����¶�
{
  static uint8 cnt[4];      //ʱ�����
  static uint8 FltL;
  
  if(FltL==0)           //0������
  {
    if(Temp<=F1_CHARGE_TEMPL)  //0��1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0;
    }
    
    if(Temp<=F2_CHARGE_TEMPL)  //0��2
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[2] = 0; 
    cnt[3] = 0;
  }
  else if(FltL == 1) //1������
  {
    if(Temp<=F2_CHARGE_TEMPL)      //1��2
    {
      if(++cnt[2]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
         cnt[2] = 0;
         FltL = 2;
      }
    }
    else
    {
      cnt[2] = 0;
    }
    
    if(Temp>=R1_CHARGE_TEMPL)    //1��0
    {
      if(++cnt[3]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[3] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[3] = 0;
    }
    cnt[0] = 0; 
    cnt[1] = 0;
  }
  else   //2������
  {
    cnt[0] = 0; 
    cnt[1] = 0;
    cnt[2] = 0;
    cnt[3] = 0; 
  }
  return(FltL);
}
/*==============================������==============================*/

/*=======================================================================
                              ����²�0x16
 ========================================================================
 *������1:     Fault_Temp_Diff_Charge(uint8 T_max,uint8 T_min)
 *����:        �������/���¶��ж��²���ϵȼ�
 *����:        T_max(��������¶�)
               T_min(��������¶�)       
 *���أ�       ����²�Ĺ��ϵȼ�
 
 *˵����       
========================================================================*/ 
//�жϹ��ϵȼ�
static uint8 Fault_Temp_Diff_Charge(uint8 T_max,uint8 T_min)  //���뵥�����/�͵�ѹ
{
  static uint8 T_Diff;
  static uint8 cnt[2];      //ʱ�����
  static uint8 FltL;
  
  T_Diff = T_max - T_min;   //�����ѹ��
  
  //�жϹ��ϵȼ�
  if(FltL==0)           //0������
  {
    if(T_Diff>=F1_CHARGE_TEMPD)  //0��1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0; 
    }
    cnt[1] = 0; 
  }
  else     //1������
  {
    if(T_Diff<=R1_CHARGE_TEMPD)    //1��0
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[0] = 0; 
  }
  return(FltL);
}
/*================================����²�==============================*/

/*=======================================================================
                                ������0x17
 ========================================================================
 *������1:     Fault_Current_Charge(float Current)
 *����:        ���ݵ����жϳ��������ϵȼ�
 *����:        Current       
 *���أ�       �������Ĺ��ϵȼ�
 
 *˵����       
========================================================================*/ 
//�жϹ��ϵȼ�
static uint8 Fault_Current_Charge(float Current)//�����¶�
{
  static uint8 cnt[4];      //ʱ�����
  static uint8 FltL;
  
  if(FltL==0)           //0������
  {
    if(Current>=F1_CHARGE_CURRH)  //0��1
    {
      if(++cnt[0]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[0] = 0;
        FltL = 1;
      }
    }
    else
    {
      cnt[0] = 0;
    }
    
    if(Current>=F2_CHARGE_CURRH)  //0��2
    {
      if(++cnt[1]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[1] = 0;
        FltL = 2;
      }
    }
    else
    {
      cnt[1] = 0;
    }
    cnt[2] = 0; 
    cnt[3] = 0;
  }
  else if(FltL == 1) //1������
  {
    if(Current>=F2_CHARGE_CURRH)      //1��2
    {
      if(++cnt[2]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
         cnt[2] = 0;
         FltL = 2;
      }
    }
    else
    {
      cnt[2] = 0;
    }
    
    if(Current<=R1_CHARGE_CURRH)    //1��0
    {
      if(++cnt[3]*PERIOD_DISCHARGE/1000>=DELAYTIME_DANGERLEVEL2)
      {
        cnt[3] = 0;
        FltL = 0;
      }
    }
    else
    {
      cnt[3] = 0;
    }
    cnt[0] = 0; 
    cnt[1] = 0;
  }
  else   //2������
  {
    cnt[0] = 0; 
    cnt[1] = 0;
    cnt[2] = 0;
    cnt[3] = 0; 
  }
  return(FltL);
}
/*================================������================================*/

