#ifndef PGN_H
#define PGN_H
 
 
  
  //���PGN�궨��  
  #define RES    		 0x000000  // Ԥ������
  #define CRM    		 0x000100  // ������ʶ����
  #define BRM     	 0x000200  // BMS�복����ʶ����

  #define BCP			   0x000600  // �������س�����
  #define CTS			   0x000700  // ��������ʱ��ͬ����Ϣ
  #define CML     	 0x000800  // ��������������
  #define BRO   	   0x000900  // ��س��׼������״̬
  #define CRO   		 0x000A00  // �������׼������״̬

  #define BCL			   0x001000  // ��س������
  #define BCS		     0x001100  // ��س����״̬
  #define CCS			   0x001200  // �������״̬          
  #define BSM		     0x001300  // ��������״̬��Ϣ
  #define BMV		     0x001500  // ���嶯�����ص�ѹ
  #define BMT			   0x001600  // ���������¶�
  #define BSP				 0x001700  // ��������Ԥ������
  #define BST				 0x001900  // BMS��ֹ���
  #define CST   		 0x001A00  // ������ֹ���

  #define BSD			   0x001C00  // BMSͳ������
  #define CSD			   0x001D00  // ����ͳ������

  #define BEM     	 0x001E00  // BMS������
  #define CEM			   0x001F00  // ����������
  
  #define DM1        0x002000  //��ǰ������
  #define DM2        0x002100  //��ʷ������
  #define DM3        0x002200  //���׼������
  #define DM4        0x002300  //��ǰ����������/��λ
  #define DM5        0x002400  //��ʷ����������/��λ
  #define DM6        0x002500  //ͣ֡����

  #define CHM        0x002600  //�������
  #define BHM        0x002700  //��������

  #define PGN_NUM    29        //PGN������PGN����Ŀ


#endif
