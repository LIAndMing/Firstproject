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
#include "WorkModeJudge.h"  

WorkStateJudge_T  g_WorkStateJudge;
/*=======================================================================
 *������:      ChargeJudgment 
 *����:        �жϳ�绹�Ƿŵ�״̬
 *����:        ��       
 *���أ�       ��
 *˵����       ����ʱ�䵽��־λ��1��10msִ��һ��
========================================================================*/
uint8 WokeModeJudgment(void)                                
{  
  //�жϳ���״̬(���or�ŵ�)
  uint8  i=0, q=0;
  uint16 cc2state;
  for(i=0; i<10; i++)
  {
     cc2state = ADC_CC2_State();
     if(cc2state <= 3000)
     {
       q++;
     }
  }   
  if(q == 10)//����10������ͬ��״̬
  {
    return MODE_CHARGE;   // ChagMode == 1
  }
  else
  {
    return MODE_DISCHARGE;
  }
}