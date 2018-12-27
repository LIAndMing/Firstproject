/*=======================================================================
 *Subsystem:   ���
 *File:        BattInfoParaStructure.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _BATT_INFO_PARA_STRUCTURE_H_
#define _BATT_INFO_PARA_STRUCTURE_H_

#include  "BattInfoConfig.h"

//��ѹ��Ϣ�ṹ��
typedef  struct
{
  uint16  CellVolt[SYS_SERIES];    //�����ѹ
  uint16  CellVolt_Max;           //��������ѹ
  uint16  CellVolt_Min;           //������С��ѹ
  uint8   CellVolt_MaxNode;       //��������ѹ�ڵ�
  uint8   CellVolt_MinNode;       //������С��ѹ�ڵ�
  uint16  CellVolt_Diff;
  uint16  CellVolt_Ave;           //ֻ�����ھ������
}VoltInfo_T;
extern VoltInfo_T VoltInfo;

//�¶���Ϣ�ṹ��
typedef  struct
{
  uint8   CellTemp[SYS_NUMBER_TEMP];   //�����¶�      �ֱ��ʣ�1��  ƫ������-40
  uint8   CellTemp_Max;           //��������¶�       �ֱ��ʣ�1��  ƫ������-40
  uint8   CellTemp_MaxNode;       //��������¶Ƚڵ�
  uint8   CellTemp_Min;           //��������¶�       �ֱ��ʣ�1��  ƫ������-40
  uint8   CellTemp_MinNode;       //��������¶Ƚڵ�
  uint8   CellTemp_Ave;           //����ƽ���¶�       �ֱ��ʣ�1��  ƫ������-40
  uint8   CellTemp_Diff;
}TempInfo_T;
extern TempInfo_T TempInfo;

//��Ե��Ϣ�ṹ��
typedef  struct
{
  uint16  Insul_Resis_Pos;          //��Ե���Եص���
  uint16  Insul_Resis_Neg;          //��Ե���Եص���
  uint16  Insul_Resis;              //��Ե����,ȡ��/���Եص����е���Сֵ
  uint8   Insul_FaultGrade;         //��Ե���ϵȼ�
  uint8   Insul_Life;               //��Ե�ź�
  uint16  Insul_Volt;               //��Ե��ѹ
}InsulInfo_T;
extern InsulInfo_T InsulInfo;

//���߿�·
typedef struct
{
  uint8  OpenWire_Status;                      //���߿�·״̬
  uint8  OpenWire_Node[SYS_NUMBER_MODULE];     //���߿�·�ڵ�
}OpenWireInfo_T;
extern OpenWireInfo_T OpenWireInfo;

//�ɼ�������
typedef struct
{
  uint32  ModuleVolt_Total[SYS_NUMBER_MODULE];     //�ɼ���ģ����ѹ 
  uint32  SysVolt_Total;                           //ϵͳ��ѹ�ܺ�    �ֱ���:0.0001V
  float   DataCollet_Current_Hall;                 //���������������ĵ���
  float   DataCollet_Current_Filter;               //�˲������ĵ���
}DataColletInfo_T;
extern DataColletInfo_T DataColletInfo;
  
  


#endif