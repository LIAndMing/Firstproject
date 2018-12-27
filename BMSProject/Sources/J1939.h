#ifndef _J1939_H_
#define _J1939_H_ 

  /*--------------------���峣��------------------*/   
  #define NUMBER_PDU_BUFFERS       8        //PDU��������ֽ���,һ��Ϊ8
  #define NUMBER_TRANS_RX_BUFFERS  128      //��������8�ֽں��������ֽ������ɰ�BMS����
  #define NUMBER_TRANS_TX_BUFFERS  128      //��������8�ֽں�������ֽ������ɰ�BMS����

  #define IN_BUFFER_SIZE 20        //���ջ�����������ݱ�����
  #define OUT_BUFFER_SIZE 20       //���ͻ�����������ݱ�����

  #define CAN_MAX_BYTE_COUNT 8

  #define NOTPRIMED 0              //����ȡ������
  #define PRIMED    1
  
  

  /*-----------------�������ڵ��ַ�궨��---------------*/
  #define NODEADDR         0xF4              //���������ڵ��ַ
  #define GLOBADDR         0xFF              //ȫ�ֽڵ��ַ
  #define NULLADDR         0xFE
  #define VCUADDRESS       0x27              //VCU�ڵ��ַ
  #define CHARGEADDRESS    0x56              //���ڵ��ַ
  #define CSSUADDRESS      0x97              //�Ӱ�ڵ��ַ
  #define INSULATIONADD    0x1A              //��Ե��ڵ��ַ
  

  /*-----------------��������ݽ���״̬---------------*/
  #define WAIT_FOR_MESSAGE               0         // State Zero: Waiting for a BAM or RTS data packet to arrive
  #define INIT_REASSEMBLE_STRUCTURE      1         // State One: Message reassemble structure and message timer are initialized.
  #define CHECK_PACKET                   2         // State Two: The incoming message is for this node.
  #define SEND_ABORT                     3         // State Three:  No more buffers to reassemble incoming message, Abort is sent.
  #define SEND_CTS_WITH_COUNT            4         // State Four: Buffers are available to reassemble incoming message, send CTS back to sender to indicate number of packets between next CTS (TRANSPORT_PACKET_COUNT). 
  #define WAIT_FOR_DATA                  5         // State Five: Waiting for DATA type packet from source.
  #define CHECK_TIMER                    6         // State Six:  Message timer is checked for expiration.
  #define RESET_REASSEMBLY_STRUCTURE     7         // State Seven: Message reassemble structure is reset.
  #define CHECK_DATA_PACKET              8         // State Eight: A DATA type packet has arrived and checked.
  #define SAVE_DATA                      9         // State Nine: The data is saved in the message reassemble buffer
  #define SEND_EOM                       10        // State Ten:  EOM message is sent.
  #define SEND_CTS                       11        // State Eleven: CTS is sent to trigger more data from the sender. 
  #define FILL_USER_MESSAGE              12        // State Twelve: The J1939_RX_MESSAGE_T data structure is filled for user application.


  /*-----------------��������ݷ���״̬---------------*/
  #define SEND_TP_CM                     0         //����TP_CM���������з�������,���͵�Ŀ�ĵط���RTS;���͹㲥����BAM 
  #define WAIT_FOR_CTS                   1         //�ȴ����շ�CTS ,ȷ�Ͻ����﷢�͵��ֽڱ�������
  #define SEND_TP_DT                     2         //�������ݱ���
  #define CHECK_TIMER1                   3         //���ʱ�����Ƿ�ʱ
  #define SEND_ABORT1                    4         //���ͷ������ӱ���
  #define WAIT_FOR_EOM                   5         //�ȴ��������ӱ���
  #define WAIT_AFTER_END                 6         //
  #define CHECK_TIMER2                   7  
  
  /*-----------------����㴫��Э��궨��---------------*/
  #define TP_CM                          0x00EC00  // 0x00EC00=60416,��������PGN
  #define TP_CM_RTS                      16
  #define TP_CM_CTS                      17         
  #define TP_CM_END_OF_MSG_ACK           19
  #define TP_CM_CONN_ABORT               255
  #define TP_CM_BAM                      32
  #define TP_DT                          0x00EB00  // 0x00EB00=60160
  #define TP_NONE                        0          
  
  /*-----------------���ݴ����ȴ���ʱ����---------------*/  
  #define TICK                           30        // �ж϶��ֽڽ����Ƿ�ʱ��20ms  
  
  
  /*--------------------���нṹ��-------------------*/ 
  
  typedef struct 
  {
    CANFRAME *pBase;
    uint8 Front;
    uint8 Rear;
    uint8 BufferLen;
    uint8 DataLen;
  }QUEUE_T, *pQUEUE_T;
  
  
   
  /*--------------------����8�ֽڵ����ݽṹ��-------------------*/ 
  typedef struct//�ײ㵽�����
  {
   PGN_T PGN;
   vuint8 data[NUMBER_TRANS_RX_BUFFERS]; //128
   vuint16 byte_count;
   vuint8 source_addr;
   vuint8 dest_addr;
  } J1939_RX_NETWork_T; //����㵽���������ݽӿڣ�����8�ֽڣ����޴���Э��
  //ת����PDU��ʽ���з���
  typedef struct
  {
   PGN_T     PGN;
   vuint8    data[8];  
   vuint16   byte_count;
   vuint8    priority;
   vuint8    dest_addr;
   vint8     status;
  }J1939_TX_PDU_T;   //��������PDU��ʽ���������㵽����������ת���ӿ�
  
  
  //���ݽ���
  typedef struct
  {
   PGN_T  PGN;
   vuint8 data[8];  
   vuint8 byte_count;
   vuint8 source_addr;
   vuint8 dest_addr;
  }J1939_RX_PDU_T;   //��������PDU��ʽ���������㵽����������ת���ӿ�
  
  typedef struct
  {
   uint8  CTS_Flag;
   PGN_T  PGN;
   vuint8 data[8];  
   vuint8 byte_count;
   vuint8 source_addr;
   vuint8 dest_addr;
  }J1939_CTS_T;   //����CTS�ṹ��
  
  typedef struct//����CTS��ENDofMsgACK֡
  {
   uint8  ENDofMsgACK_Flag;
   PGN_T  PGN;
   vuint8 data[8];  
   vuint8 byte_count;
   vuint8 source_addr;
   vuint8 dest_addr;
  }J1939_CENDofMsgACK_T;   //����ENDofMsgACK�ṹ��
  
  //���䵽Ӧ�ò�
  typedef struct
  {
   PGN_T  PGN;
   vuint8 status;
   vuint8 packet_number;
   vuint8 total_packet_number;
   vuint8 byte_count;
   vuint8 timer_counter;
   vuint8 source_addr;
   vuint8 dest_addr;
   vuint8 TP;
  }J1939_RX_STATE_MACHINE_T;     //��������ݴ��䴦��ṹ��ʽ������8�ֽڣ�������Э��ṹ����
  
  /*-----------------����������нṹ��---------------*/

  
  
  /*-----------------��������ݴ���Э���ڲ��ṹ��---------------*/
   //��������ݴ��䴦��ṹ��ʽ������8�ֽڣ�������Э��ṹ����



  typedef struct
  {
     PGN_T  PGN;
     vuint8 status;
     vuint8 packet_number;
     vuint8 total_packet_number;
     vuint8 total_rxpacket_number;
     vuint8 byte_count;
     vuint8 timer_counter;
     vuint16 timer1_counter;
     vuint8 source_addr;
     vuint8 dest_addr;
     vuint8 TP;
  }J1939_TX_STATE_MACHINE_T;     //��������ݴ��䴦��ṹ��ʽ������8�ֽڣ�������Э��ṹ����
   
   
  /*-----------------����㵽Ӧ�ò����ݴ���ṹ��---------------*/
 
  typedef struct
  {
     PGN_T PGN;
     vuint8 data[NUMBER_TRANS_TX_BUFFERS]; //128
     vuint16 byte_count;
     vuint8 priority;
     vuint8 dest_addr;
     vint8 status;
  }J1939_TX_MESSAGE_T;  //����㵽���������ݽӿڣ�С��8�ֽڣ����޴���Э��  

#endif
