
//������ҵ���
/*=======================================================================
 *Subsystem:   ���
 *File:        CAN.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _CAN_H_
#define _CAN_H_
  
  #include  "TypeDefinition.h"
   
  
  //CAN��ʼ��
  enum Init_CAN
  {
    //��ʼ��λ������Ϊ0
    Init_Normal_CAN = 0,
    //��ʼ��λ��ʧ�ܵĶ�Ӧ
    Init_Fault_CAN_BaudRate,
    Init_Fault_CAN_EnterState,
    Init_Fault_CAN_Unready,
    Init_Fault_CAN_Synchr
  };
  
  //CAN��������
  enum SendMsg_CAN
  {
    SendMsg_Normal = 0,
    SendMsg_Fault_Lenth,
    SendMsg_Fault_Synch,
    SendMsg_Fault_NoEmptyNode
  };
  
  //CAN��������
  enum GetMsg_CAN
  {
    GetMsg_Normal = 0,
    GetMsg_Fault_RFLG_RXF
  };
  
  //CAN���Ľṹ��
  typedef struct 
  {
    uint32 m_ID;      // msg���ͷ�ID
    uint8 m_IDE;      // ��չ֡Ϊ1����׼֡Ϊ0
    uint8 m_RTR;      // Զ��֡Ϊ1������֡Ϊ0
    uint8 m_data[8];  // ֡����
    uint8 m_dataLen;  // ֡���ݳ���
    uint8 m_priority; // �������ȼ� 
  }CANFRAME, *pCANFRAME; 

  

  //CAN0
  uint8 INIT_CAN0(uint16 Baud_Rate);
  uint8 SendMsg_CAN0(pCANFRAME sendFrame);
  uint8 GetMsg_CAN0(pCANFRAME receiveFrame);

  //CAN1  ���
  uint8 INIT_CAN1(uint16 Baud_Rate);
  uint8 SendMsg_CAN1(pCANFRAME sendFrame);
  uint8 GetMsg_CAN1(pCANFRAME receiveFrame);
  
  //CAN2  ���� 
  uint8 INIT_CAN2(uint16 Baud_Rate);
  uint8 SendMsg_CAN2(pCANFRAME sendFrame);
  uint8 GetMsg_CAN2(pCANFRAME receiveFrame);
  
  //CAN3
  uint8 INIT_CAN3(uint16 Baud_Rate);
  uint8 SendMsg_CAN3(pCANFRAME sendFrame);
  uint8 GetMsg_CAN3(pCANFRAME receiveFrame);

#endif  




