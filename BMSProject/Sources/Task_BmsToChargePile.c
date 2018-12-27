  /*=======================================================================
 *Subsystem:   ���
 *File:        Task_BmsToChargePile.C
 *Author:      Wen Yuhao
 *Description: ͨ�ţ�
               �ӿڣ�
               �����ʣ�
 ========================================================================
 * History:    �޸���ʷ��¼�б�
 * 1. Date:
      Author:
      Modification:
========================================================================*/
 
 #include  "Task_BmsAndChargePile.h"
 #include  "Task_CurrentLimit.h"
 #include  "BattInfoParaStructure.h"
 #include  "Task_SOC.h"
 #include  "Task_FaultLevelJudge.h"
 
 BMSChargePile_T        BMSChargePile;
 BMSChargePile_State_T  BMSChargePile_State;
 
 
/*=======================================================================
 *������:      CAN_ToChargePileInit(void) 
 *����:        ����׮ͨ�ŵ�CANͨ����ʼ��
 *����:        ��       
 *���أ�       ��
 
 *˵����       
========================================================================*/ 
uint8 CAN_ToChargePileInit(void)
{
  uint8 state;
  state = INIT_CAN1(250);
  return(state);
}

/*=======================================================================
 *������:      CAN_ToUpMonitor(pCANFRAME canptr) 
 *����:        BMS information to UpMonitor
 *����:        ��       
 *���أ�       ��
 *˵����       BMS������Ϣ����λ����CANͨ��ѡ��
========================================================================*/ 
static
uint8 CAN_ToChargePile(pCANFRAME canptr)
{
  if(!SendMsg_CAN1(canptr))
  {
     return 0;  //����
  }  
  return 1;   //����
}

/*=======================================================================
 *������:      Task_BmsToChargePile(void) 
 *����:        BMS information to ChargePile
 *����:        ��       
 *���أ�       ��
 *˵����       BMS������Ϣ������
========================================================================*/ 
void Task_BmsToChargePile(void) 
{
  pCANFRAME BMS_to_ChargePile;
  
  BmsToChargePileInfo();
  
  BMS_to_ChargePile->m_IDE = 0;    //��׼֡
	BMS_to_ChargePile->m_RTR = 0;
	BMS_to_ChargePile->m_dataLen = 8;
	BMS_to_ChargePile->m_priority = 6; //?? 
  
  //0x111
  BMS_to_ChargePile->m_ID = 0x111;
 
  BMS_to_ChargePile->m_data[0] = (BMSChargePile.Volt_Max_ChargePile*10) >> 8;       //���������˵�ѹ���ֽ�   0.1V/bit
	BMS_to_ChargePile->m_data[1] = (uint8)(BMSChargePile.Volt_Max_ChargePile*10);                              
	BMS_to_ChargePile->m_data[2] = (BMSChargePile.Curr_Max_ChargePile*10)>> 8;        //����������������ֽ�   0.1A/bit   
	BMS_to_ChargePile->m_data[3] = (uint8)(BMSChargePile.Curr_Max_ChargePile*10); 
	BMS_to_ChargePile->m_data[4] = BMSChargePile.Control_ChargePile;                  //���Ƴ��׮    0:����    1:�ر�
	BMS_to_ChargePile->m_data[5] = (((0x01)&0x01) + \
	                                ((BMSChargePile_State.TempH_Cell<<1)&0x02) + \
	                                ((BMSChargePile_State.TempL_Cell<<2)&0x04) + \
	                                ((BMSChargePile_State.CurrH_Cell<<3)&0x08) + \
	                                ((BMSChargePile_State.Insul<<4)&0x10) + \
	                                ((BMSChargePile_State.BMSGetMsg<<5)&0x20) + \
	                                ((BMSChargePile_State.FaultFlag<<6)&0x40) + \
	                                ((0x01<<7)&0x80));
	BMS_to_ChargePile->m_data[6] = 0xFF;                 
	BMS_to_ChargePile->m_data[7] = 0xFF;       
  while(CAN_ToChargePile(BMS_to_ChargePile));
  delayus(100);
 
 //0x115
  BMS_to_ChargePile->m_ID = 0x115;
 
  BMS_to_ChargePile->m_data[0] = (BMSChargePile.VoltC_Max*1000) >> 8;      //������ߵ�ѹ���ֽ�   0.001V/bit
	BMS_to_ChargePile->m_data[1] = (uint8)(BMSChargePile.VoltC_Max*1000);                              
	BMS_to_ChargePile->m_data[2] = (BMSChargePile.VoltC_Min*1000)>> 8;       //������͵�ѹ���ֽ�   0.001V/bit 
	BMS_to_ChargePile->m_data[3] = (uint8)(BMSChargePile.VoltC_Min*1000); 
	BMS_to_ChargePile->m_data[4] = BMSChargePile.SOC*250;                    //SOC   0.4%/bit
	BMS_to_ChargePile->m_data[5] = BMSChargePile.Temp_Max;                   //����¶�     1��/bit  ƫ����:-40��
	BMS_to_ChargePile->m_data[6] = (BMSChargePile.VoltS*10)>>8;              //������ѹ���ֽ�     0.1V/bit
	BMS_to_ChargePile->m_data[7] = (uint8)(BMSChargePile.VoltS*10);       
  while(CAN_ToChargePile(BMS_to_ChargePile));
  delayus(100);
}






