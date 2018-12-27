/*=======================================================================
 *Subsystem:   ���
 *File:        DS3231.c
 *Author:      Wenming
 *Description: ͨ�ţ�
               �ӿڣ�PJ1��PJ0��
               �����ʣ�100KHz
 ========================================================================
 * History:    �޸���ʷ��¼�б�
 * 1. Date:
      Author:
      Modification:
========================================================================*/
#include  "DS3231.h"
#include  "TypeDefinition.h"
#include  "MC9S12XEP100.h"
#include  "derivative.h" 

Read_IIC_Time_T Read_IIC_Time;  
/*=======================================================================
 *������:      BCD2HEX(uint8)
 *����:        ������תʮ��������
 *����:        ��       
 *���أ�       ��
 *˵����       ��BCD��ת����16������,BCD������4λ2��������ʾ1��10������,����λ��ʮ��������
               ����λ��ʮ�����������ӣ�0x00100100;����λ����2������λ��4������24��
========================================================================*/
uint8 BCD2HEX(uint8 val) 
{
  uint8 value;
  value=val&0x0f;
  val>>=4;
  val&=0x0f;
  val*=10;
  value+=val;
  return value;
}

/*=======================================================================
 *������:      HEX2BCD(uint8)
 *����:        ������תʮ��������
 *����:        ��       
 *���أ�       ��
 *˵����       ������Ļ����෴��
========================================================================*/
uint8 HEX2BCD(uint8 val) 
{
    uint8 i,j,k;
    i=val/10;
    j=val%10;
    k=j+(i<<4);
    return k;
}

/*=======================================================================
 *������:      DS3231SN_INIT()
 *����:        ��IICģ���������
 *����:        year:��,month:��,day:��       
 *���أ�       ��
 *˵����       ������ʱ�䵥λ���㣬�ӳ�ʱ�䳤�ˣ��ɸġ�
========================================================================*/
void DS3231SN_INIT(uint8 year, uint8 month, uint8 week, uint8 day, uint8 hour, uint8 min) // ���������һ������ʱ����ʹ��һ��
{
   IIC_write(0xd0,0x00,0x00);    /*��������ʼ��Ϊ0*/
   delayus(10);
   IIC_write(0xd0,0x01,min);    /*�����ӳ�ʼ��Ϊ0*/
   delayus(10);
   IIC_write(0xd0,0x02,hour);    /*��Сʱ����ʼ��Ϊ0*/
   delayus(10);
   IIC_write(0xd0,0x03,day);     /*��������ʼ��Ϊ0*/
   delayus(10);
   IIC_write(0xd0,0x04,week);     /*�����ʼ��Ϊ0*/
   delayus(10);
   IIC_write(0xd0,0x05,month);   /*��������ʼ��Ϊ0*/
   delayus(10);
   IIC_write(0xd0,0x06,year);    /*��������ʼ��Ϊ0*/
   //delay(); 
}

/*=======================================================================
 *������:      DS3231_Read_Second()
 *����:        ������
 *����:        ��       
 *���أ�       ��
 *˵����       ��ȡ�����������59��
========================================================================*/
uint8 DS3231_Read_Second(void) 
{
  uint8 receivedata;
  receivedata=IIC_read(0xd0,0x00);
  receivedata=BCD2HEX(receivedata);
  return receivedata;
}
/*=======================================================================
 *������:      DS3231_Read_Minute()
 *����:        ������
 *����:        ��       
 *���أ�       ��
 *˵����       ��ȡ���������ֵ59��
========================================================================*/
uint8 DS3231_Read_Minute(void) 
{
  uint8 receivedata;
  receivedata=IIC_read(0xd0,0x01);
  receivedata=BCD2HEX(receivedata);
  return receivedata;
}

/*=======================================================================
 *������:      DS3231_Read_Hour()
 *����:        ��Сʱ��
 *����:        ��       
 *���أ�       ��
 *˵����       ��ȡСʱ�������ֵ��23Сʱ��
========================================================================*/
uint8 DS3231_Read_Hour(void) 
{
  uint8 receivedata;
  receivedata=IIC_read(0xd0,0x02);
  receivedata=BCD2HEX(receivedata);
  return receivedata;
}

/*=======================================================================
 *������:      DS3231_Read_Day()
 *����:        ������
 *����:        ��       
 *���أ�       ��
 *˵����       ��ȡ���������ֵ��31��
========================================================================*/
uint8 DS3231_Read_Day(void) 
{
  uint8 receivedata;
  receivedata=IIC_read(0xd0,0x04);
  receivedata=BCD2HEX(receivedata);
  return receivedata;
}

/*=======================================================================
 *������:      DS3231_Read_Month()
 *����:        ������
 *����:        ��       
 *���أ�       ��
 *˵����       ��ȡ�����������ֵ��12�£�
========================================================================*/
uint8 DS3231_Read_Month(void) 
{
  uint8 receivedata;
  receivedata=IIC_read(0xd0,0x05);
  receivedata=BCD2HEX(receivedata);
  return receivedata;
}

/*=======================================================================
 *������:      DS3231_Read_Year()
 *����:        ������
 *����:        ��       
 *���أ�       ��
 *˵����       ��ȡ���������ֵ��99�ꣻ
========================================================================*/
uint8 DS3231_Read_Year(void) 
{
  uint8 receivedata;
  receivedata = IIC_read(0xd0,0x06);
  receivedata = BCD2HEX(receivedata);
  return receivedata;
}

/*=======================================================================
 *������:      DS3231_Read_Year()
 *����:        ����ʱ��
 *����:        ��       
 *���أ�       ��
 *˵����       ��ȡ���������ֵ��99�ꣻ
========================================================================*/
 void DS3231_Read_Time(void) 
 {   
    uint8 receivedata;   
     
    receivedata=IIC_read(0xd0,0x01);
    receivedata=BCD2HEX(receivedata);
    Read_IIC_Time.IIC_Read_Minute = receivedata;
    delayus(10);
    
    receivedata=IIC_read(0xd0,0x02);
    receivedata=BCD2HEX(receivedata);
    Read_IIC_Time.IIC_Read_Hour = receivedata;
    delayus(10);
    
    receivedata=IIC_read(0xd0,0x04);
    receivedata=BCD2HEX(receivedata);
    Read_IIC_Time.IIC_Read_Day = receivedata;
    delayus(10);
    
    receivedata=IIC_read(0xd0,0x05);
    receivedata=BCD2HEX(receivedata);
    Read_IIC_Time.IIC_Read_Month = receivedata;
    delayus(10);
    
    receivedata=IIC_read(0xd0,0x06);
    receivedata=BCD2HEX(receivedata);
    Read_IIC_Time.IIC_Read_Year = receivedata;
 }

 
 
