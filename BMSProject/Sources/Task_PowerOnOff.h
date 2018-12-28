#ifndef _TASK_POWERONOFF_H_
#define _TASK_POWERONOFF_H_
    
    #include  "TypeDefinition.h"
    #include  "Port_Control.h"
    #include  "Task_FaultLevelJudge.h"
    #include  "Task_ChargeCfg.h"
    #include  "DS3231.h"
    #include  "BattInfoParaStructure.h"
    
    //���µ��жϺ�����ִ������Ϊ500ms
    #define   PEWERONOFF_PERIO            500
    //1.�̵������غ궨��
    #define Relay_ON        0  //��
    #define Relay_OFF       1  //��
    
    //2.�̵������ź궨��
    #define Relay_Positive_PORT            PA  
    #define Relay_Positive_pin              1   
    
    //#define Relay_Negative_PORT          PB
    //#define Relay_Negative_pin            1  
    
    //#define Relay_Quickcharge_PORT       PB
    //#define Relay_Quickcharge_pin         2
    
    #define Relay_CSSUPower_PORT           PA
    #define Relay_CSSUPower_pin             3 
    
    #define Relay_ScreenPower_PORT         PA
    #define Relay_ScreenPower_pin           4 
    
    typedef struct
    {
       uint8  SwitchOff_StartTime;               // ��������ʱ��ʼ�Ŀ�ʼ��ʱ
    }PowerOff_StartTime_T;
    
    
    void Task_PowerOnOff(void);

#endif