/*=======================================================================
 *Subsystem:   ���
 *File:        Delay_Function.C
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    �޸���ʷ��¼�б�
 * 1. Date:
      Author:
      Modification:
========================================================================*/
#include  "Delay_Function.h"
#include  "TypeDefinition.h"
#include  "MC9S12XEP100.h"
#include  "derivative.h"

void delay(void) 
{
 unsigned int i;
 uint16 j;
 for(i=0;i<50000;i++)//;  /*�˴���ʱʱ������޸ģ�������ֵ��Ҫ����*/
   ;
}


/*10s��ʱ
void delay_init(void) 
{
  unsigned int i,j;
  for(j=0;j<55;j++)
  for(i=0;i<60000;i++)
  ;
}

void delay_init2(void) 
{
  unsigned int i,j;
  for(j=0;j<30;j++)
  for(i=0;i<60000;i++)
  ;
} */

void delayus(uint16 us) 
{
	  uint16 delayval;
	  delayval = us * 9;
	  while(delayval--);
}
