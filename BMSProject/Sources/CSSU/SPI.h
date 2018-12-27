#ifndef SPI_H_
#define SPI_H_

  #include  "MC9S12XEP100.h"
  #include  "TypeDefinition.h"

  //SPI1ͨ����ƬѡIO�ں궨��
  #define  SS2                PTP_PTP3
  #define  SS2_Dir            DDRP_DDRP3

  //SPI1������ʼ��
  uint8  SPI1_Init(void);
  void  SPI1_Write(uint8 data); 
  uint8 SPI1_read(void);

  //SPI1���Ƶ���������������
  void wakeup_idle(void);
  void wakeup_sleep(void);
  //��ʱ����
  void delay_time(uint16 t);
  
#endif