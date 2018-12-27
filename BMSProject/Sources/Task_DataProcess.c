/*=======================================================================
 *Subsystem:   ���
 *File:        Task_DataCollection.c
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:  
========================================================================*/

#include  "TypeDefinition.h"
#include  "BattInfoParaStructure.h"
#include  "ADC.h"
#include  "Task_DataProcess.h"
#include  "Filter_Function.h"
#include  "CAN.h"
#include  "DataFromCSSU.h"

DataColletInfo_T DataColletInfo;

static void DataCollection_Volt(void);
static void DataCollection_Temp(void);
/*=======================================================================
 *������:      Task_DataProcess(void) 
 *����:        �ɼ���������ѹ���¶ȵ���Ϣ
 *����:        ��       
 *���أ�       ��
 *˵����       ����SOCʱ�����õĵ���ֵ,����Ҫ���ǻ����������Ƿ���һ����
========================================================================*/
void Task_DataProcess(void)                       
{
  
  
}

/*=======================================================================
 *������:      DataCollection_Volt()  
 *����:        �ɼ���ѹ��Ϣ
 *����:        ��       
 *���أ�       ��
 
 *˵����        
========================================================================*/
static
void DataCollection_Volt(void)
{
  uint8   i;
  uint16  max_v=0,min_v=0xFFFFFFFF;
  uint32  total_v=0;
  uint8   max_vnub=0,min_vnub=0;
  
  for(i=0; i<SYS_NUMBER_BOX_MODULE; i++)
  {
    if (Module_VoltInfo[i].Max_VoltC > max_v) 
    {
      max_v = Module_VoltInfo[i].Max_VoltC;
      max_vnub = Module_VoltInfo[i].Max_VoltC_Node;
    } 
    if (Module_VoltInfo[i].Min_VoltC < min_v) 
    {
      min_v = Module_VoltInfo[i].Min_VoltC;
      min_vnub = Module_VoltInfo[i].Min_VoltC_Node;
    }      
    total_v += DataColletInfo.ModuleVolt_Total[i] ; //����ģ��ĵ�ѹ֮��
    total_v *= 0.5;                                 //ϵͳ��ѹ
  }
  
  VoltInfo.CellVolt_Max         = max_v;
  VoltInfo.CellVolt_Min         = min_v; 
  VoltInfo.CellVolt_MaxNode     = max_vnub;
  VoltInfo.CellVolt_MinNode     = min_vnub;
  DataColletInfo.SysVolt_Total  = total_v;
  VoltInfo.CellVolt_Diff        = max_v - min_v;
  VoltInfo.CellVolt_Ave         = DataColletInfo.SysVolt_Total / SYS_SERIES;
} 

/*=======================================================================
 *������:      DataCollection_Temp()  
 *����:        �ɼ���ѹ��Ϣ
 *����:        ��       
 *���أ�       ��
 
 *˵����        
========================================================================*/
static
void DataCollection_Temp(void) 
{
  uint8   i;
  uint16  max_t=0,min_t=0xFFFFFFFF;
  uint32  ave_t;
  uint8   max_tnub=0,min_tnub=0;
  
  for(i=0; i<SYS_NUMBER_BOX_MODULE; i++)
  {
    if (Module_TempInfo[i].Max_Temp > max_t) 
    {
      max_t = Module_TempInfo[i].Max_Temp;
      max_tnub = Module_TempInfo[i].Max_TempNode;
    } 
    if (Module_TempInfo[i].Min_Temp < min_t) 
    {
      min_t = Module_TempInfo[i].Min_Temp;
      min_tnub = Module_TempInfo[i].Min_TempNode;
    }      
    ave_t   += Module_TempInfo[i].Ave_Temp ; //ģ���ƽ���¶�
    ave_t   *= 0.5;                                 //ϵͳƽ���¶�
  }
  
  TempInfo.CellTemp_Max         = max_t;
  TempInfo.CellTemp_Min         = min_t; 
  TempInfo.CellTemp_MaxNode     = max_tnub;
  TempInfo.CellTemp_MinNode     = min_tnub;
  TempInfo.CellTemp_Ave         = ave_t;
  TempInfo.CellTemp_Diff        = max_t - min_t;
}





