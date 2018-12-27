/*=======================================================================
 *Subsystem:   ���
 *File:        Task_CurrentLimit.C
 *Author:      Wenming
 *Description: ͨ�ţ�
               �ӿڣ�
               �����ʣ�
 ========================================================================
 * History:    �޸���ʷ��¼�б�
 * 1. Date:
      Author:
      Modification:
========================================================================*/
 #include"TypeDefinition.h"
 #include"Task_CurrentLimit.h"
 #include"BattInfoParaStructure.h"
 
 CurrentLimit_T CurrentLimit;

 static float ChagCurrLimit_Constant(uint8 Temp);
/*=======================================================================
 *������:      Dis_ChargeCurrent
 *����:        ��ŵ�����ж�
 *����:        ��                                     
 *���أ�       ��
 *˵����       BMS�����¶������ж���������������Ĵ�С
========================================================================*/ 
void Task_CurrentLimit(void)
{
  //����������
  CurrentLimit.Current_Charge_Constant = ChagCurrLimit_Constant(TempInfo.CellTemp_Ave);
    
  //Task_Flag_Counter.Counter_ChargeCurrent++;

} 

/*=======================================================================
 *������:      Current_Charge_Constant()
 *����:        ��ŵ�����ж�
 *����:        ��                                     
 *���أ�       ��
 *˵����       BMS�����¶������ж���������������Ĵ�С
========================================================================*/ 
static
float ChagCurrLimit_Constant(uint8 Temp)
{
  static float Current;
  
  if(Temp>=0 && Temp<=55)
  {
    if(Temp>50)
    {
      Current = 100;
    }
    else if(Temp>10)
    {
      Current = 150;
    }
    else if(Temp>5)
    {
      Current = 100;
    }
    else 
    {
      Current = 50;
    }
  }
  else
  {
    Current = 0;
  }
  return Current;
}