/*=======================================================================
 *Subsystem:   ���
 *File:        Task_PowerOnOff.c
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:  
========================================================================*/

 #include   "Task_PowerOnOff.h"
/*=======================================================================
 *������:      PositiveRelay_OFF
 *����:        �����̵����Ŀ���
 *����:        wokemode:����ģʽ
               FltLevel:�պϼ̵�����2������
                      
 *���أ�       ��
 *˵����       ADC ʱ��Ƶ�ʣ�2MHz
========================================================================*/
void PositiveRelay_OFF(uint8 FltLevel)
{
  if(FltLevel&&(Relay_StateGet(Relay_Positive_PORT, Relay_Positive_pin)==Relay_ON))
  {
     Relay_Control(Relay_Positive_PORT, Relay_Positive_pin, Relay_OFF);
  }

} 

void PositiveRelay_ON(void)
{
  if(Relay_StateGet(Relay_Positive_PORT, Relay_Positive_pin)==Relay_OFF)
  {
    Relay_Control(Relay_Positive_PORT, Relay_Positive_pin, Relay_ON); 
  } 
}


/*=======================================================================
 *������:      PositiveRelay_OFF
 *����:        �����̵����Ŀ���
 *����:        wokemode:����ģʽ
               FltLevel:�պϼ̵�����2������
                      
 *���أ�       ��
 *˵����       ADC ʱ��Ƶ�ʣ�2MHz
========================================================================*/
/*void Task_PowerOnOff(uint8 FltLevel, uint8 selfchecksate)
{
   if(selfchecksate == 1)
   {
      PositiveRelay_ON();
   }
   
   if(FltLevel == 1)
   {
      PositiveRelay_OFF();
   }
}*/