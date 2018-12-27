#ifndef _TASK_POWERONOFF_H_
#define _TASK_POWERONOFF_H_
    
    #include  "TypeDefinition.h"
    #include  "Port_Control.h"
    
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
    
    #define Relay_Screen_PORT              PA
    #define Relay_Screen_pin                4 

    void PositiveRelay_OFF(uint8);
    void PositiveRelay_ON(void);

#endif