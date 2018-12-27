/*=======================================================================
 *Subsystem:   ���
 *File:        PLL.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _PLL_H_
#define _PLL_H_  

  #include"TypeDefinition.h"

  #define  BUS_CLOCK		   32000000	   //����Ƶ��
  #define  OSC_CLOCK		   16000000	   //����Ƶ��

  #define  BAUD1             115200 
  #define  BAUD              9600  
  
  typedef enum
  {
    Init_Normal_PLL = 0,
    Init_Fault_PLL_PLLCLK
  };

  uint8 Init_PLL(void) ; 

#endif 