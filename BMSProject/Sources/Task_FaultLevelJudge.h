/*=======================================================================
 *Subsystem:   ���
 *File:        Task_FaultLevelJudge.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _TASK_FAULTLEVELJUDGE_H_
#define _TASK_FAULTLEVELJUDGE_H_

/*======================================================================
                             �����жϽṹ��
========================================================================*/
  //==========================�������ж�============================
  typedef struct
  {
    //-------------------���ϵȼ��������趨Ҫ��-----------------------
    //�����ѹ
    uint8 Level_Volt_Cell_High;               
    uint8 Level_Volt_Cell_Low; 
    uint8 Level_Volt_Cell_Diff_High;
    //ϵͳ��ѹ    
    uint8 Level_Volt_Sys_High; 
    
    //�¶�     
    uint8 Level_Temp_High;    
    uint8 Level_Temp_Low;     
    uint8 Level_Temp_Diff_High;
    
    //������   
    uint8 Level_Current_Charge_High; 
 
    uint8 Level_Charge_SwitchOff_flag;
    
    uint8 Level_Charge_BalanceOff_Flag;
          
  }Fault_Charge_T;
  extern Fault_Charge_T Fault_Charge;             //�������ж�
  
  //==========================�ŵ�����ж�===========================
  typedef struct
  {
    //-------------------���ϵȼ��������趨Ҫ��-----------------------
    //�����ѹ
    uint8 Level_Volt_Cell_Low; 
    uint8 Level_Volt_Cell_Diff_High;
    //ϵͳ��ѹ    
    uint8 Level_Volt_Sys_Low;
    
    //�¶�     
    uint8 Level_Temp_High;    
    uint8 Level_Temp_Low;     
    uint8 Level_Temp_Diff_High;
    
    //�ŵ����   
    uint8 Level_Current_DisCharge_High; 
    
    uint8 Level_DisCharge_SwitchOff_flag;
    
    uint8 Level_DisCharge_BalanceOff_Flag;
    
  }Fault_DisCharge_T;
  extern Fault_DisCharge_T Fault_DisCharge;       //�ŵ�����ж�
  
  //==============================�����ź�==============================
  typedef struct
  {
    //---------------------����״̬��0������1����----------------------
    uint8 VCU;     
    uint8 HVU;
    
    uint8 CSSU1;
    #if(SYS_NUMBER_MODULE>=2)
      uint8 CSSU2;
    #endif
    #if(SYS_NUMBER_MODULE>=3)
      uint8 CSSU3;
    #endif
    #if(SYS_NUMBER_MODULE>=4)
      uint8 CSSU4;
    #endif
    #if(SYS_NUMBER_MODULE>=5)
      uint8 CSSU5;
    #endif
    #if(SYS_NUMBER_MODULE>=6)
      uint8 CSSU6;
    #endif
          
  }State_Offline_T;
  extern State_Offline_T State_Offline;             //�������ж� 
  
  //==============================�����ź�==============================
  typedef struct
  {
    //CSSU
    uint8 HeartBeat_CSSU1;
    #if(SYS_NUMBER_MODULE>=2)
      uint8 HeartBeat_CSSU2;
    #endif
    #if(SYS_NUMBER_MODULE>=3)
      uint8 HeartBeat_CSSU3;
    #endif
    #if(SYS_NUMBER_MODULE>=4)
      uint8 HeartBeat_CSSU4;
    #endif
    #if(SYS_NUMBER_MODULE>=5)
      uint8 HeartBeat_CSSU5;
    #endif
    #if(SYS_NUMBER_MODULE>=6)
      uint8 HeartBeat_CSSU6;
    #endif
    //VCU
    uint8 HeartBeat_VCU;
    //HVU
    uint8 HeartBeat_HVU;
  }HeartBeat_T;
  extern HeartBeat_T HeartBeat;              //�����ź�
  
  //=========================�ŵ������ֵ================================
  typedef struct
  {
    //�ŵ���ѹ��
    uint32 F2_DisChg_VoltSL_NT;
    uint32 F1_DisChg_VoltSL_NT;
    
    uint32 F2_DisChg_VoltSL_LT;
    uint32 F1_DisChg_VoltSL_LT;
    //�ŵ絥���
    uint16 F2_DisChg_VoltCL_NT;
    uint16 F1_DisChg_VoltCL_NT;
    
    uint16 F2_DisChg_VoltCL_LT;
    uint16 F1_DisChg_VoltCL_LT;
    //�ŵ絥��ѹ��
    uint16 F1_DisChg_VoltCD;
    //�ŵ����
    uint8  F2_DisChg_TempH;
    uint8  F1_DisChg_TempH;
    //�ŵ����
    uint8  F2_DisChg_TempL;
    uint8  F1_DisChg_TempL;
    //�ŵ��²�
    uint8  F1_DisChg_TempD;
    //�ŵ����
    float  F2_DisChg_CurrH;
    float  F1_DisChg_CurrH;
    //��Ե����
    uint16 F2_DisChg_Insul;
    
  }FltThreshold_DisChg_T;
  extern  FltThreshold_DisChg_T FltThreshold_DisChg;

  //=========================��������ֵ================================  
  typedef struct
  {
    //�����ѹ��
    uint32 F2_Charge_VoltSL;
    uint32 F1_Charge_VoltSL;
    
    //��絥���
    uint16 F2_Charge_VoltCL;
    uint16 F1_Charge_VoltCL;
    
    //��絥��ѹ��
    uint16 F1_Charge_VoltCD;
    //������
    uint8  F2_Charge_TempH;
    uint8  F1_Charge_TempH;
    //������
    uint8  F2_Charge_TempL;
    uint8  F1_Charge_TempL;
    //����²�
    uint8  F1_Charge_TempD;
    //������
    float  F2_Charge_CurrH;
    float  F1_Charge_CurrH;
    //��Ե����
    uint16 F2_Charge_Insul;
    
  }FltThreshold_Charge_T;
  extern  FltThreshold_Charge_T FltThreshold_Charge;
/*-======================================================================
                              ��������
=========================================================================*/
 void Task_FaultLevelJudge(uint8 workstate);

#endif