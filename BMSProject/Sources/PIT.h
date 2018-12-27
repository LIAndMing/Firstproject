/*=======================================================================
 *Subsystem:   ���
 *File:        PIT.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _PIT_H_
#define _PIT_H_  

  #include"TypeDefinition.h"

//���Ż��ֹPIT0����ж�
    #define EnablePIT(channel)           PITINTE |= (1<<channel)    //���Ŷ�ʱ������ж�
    #define DisablePIT(channel)          PITINTE &= ~(1<<channel)  //��ֹ��ʱ������ж�

  //PIT��ʼ��
  enum Init_PIT
  {
    //PIT��ʼ������
    Init_Normal_PIT = 0,
    Init_Fault_PIT_Channel,
    Init_Fault_PIT_Period
  };

  typedef struct 
  {
    uint16 Period_10ms;       // 10ms����ʱ���־;
    uint16 Period_50ms;       // 50ms����ʱ���־;
    uint16 Period_100ms;      // 100ms����ʱ���־;
    uint16 Period_200ms;      // 200ms����ʱ���־;
    uint16 Period_250ms;      // 250ms����ʱ���־;
    uint16 Period_500ms;      // 500ms����ʱ���־;
    uint16 Period_1s;         // 1s����ʱ���־; 
  }PIT_Time_10ms_T;
  extern PIT_Time_10ms_T PIT_Time_10ms;  



//��������������

    //TimerInit:��ʱ����ʼ������--------------------------------------------*
    //��  ��:��ʱ����ʼ�����ж�һ��ʱ��Ϊ1/2��                              *
    //��  ��:channel==0~7, MUXSEL:0-ѡ��΢ʱ��0��1-ѡ��΢ʱ��1
    // MTLD:����΢��ʱ�����ؼĴ�����ֵ�� LD������ͨ�����ؼĴ�����ֵ         *                                                             *
    //��  ��:��                                                             *
    //----------------------------------------------------------------------*
    void PITInit(uint8 channel,uint8 MUXSEL,uint8 MTLD,uint16 LD);
    uint8 Init_PIT(uint8 channel, uint8 perio);
   // void init_PIT0(void);
   // void init_PIT1(void);
    //----------------------------------------------------------------------*
    //������: SecAdd1(��ʱ����)                                             *
    //��  ��: ����Ϊ��С��λ����ʱ,��,�뻺������ֵ(00:00:00-23:59:59)       *
    //��  ��: *p:�����������׵�ַ                                           *
    //��  ��: ��                                                            *
    //----------------------------------------------------------------------*

#endif                                                                           
