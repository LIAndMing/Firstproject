/*=======================================================================
 *Subsystem:   ���
 *File:        Sys_Init.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _SYS_INIT_H_
#define _SYS_INIT_H_  

  #include  "TypeDefinition.h"
  #include  "libdefs.h"
  #include  "MC9S12XEP100.h"
  #include  "derivative.h"
  #include  "Task_FaultLevelJudge.h"
  #include  "Roll_Task.h"
  #include  "Task_InsulDetect.h"
  #include  "BattInfoParaStructure.h"
  #include  "Task_CurrentLimit.h"
  #include  "EEDFLASH.h"
  #include  "Task_DataProcess.h"
  #include  "EEPROM.h"
  #include  "DS3231.h"
  #include  "Task_SOC.h"
  #include  "Task_BmsAndUpMonitor.h"
  #include  "Task_PowerOnOff.h"
  #include  "Task_BMSCheckSelf.h"


  typedef struct
  {
  
    //�ײ��ʼ�� 00:�ɹ�;1:ʧ��
    uint8 ADC;
    uint8 IIC;
    //PIT
    uint8 PIT0;
    
    uint8 PLL;
    //GPIO
    uint8 Relay_Positvie;
    
    uint8 EEPROM;
    
    //CAN
      //uint8 CAN0;
    uint8 CAN1;
    uint8 CAN2;
      //uint8 CAN3;
    
    //SCI ��ʾ����ʼ��
    uint8 Screen;
         
    uint8 SPI;
    
    uint8 Insul; 
    //���г�ʼ���ɹ� 00:�ɹ�
    uint8 AllInit_State;   
   
  }System_InitState_T;
  extern System_InitState_T System_InitState;
  

  void System_Init(void);
  
#endif  