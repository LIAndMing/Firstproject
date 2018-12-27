 /*=======================================================================
 *Subsystem:   ���
 *File:        Task_SOC.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef  _TASK_SOC_H_
#define  _TASK_SOC_H_

  #define SOC_Period      100          /* SOC�ļ���������100ms */
  
//SOC��Ϣ�ṹ��
typedef  struct
{
  uint16  SOC_Init;                   //SOC��ʼ��
  float   SOC_LowestVoltGet;          //��͵�ѹ��Ӧ��SOCֵ��0-1��
  float   SOC_HighestVoltGet;         //��ߵ�ѹ��Ӧ��SOCֵ��0-1��  
  uint16  SOC_CalTime;                //SOC��ʱ 1s����һ��
  float   SOC_ValueRead;              //SOC��ȡֵ����λΪ1��0-1,��ȡ��ֵ
  float   SOC_ValueVoltGet;           //SOC����ֵ����λΪ1��0-1
  float   SOC_ValueInitDiff;         //SOC��ֵ����ȡֵ-����ֵ
  float   SOC_ValueRealtimeDiff;     //SOCʵʱ���𽥵ݼ�
  uint16  SOC_CheckTable_StartTime;   //����ʱ�ĳ�ֵ�����ڳ����µ�SOCʱ������
  uint8   SOC_CheckTable_Flag;        //����״̬�£����Ƿ���в����б��
  uint32  SOC_StandingTime;           //SOC����ʱ��,���ο���ʱ��-�ϴιر�ʱ��
}SOCInfo_T;
extern    SOCInfo_T    SOCInfo;

typedef  struct
{
  float   Energy_Once_DisCharge;           //���ηŵ���
  float   Energy_Total_Charge;             //�ܳ���� 
  float   Energy_Total_DisCharge;          //�ܷŵ���
   
}EnergyInfo_T;
extern    EnergyInfo_T    EnergyInfo;

/*
//δ���õı��
 typedef struct
  {
    uint8   Start_Run_Time;  //�����ϵͳ��ʼ����ʱ��
    
    
    

  }SOC_LEP_DATA_T;
 */
#endif