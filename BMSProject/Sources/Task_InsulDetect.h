/*=======================================================================
 *Subsystem:   ���
 *File:        Task_InsulDetect.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _TASK_INSUL_DETECT_H_
#define _TASK_INSUL_DETECT_H_  

  #include"TypeDefinition.h"
  #include"MC9S12XEP100.h"

//#define HighVoltSV       PORTB_PB0     //��ѹ��⿪��
   //#define HighVoltSV_Dir   DDRB_DDRB0
   
   #define HighVoltS1       PORTA_PA6     //V+����
   #define HighVoltS1_Dir   DDRA_DDRA6
   
   #define HighVoltS2       PORTA_PA7     //V-����
   #define HighVoltS2_Dir   DDRA_DDRA7
   
   //#define HighVoltLS1      PORTB_PB4     //��ѹ�����⿪����
   //#define HighVoltLS1_Dir  DDRB_DDRB4
   
   //#define HighVoltLS2      PORTB_PB1     //��ѹ�����⿪�ظ�
   //#define HighVoltLS2_Dir  DDRB_DDRB1
   
   #define INS_SwitchON         1
   #define INS_SwitchOFF        0             //��ԵMOS�ܿ���
   
   #define DetectCount      12            //������ѹ12��
   
   #define LowVoltageDec    30            //��ѹʱ�ľ�Ե���ֵ��δ����
   #define Bias_Resitance   62            //ƫ�õ�����ֵ
   
   #define Resistance_Alarm1  100         //��Ե������ϵȼ���ֵ��δ����
   #define Resistance_Alarm2  50          //��Ե������ϵȼ���ֵ ��δ���� 
     
   #define HVPositiveChannel  14          //��Ե�������AD��
   #define HVNegtiveChannel    6          //��Ե��⸺��AD��
  
   typedef struct
   {
     uint8  insulation_grade;    //��Ե���ϵȼ�
     uint8  insulation_curr;     //�Ƿ�©��0��1
     uint16 insulation_resist;   //��Ե��ֵ
     float insulation_Vposit;    //������ѹ
     float insulation_Vnegt;     //�Ը���ѹ
     float insulation_TotalVolt; //��Ե�����ѹ
     uint8 insulation_Disable;   //ʹ�ܱ��(���ǰ�ڲ���ʹ��)
     
     uint16 insulation_resist_P;  //��������
     uint16 insulation_resist_N;  //��������
   }IsoResist;
   extern IsoResist IsoDetect;

   void VoltageDetectInit(void);
   void Task_InsulationDetect(void);





#endif