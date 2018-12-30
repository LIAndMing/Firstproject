/*=======================================================================
 *Subsystem:   ���
 *File:        Task_UpMonitor.C
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

  #include  "Task_UpMonitor.h"
  #include  "CAN.h"  
  #include  "BattInfoConfig.h"
  #include  "FltLevcfg.h"
  #include  "DS3231_TimeGet.h"
  #include  "Task_EEEReadWrite.h"
  #include  "BMSCheckSelf.h"
  #include  "Port_Control.h"
  #include  "Task_CurrLimit.h"
  #include  "Task_SOCSOH.h"
  #include  "Task_FltLevJudg.h"
  #include  "WorkModeJudge.h"
  #include  "LTC6811_ConnectType.h"
  #include  "LTC6811_VoltCollect.h"
  #include  "LTC6811_TempCollect.h"
  #include  "LTC6811_OpWire.h"
  #include  "Task_InsulDetect.h"
  #include  "Task_PowerOnOff.h"
  #include  "Task_SysTimeGet.h"

BMSMonitor_Volt_T g_BMSMonitor_Volt;
BMSMonitor_Temp_T g_BMSMonitor_Temp;
BMSMonitor_Curr_T g_BMSMonitor_Curr;
BMSMonitor_SOC_T  g_BMSMonitor_SOC;
BMSMonitor_Insu_T g_BMSMonitor_Insul; 
BMSMonitor_SOH_T  g_BMSMonitor_SOH;
BMSMonitor_New_LT_T g_BMSMonitor_New_LT;

/*=======================================================================
 *������:      CAN_UpMonitorInit(void) 
 *����:        ��λ��ʹ�õ�CANͨ����ʼ��
 *����:        ��       
 *���أ�       ��
 
 *˵����       
========================================================================*/ 
uint8 CAN_UpMonitorInit(void)
{
  uint8 state;
  state = CAN2_Init(500);
  return(state);
}

/*=======================================================================
 *������:      CAN_ToUpMonitor(pCANFRAME canptr) 
 *����:        BMS information to UpMonitor
 *����:        ��       
 *���أ�       ��
 *˵����       BMS������Ϣ����λ����CANͨ��ѡ��
========================================================================*/ 
uint8 CAN_ToUpMonitor(pCANFRAME canptr)
{
  if(!CAN2_SendMsg(canptr))
  {
     return 0; 
  }  
  return 1;
}

/*=======================================================================
 *������:      UpMonitor_DelayTimeus(uint16 us)
 *����:        
 *����:              
 *���أ�       
 *˵����       
========================================================================*/
static
void UpMonitor_DelayTimeus(uint16 us) 
{
	  uint16 delayval;
	  delayval = us * 9;
	  while(delayval--);
}

/*=======================================================================
 *������:      Task_BmsToUpMonitor(void) 
 *����:        BMS information to UpMonitor
 *����:        ��       
 *���أ�       ��
 *˵����       BMS������Ϣ����λ����
========================================================================*/  
void Task_BMSToUpMonitor(void)
{  
  uint8 i;
  uint8 Positive;
  
  pCANFRAME BMS_to_Upmonitor;
  
  BMS_to_Upmonitor->m_IDE = 1;
	BMS_to_Upmonitor->m_RTR = 0;
	BMS_to_Upmonitor->m_dataLen = 8;
	BMS_to_Upmonitor->m_priority = 6; 
  
  if(g_MonitorBMS_Start.CellMessageSend_judge == 0x01)
  {
    //�������Ϣ
    //0x1800C0F4
    for(i=0;i<3;i++)
    { 
      BMS_to_Upmonitor->m_ID = 0x1800C0F4;     	
      switch(i)
      {
        case 0:
        	BMS_to_Upmonitor->m_data[0] = i;     //��� 0x00
        	BMS_to_Upmonitor->m_data[1] = CELL_TYPE;                                //������� 
        	BMS_to_Upmonitor->m_data[2] = (uint8)(CELL_RESIS_INTERNAL*10);          //������裬��λ��0.1m��/Bit  
        	BMS_to_Upmonitor->m_data[3] = ((uint16)(CELL_RESIS_INTERNAL*10)) >> 8; 
        	BMS_to_Upmonitor->m_data[4] = (uint8)(CELL_CAPACITY*100);               //�����������λ��0.01AH/Bit
        	BMS_to_Upmonitor->m_data[5] = ((uint16)(CELL_CAPACITY*100)) >> 8;
        	BMS_to_Upmonitor->m_data[6] = (uint8)CELL_LIFE_CYCLE;                   //ѭ��������1��/Bit
        	BMS_to_Upmonitor->m_data[7] = ((uint16)(CELL_LIFE_CYCLE)) >> 8;       
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          UpMonitor_DelayTimeus(100);
        break;
        
        case 1:
          BMS_to_Upmonitor->m_data[0] = i;     //��� 0x01
          BMS_to_Upmonitor->m_data[1] = (uint8)(CELL_VOLT_NOMINAL*10000);     //�����Ƶ�ѹ����λ��0.0001V/Bit  
          BMS_to_Upmonitor->m_data[2] = ((uint16)(CELL_VOLT_NOMINAL*10000)) >> 8;          
          BMS_to_Upmonitor->m_data[3] = (uint8)(CELL_VOLT_MAX*10000);    //��ߵ����ѹ����λ��0.0001V/Bi
          BMS_to_Upmonitor->m_data[4] = ((uint16)(CELL_VOLT_MAX*10000)) >> 8;             
          BMS_to_Upmonitor->m_data[5] = (uint8)(CELL_VOLT_MIN*10000);        //��͵����ѹ����λ��0.0001V/Bit
          BMS_to_Upmonitor->m_data[6] = ((uint16)(CELL_VOLT_MIN*10000)) >> 8;                  
          BMS_to_Upmonitor->m_data[7] = CELL_TEMP_MAX_DISCHARGE;   //�����߱����¶�
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          UpMonitor_DelayTimeus(100);
        break;
        
        case 2:
          BMS_to_Upmonitor->m_data[0] = i;     //��� 0x02
          BMS_to_Upmonitor->m_data[1] = (uint8)(F1_DISCHG_VOLTCD);          //�����ѹ�� 
          BMS_to_Upmonitor->m_data[2] = (uint16)(F1_DISCHG_VOLTCD) >> 8;            
          BMS_to_Upmonitor->m_data[3] = (uint8)F2_DISCHG_TEMPL;             //������ʹ���¶�
          BMS_to_Upmonitor->m_data[4] = (uint8)SYS_SERIES_YiDongLi;                  //���ϵͳ�ܴ���   
          BMS_to_Upmonitor->m_data[5] = (uint8)SYS_NUMBER_MODULE;           //���ϵͳģ����
          BMS_to_Upmonitor->m_data[6] = (uint8)SYS_NUMBER_BOX;              //���ϵͳ����
          BMS_to_Upmonitor->m_data[7] = (uint8)SYS_NUMBER_TEMP;             //���ϵͳ�¶ȼ��������
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          UpMonitor_DelayTimeus(100);
        break;
      }
    }

    //��ع��ϵȼ�
    //0x1810C0F4      ��ص�ѹ������ֵ��Ϣ
    for(i=0;i<6;i++)
    {
      BMS_to_Upmonitor->m_ID = 0x1810C0F4;
      switch(i)
      {
        //�����ѹ
        case 0 :
        	BMS_to_Upmonitor->m_data[0] = i;     //��� 0x00
        	BMS_to_Upmonitor->m_data[1] = (uint8)(g_BMSMonitor_Volt.Volt_Cell_High1);         //�����ѹ������ֵ1�� 
        	BMS_to_Upmonitor->m_data[2] = ((uint16)(g_BMSMonitor_Volt.Volt_Cell_High1)) >> 8;             
        	BMS_to_Upmonitor->m_data[3] = (uint8)(g_BMSMonitor_Volt.Volt_Cell_High2);         //�����ѹ������ֵ2��
        	BMS_to_Upmonitor->m_data[4] = ((uint16)(g_BMSMonitor_Volt.Volt_Cell_High2)) >> 8;       
        	BMS_to_Upmonitor->m_data[5] = 0xFF;//(uint8)(g_BMSMonitor_Volt.Single_Over3_Volt);         //�����ѹ������ֵ3��
        	BMS_to_Upmonitor->m_data[6] = 0xFF;//((uint16)(g_BMSMonitor_Volt.Single_Over3_Volt)) >> 8;       
        	BMS_to_Upmonitor->m_data[7] = 0xFF;       
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          UpMonitor_DelayTimeus(100); 
        break;
        //����Ƿѹ   
        case 1:
          BMS_to_Upmonitor->m_data[0] = i;     //��� 0x01
         	BMS_to_Upmonitor->m_data[1] = (uint8)(g_BMSMonitor_Volt.Volt_Cell_Low1);         //�����ѹ������ֵ1�� 
        	BMS_to_Upmonitor->m_data[2] = ((uint16)(g_BMSMonitor_Volt.Volt_Cell_Low1)) >> 8;             
        	BMS_to_Upmonitor->m_data[3] = (uint8)(g_BMSMonitor_Volt.Volt_Cell_Low2);         //�����ѹ������ֵ2��
        	BMS_to_Upmonitor->m_data[4] = ((uint16)(g_BMSMonitor_Volt.Volt_Cell_Low2)) >> 8;       
        	BMS_to_Upmonitor->m_data[5] = 0xFF;//(uint8)(g_BMSMonitor_Volt.Single_Under3_Volt);         //�����ѹ������ֵ3��
        	BMS_to_Upmonitor->m_data[6] = 0xFF;//((uint16)(g_BMSMonitor_Volt.Single_Under3_Volt)) >> 8;                  
          BMS_to_Upmonitor->m_data[7] = 0xFF;                               
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          UpMonitor_DelayTimeus(100);
        break;
        //ϵͳ��ѹ  
        case 2:
          BMS_to_Upmonitor->m_data[0] = i;     //��� 0x02
        	BMS_to_Upmonitor->m_data[1] = (uint8)(g_BMSMonitor_Volt.Volt_Sys_High1);         //ϵͳ��ѹ������ֵ1�� 
        	BMS_to_Upmonitor->m_data[2] = ((uint16)(g_BMSMonitor_Volt.Volt_Sys_High1)) >> 8;             
        	BMS_to_Upmonitor->m_data[3] = (uint8)(g_BMSMonitor_Volt.Volt_Sys_High2);         //ϵͳ��ѹ������ֵ2��
        	BMS_to_Upmonitor->m_data[4] = ((uint16)(g_BMSMonitor_Volt.Volt_Sys_High2)) >> 8;       
        	BMS_to_Upmonitor->m_data[5] = 0xFF;//(uint8)(Monitor_BMS_Volt.Pack_Over3_Volt);         //ϵͳ��ѹ������ֵ3��
        	BMS_to_Upmonitor->m_data[6] = 0xFF;//((uint16)(Monitor_BMS_Volt.Pack_Over3_Volt)) >> 8;       
        	BMS_to_Upmonitor->m_data[7] = 0xFF;
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          UpMonitor_DelayTimeus(100);
        break;
        //ϵͳǷѹ
        case 3:
          BMS_to_Upmonitor->m_data[0] = i;     //��� 0x03
        	BMS_to_Upmonitor->m_data[1] = (uint8)(g_BMSMonitor_Volt.Volt_Sys_Low1);         //ϵͳ��ѹ������ֵ1�� 
        	BMS_to_Upmonitor->m_data[2] = ((uint16)(g_BMSMonitor_Volt.Volt_Sys_Low1)) >> 8;             
        	BMS_to_Upmonitor->m_data[3] = (uint8)(g_BMSMonitor_Volt.Volt_Sys_Low2);         //ϵͳ��ѹ������ֵ2��
        	BMS_to_Upmonitor->m_data[4] = ((uint16)(g_BMSMonitor_Volt.Volt_Sys_Low2)) >> 8;       
        	BMS_to_Upmonitor->m_data[5] = 0xFF;//(uint8)(Monitor_BMS_Volt.Pack_Under3_Volt);         //ϵͳ��ѹ������ֵ3��
        	BMS_to_Upmonitor->m_data[6] = 0xFF;//((uint16)(Monitor_BMS_Volt.Pack_Under3_Volt)) >> 8;       
        	BMS_to_Upmonitor->m_data[7] = 0xFF;
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          UpMonitor_DelayTimeus(100);
        break;
        //����ѹ��
        case 4:
          BMS_to_Upmonitor->m_data[0] = i;     //��� 0x04
        	BMS_to_Upmonitor->m_data[1] = (uint8)(g_BMSMonitor_Volt.Volt_Cell_Diff1);         //�����ѹѹ����ֵ1�� 
        	BMS_to_Upmonitor->m_data[2] = ((uint16)(g_BMSMonitor_Volt.Volt_Cell_Diff1)) >> 8;             
        	BMS_to_Upmonitor->m_data[3] = 0xFF;//(uint8)(Monitor_BMS_Volt.Single_Diff2_Volt);         //�����ѹѹ����ֵ2��
        	BMS_to_Upmonitor->m_data[4] = 0xFF;//((uint16)(Monitor_BMS_Volt.Single_Diff2_Volt)) >> 8;       
        	BMS_to_Upmonitor->m_data[5] = 0xFF;//(uint8)(Monitor_BMS_Volt.Single_Diff3_Volt);         //�����ѹѹ����ֵ3��
        	BMS_to_Upmonitor->m_data[6] = 0xFF;//((uint16)(Monitor_BMS_Volt.Single_Diff3_Volt)) >> 8;       
        	BMS_to_Upmonitor->m_data[7] = 0xFF;
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          UpMonitor_DelayTimeus(100);
        break; 
        //ϵͳѹ��
        case 5:
          BMS_to_Upmonitor->m_data[0] = i;     //��� 0x05
        	BMS_to_Upmonitor->m_data[1] = 0xFF;//(uint8)(Monitor_BMS_Volt.System_Diff1_Volt);         //ϵͳ��ѹѹ����ֵ1�� 
        	BMS_to_Upmonitor->m_data[2] = 0xFF;//((uint16)(Monitor_BMS_Volt.System_Diff1_Volt)) >> 8;             
        	BMS_to_Upmonitor->m_data[3] = 0xFF;//(uint8)(Monitor_BMS_Volt.System_Diff2_Volt);         //ϵͳ��ѹѹ����ֵ2��
        	BMS_to_Upmonitor->m_data[4] = 0xFF;//((uint16)(Monitor_BMS_Volt.System_Diff2_Volt)) >> 8;       
        	BMS_to_Upmonitor->m_data[5] = 0xFF;//(uint8)(Monitor_BMS_Volt.System_Diff3_Volt);         //ϵͳ��ѹѹ����ֵ3��
        	BMS_to_Upmonitor->m_data[6] = 0xFF;//((uint16)(Monitor_BMS_Volt.System_Diff3_Volt)) >> 8;       
        	BMS_to_Upmonitor->m_data[7] = 0xFF;
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          UpMonitor_DelayTimeus(100);
        break; 
      }
    }

    //0x1811C0F4      ����¶ȱ�����ֵ��Ϣ
    for(i=0;i<4;i++)
    {
      BMS_to_Upmonitor->m_ID = 0x1811C0F4;
      switch(i)
      {
        //����о�¶� 
        case 0:
        	BMS_to_Upmonitor->m_data[0] = i;     //��� 0x00
        	BMS_to_Upmonitor->m_data[1] = (uint8)(g_BMSMonitor_Temp.Temp_Charge_High1);         //����о�¶ȹ��� 
        	BMS_to_Upmonitor->m_data[2] = (uint8)(g_BMSMonitor_Temp.Temp_Charge_High2);              
        	BMS_to_Upmonitor->m_data[3] = 0xFF;//(uint8)(Monitor_BMS_Temp.Charge_Over3_Temp);         
        	BMS_to_Upmonitor->m_data[4] = (uint8)(g_BMSMonitor_Temp.Temp_Charge_Low1);        //����о�¶ȹ��� 
        	BMS_to_Upmonitor->m_data[5] = (uint8)(g_BMSMonitor_Temp.Temp_Charge_Low2);         
        	BMS_to_Upmonitor->m_data[6] = 0xFF;//(uint8)(Monitor_BMS_Temp.Charge_Under3_Temp);        
        	BMS_to_Upmonitor->m_data[7] = 0xFF;       
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          UpMonitor_DelayTimeus(100); 
        break;
        //�ŵ��о�¶�   
        case 1:
          BMS_to_Upmonitor->m_data[0] = i;     //��� 0x01
         	BMS_to_Upmonitor->m_data[1] = (uint8)(g_BMSMonitor_Temp.Temp_DisCharge_High1);         //�ŵ��о�¶ȹ��� 
        	BMS_to_Upmonitor->m_data[2] = (uint8)(g_BMSMonitor_Temp.Temp_DisCharge_High2);             
        	BMS_to_Upmonitor->m_data[3] = 0xFF;//(uint8)(Monitor_BMS_Temp.DisCharge_Over3_Temp);         
        	BMS_to_Upmonitor->m_data[4] = (uint8)(g_BMSMonitor_Temp.Temp_DisCharge_Low1);        //�ŵ��о�¶ȹ��� 
        	BMS_to_Upmonitor->m_data[5] = (uint8)(g_BMSMonitor_Temp.Temp_DisCharge_Low2);         
        	BMS_to_Upmonitor->m_data[6] = 0xFF;//(uint8)(Monitor_BMS_Temp.DisCharge_Under3_Temp);                  
          BMS_to_Upmonitor->m_data[7] = 0xFF;                               
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          UpMonitor_DelayTimeus(100);
        break;
        //����²�   
        case 2:
          BMS_to_Upmonitor->m_data[0] = i;     //��� 0x02
        	BMS_to_Upmonitor->m_data[1] = (uint8)(g_BMSMonitor_Temp.Temp_Charge_Diff1);         //������²����
        	BMS_to_Upmonitor->m_data[2] = 0xFF;//(uint8)(Monitor_BMS_Temp.Charge_Diff2_Temp);             
        	BMS_to_Upmonitor->m_data[3] = 0xFF;//(uint8)(Monitor_BMS_Temp.Charge_Diff3_Temp);         
        	BMS_to_Upmonitor->m_data[4] = (uint8)(g_BMSMonitor_Temp.Temp_DisCharge_Diff1);      //�ŵ����²���� 
        	BMS_to_Upmonitor->m_data[5] = 0xFF;//(uint8)(Monitor_BMS_Temp.DisCharge_Diff2_Temp);       
        	BMS_to_Upmonitor->m_data[6] = 0xFF;//(uint8)(Monitor_BMS_Temp.DisCharge_Diff3_Temp);       
        	BMS_to_Upmonitor->m_data[7] = 0xFF;
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          UpMonitor_DelayTimeus(100);
        break;
        //���ǹ&BMS������ֵ
        case 3:
          BMS_to_Upmonitor->m_data[0] = i;     //��� 0x03
        	BMS_to_Upmonitor->m_data[1] = 0xFF;//(uint8)(Monitor_BMS_Temp.ChargeGun_Over1_Temp);         //���ǹ����
        	BMS_to_Upmonitor->m_data[2] = 0xFF;//(uint8)(Monitor_BMS_Temp.ChargeGun_Over2_Temp);             
        	BMS_to_Upmonitor->m_data[3] = 0xFF;//(uint8)(Monitor_BMS_Temp.ChargeGun_Over3_Temp);         
        	BMS_to_Upmonitor->m_data[4] = 0xFF;//(uint8)(Monitor_BMS_Temp.Chip_Over1_Temp);              //BMS����
        	BMS_to_Upmonitor->m_data[5] = 0xFF;//(uint8)(Monitor_BMS_Temp.Chip_Over2_Temp);         
        	BMS_to_Upmonitor->m_data[6] = 0xFF;//(uint8)(Monitor_BMS_Temp.Chip_Over3_Temp);       
        	BMS_to_Upmonitor->m_data[7] = 0xFF;
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          UpMonitor_DelayTimeus(100);
        break;
      }
    }

    //0x1812C0F4      ��ص�����ֵ��Ϣ
    for(i=0;i<3;i++)
    {
      BMS_to_Upmonitor->m_ID = 0x1812C0F4;
      switch(i)
      {
        //ϵͳ�ŵ���� 
        case 0:
        	BMS_to_Upmonitor->m_data[0] = i;     //��� 0x00
        	BMS_to_Upmonitor->m_data[1] = (uint8)(g_BMSMonitor_Curr.Current_DisCharge_High1);          
        	BMS_to_Upmonitor->m_data[2] = ((uint16)(g_BMSMonitor_Curr.Current_DisCharge_High1)) >> 8;              
        	BMS_to_Upmonitor->m_data[3] = (uint8)(g_BMSMonitor_Curr.Current_DisCharge_High2);         
        	BMS_to_Upmonitor->m_data[4] = ((uint16)(g_BMSMonitor_Curr.Current_DisCharge_High2)) >> 8;       
        	BMS_to_Upmonitor->m_data[5] = 0xFF;//(uint8)(Monitor_BMS_Current.DisCharge_Over3_Current);         
        	BMS_to_Upmonitor->m_data[6] = 0xFF;//((uint16)(Monitor_BMS_Current.DisCharge_Over3_Current)) >> 8;       
        	BMS_to_Upmonitor->m_data[7] = 0xFF;       
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          UpMonitor_DelayTimeus(100); 
        break;
        //ϵͳ������ 
        case 1:
          BMS_to_Upmonitor->m_data[0] = i;     //��� 0x01
         	BMS_to_Upmonitor->m_data[1] = (uint8)(g_BMSMonitor_Curr.Current_Charge_High1);         
        	BMS_to_Upmonitor->m_data[2] = ((uint16)(g_BMSMonitor_Curr.Current_Charge_High1)) >> 8;             
        	BMS_to_Upmonitor->m_data[3] = (uint8)(g_BMSMonitor_Curr.Current_Charge_High2);         
        	BMS_to_Upmonitor->m_data[4] = ((uint16)(g_BMSMonitor_Curr.Current_Charge_High2)) >> 8;         
        	BMS_to_Upmonitor->m_data[5] = 0xFF;//(uint8)(Monitor_BMS_Current.Charge_Over3_Current);         
        	BMS_to_Upmonitor->m_data[6] = 0xFF;//((uint16)(Monitor_BMS_Current.Charge_Over3_Current)) >> 8;                  
          BMS_to_Upmonitor->m_data[7] = 0xFF;                               
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          UpMonitor_DelayTimeus(100);
        break;
        //SOC��ֵ
        case 2:
          BMS_to_Upmonitor->m_data[0] = i;     //��� 0x02
        	BMS_to_Upmonitor->m_data[1] = 0xFF;//(uint8)(Monitor_BMS_Current.SOC_Low1);         //��SOC
        	BMS_to_Upmonitor->m_data[2] = 0xFF;//(uint8)(Monitor_BMS_Current.SOC_Low2);             
        	BMS_to_Upmonitor->m_data[3] = 0xFF;//(uint8)(Monitor_BMS_Current.SOC_Low3);         
        	BMS_to_Upmonitor->m_data[4] = 0xFF;//(uint8)(Monitor_BMS_Current.SOC_High1);        //��SOC
        	BMS_to_Upmonitor->m_data[5] = 0xFF;//(uint8)(Monitor_BMS_Current.SOC_High2);         
        	BMS_to_Upmonitor->m_data[6] = 0xFF;//(uint8)(Monitor_BMS_Current.SOC_High3);       
        	BMS_to_Upmonitor->m_data[7] = 0xFF;
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          UpMonitor_DelayTimeus(100);
        break;
      }
    }

    //0x1813C0F4     ��Ե������ֵ��Ϣ
    BMS_to_Upmonitor->m_ID = 0x1813C0F4;     	
    BMS_to_Upmonitor->m_data[0] = 0x00;     //���  0x00
    BMS_to_Upmonitor->m_data[1] = 0xFF;//(uint8)(Monitor_BMS_Insulation.Insulation_Resis1);         
    BMS_to_Upmonitor->m_data[2] = 0xFF;//((uint16)(Monitor_BMS_Insulation.Insulation_Resis1)) >> 8;             
    BMS_to_Upmonitor->m_data[3] = (uint8)(g_BMSMonitor_Insul.Insulation_Resis2);         
    BMS_to_Upmonitor->m_data[4] = ((uint16)(g_BMSMonitor_Insul.Insulation_Resis2)) >> 8;         
    BMS_to_Upmonitor->m_data[5] = 0xFF;//(uint8)(Monitor_BMS_Insulation.Insulation_Resis3);         
    BMS_to_Upmonitor->m_data[6] = 0xFF;//((uint16)(Monitor_BMS_Insulation.Insulation_Resis3)) >> 8;                  
    BMS_to_Upmonitor->m_data[7] = 0xFF;                               
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
    UpMonitor_DelayTimeus(100);

    g_MonitorBMS_Start.CellMessageSend_judge = 0x00;
  
    //0x1814C0F4      ��ص�����ֵ��Ϣ
    for(i=0;i<2;i++)
    {
      BMS_to_Upmonitor->m_ID = 0x1814C0F4;
      switch(i)
      {
        //���·ŵ���ѹ��
        case 0:
        	BMS_to_Upmonitor->m_data[0] = i;     //��� 0x00
        	BMS_to_Upmonitor->m_data[1] = (uint8)(g_BMSMonitor_New_LT.Voll_Sys_Low1_LT );          
        	BMS_to_Upmonitor->m_data[2] = ((uint16)(g_BMSMonitor_New_LT.Voll_Sys_Low1_LT )) >> 8;              
        	BMS_to_Upmonitor->m_data[3] = (uint8)(g_BMSMonitor_New_LT.Voll_Sys_Low2_LT);         
        	BMS_to_Upmonitor->m_data[4] = ((uint16)(g_BMSMonitor_New_LT.Voll_Sys_Low2_LT)) >> 8;       
        	BMS_to_Upmonitor->m_data[5] = 0xFF;
        	BMS_to_Upmonitor->m_data[6] = 0xFF;
        	BMS_to_Upmonitor->m_data[7] = 0xFF;       
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          UpMonitor_DelayTimeus(100); 
        break;
        //���·ŵ絥���
        case 1:
          BMS_to_Upmonitor->m_data[0] = i;     //��� 0x01
         	BMS_to_Upmonitor->m_data[1] = (uint8)(g_BMSMonitor_New_LT.Volt_Cell_Low1_LT);         
        	BMS_to_Upmonitor->m_data[2] = ((uint16)(g_BMSMonitor_New_LT.Volt_Cell_Low1_LT)) >> 8;             
        	BMS_to_Upmonitor->m_data[3] = (uint8)(g_BMSMonitor_New_LT.Volt_Cell_Low2_LT);         
        	BMS_to_Upmonitor->m_data[4] = ((uint16)(g_BMSMonitor_New_LT.Volt_Cell_Low2_LT)) >> 8;         
        	BMS_to_Upmonitor->m_data[5] = 0xFF;        
        	BMS_to_Upmonitor->m_data[6] = 0xFF;                  
          BMS_to_Upmonitor->m_data[7] = 0xFF;                               
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          UpMonitor_DelayTimeus(100);
        break;
      }
   }
}
  
  
  //ʵʱ����������Ϣ 
  //0x1820C0F4    ���ϵͳ�Լ켰ʱ����Ϣ
  for(i=0;i<3;i++)
  {
    BMS_to_Upmonitor->m_ID = 0x1820C0F4;
    switch(i)
    {
      case 0 :     //ϵͳʵʱʱ��
      	BMS_to_Upmonitor->m_data[0] = i;     //��� 0x00
      	BMS_to_Upmonitor->m_data[1] = Read_IIC_Time.IIC_Read_Year;     //ϵͳʵʱʱ��:�� 
      	BMS_to_Upmonitor->m_data[2] = Read_IIC_Time.IIC_Read_Month;    //ϵͳʵʱʱ��:��         
      	BMS_to_Upmonitor->m_data[3] = Read_IIC_Time.IIC_Read_Day;      //ϵͳʵʱʱ��:��
      	BMS_to_Upmonitor->m_data[4] = Read_IIC_Time.IIC_Read_Hour;     //ϵͳʵʱʱ��:ʱ  
      	BMS_to_Upmonitor->m_data[5] = Read_IIC_Time.IIC_Read_Minute;   //ϵͳʵʱʱ��:��
      	BMS_to_Upmonitor->m_data[6] = 0xFF;
      	BMS_to_Upmonitor->m_data[7] = 0xFF;       
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
        UpMonitor_DelayTimeus(100);
      break;
      
      case 1:    //ϵͳ����״̬��ʱ��
        BMS_to_Upmonitor->m_data[0] = i;     //��� 0x01
        BMS_to_Upmonitor->m_data[1] = g_WorkStateJudge.WorkState;    //ϵͳ����״̬��00 �ŵ磬01 ��䣬02����  
        BMS_to_Upmonitor->m_data[2] = (uint8)(g_SysTime.BMS_TotalRun_MiniteTime/60);    //BMSϵͳ����ʱ�䣨Сʱ��       
        BMS_to_Upmonitor->m_data[3] = (uint16)(g_SysTime.BMS_TotalRun_MiniteTime/60) >> 8;    
        BMS_to_Upmonitor->m_data[4] = 0xFF;    //BMSϵͳ��������ʱ��
        BMS_to_Upmonitor->m_data[5] = 0xFF;        
        BMS_to_Upmonitor->m_data[6] = 0xFF;    //BMSϵͳ�ϴ�����ʱ��              
        BMS_to_Upmonitor->m_data[7] = 0xFF;   
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
        UpMonitor_DelayTimeus(100);
      break;
      
      case 2:      //����Լ�״̬���Լ�ʧ��ԭ��
      
        BMS_to_Upmonitor->m_data[0] = i;     //��� 0x02
        BMS_to_Upmonitor->m_data[1] = BMSCheckSelf.Check_Self_state;   //����Լ�״̬  00 �Լ��У�01 �ɹ���02 ʧ��
        if(BMSCheckSelf.Check_Self_state == 1)
        {
          BMS_to_Upmonitor->m_data[2] = 0xFF;     //�Լ�ʧ��ԭ��      
          BMS_to_Upmonitor->m_data[3] = 0xFF;
          BMS_to_Upmonitor->m_data[4] = 0xFF; 
          BMS_to_Upmonitor->m_data[5] = 0xFF;
          BMS_to_Upmonitor->m_data[6] = 0xFF;
          BMS_to_Upmonitor->m_data[7] = 0xFF;
        }
        else if(BMSCheckSelf.Check_Self_state == 2)
        {
          BMS_to_Upmonitor->m_data[2] = (BMSCheckSelf.Check_Self & 0x01);     
          BMS_to_Upmonitor->m_data[3] = (BMSCheckSelf.Check_Self & 0x02);     
          BMS_to_Upmonitor->m_data[4] = (BMSCheckSelf.Check_Self & 0x04);     
          BMS_to_Upmonitor->m_data[5] = 0xFF;
          BMS_to_Upmonitor->m_data[6] = 0xFF;
          BMS_to_Upmonitor->m_data[7] = 0xFF;
        }
        
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
        UpMonitor_DelayTimeus(100);
      break;
    }
  }
  
  //0x1830C0F4    ���ϵͳ��ѹ��Ϣ
  for(i=0;i<2;i++)
  {
    BMS_to_Upmonitor->m_ID = 0x1830C0F4; 
    switch(i)
    {
      case 0:    //�����ѹ��ѹ��
      	BMS_to_Upmonitor->m_data[0] = i;     //��� 0x00
      	BMS_to_Upmonitor->m_data[1] = (uint8)(g_DataColletInfo.SysVolt_Total*0.001);     //�����ѹ�������ۼӣ�   0.1V�ֱ���
      	BMS_to_Upmonitor->m_data[2] = ((uint16)(g_DataColletInfo.SysVolt_Total*0.001))>>8;             
      	BMS_to_Upmonitor->m_data[3] = 0xFF; //(uint8)(SOC_DATA.BMSU_Volt*10);     //�����ѹ����ѹģ����ѹ��  0.1V�ֱ���
      	BMS_to_Upmonitor->m_data[4] = 0xFF; //((uint16)(SOC_DATA.BMSU_Volt*10)) >> 8;        
      	BMS_to_Upmonitor->m_data[5] = 0xFF; //(uint8)(abs(g_TempInfo.TotalVolt_V-SOC_DATA.BMSU_Volt)*10);  //ѹ�abs(�ۼӵ�ѹ-���Ե�ѹ)��    0.1V�ֱ���
      	BMS_to_Upmonitor->m_data[6] = 0xFF; //((uint16)(abs(g_TempInfo.TotalVolt_V-SOC_DATA.BMSU_Volt)*10)) >> 8;
      	BMS_to_Upmonitor->m_data[7] = 0xFF;       
        while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
        UpMonitor_DelayTimeus(100);
      break;
      
      case 1:    //�������/��͵�ѹ��ѹ��
        BMS_to_Upmonitor->m_data[0] = i;     //��� 0x01
        BMS_to_Upmonitor->m_data[1] = (uint8)(g_VoltInfo.CellVolt_Max);          //������ߵ�ѹ���ֱ��ʣ�0.0001V��
        BMS_to_Upmonitor->m_data[2] = ((uint16)(g_VoltInfo.CellVolt_Max)) >> 8;   
        BMS_to_Upmonitor->m_data[3] = (uint8)(g_VoltInfo.CellVolt_Min);          //������͵�ѹ���ֱ��ʣ�0.0001V)   
        BMS_to_Upmonitor->m_data[4] = ((uint16)(g_VoltInfo.CellVolt_Min)) >> 8;
        BMS_to_Upmonitor->m_data[5] = (uint8)(g_VoltInfo.CellVolt_Diff);                //����ѹ��ֱ��ʣ�0.0001V) 
        BMS_to_Upmonitor->m_data[6] = ((uint16)(g_VoltInfo.CellVolt_Diff)) >> 8;             
        BMS_to_Upmonitor->m_data[7] = 0xFF;   
        while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
        UpMonitor_DelayTimeus(100);
      break;
    }
  }
   
  //0x1840C0F4    ���ϵͳ�¶���Ϣ
  for(i=0;i<2;i++)
  {
    BMS_to_Upmonitor->m_ID = 0x1840C0F4;
    switch(i)
    {
      case 0:     //�������/����¶ȼ��²�
      	BMS_to_Upmonitor->m_data[0] = i;     //��� 0x00
      	BMS_to_Upmonitor->m_data[1] = (uint8)(g_TempInfo.CellTemp_Max);     //����������¶�   1��ֱ���
      	BMS_to_Upmonitor->m_data[2] = (uint8)(g_TempInfo.CellTemp_Min);     //����������¶�   1��ֱ���             
      	BMS_to_Upmonitor->m_data[3] = (uint8)(g_TempInfo.CellTemp_Diff + 40);    //�������²�  1��ֱ���
      	BMS_to_Upmonitor->m_data[4] = 0xFF;        
      	BMS_to_Upmonitor->m_data[5] = 0xFF;  
      	BMS_to_Upmonitor->m_data[6] = 0xFF;
      	BMS_to_Upmonitor->m_data[7] = 0xFF;       
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
        UpMonitor_DelayTimeus(100);
      break;
       /*
      case 1:    //�����¶ȼ�����/�����ǹ�¶�
        BMS_to_Upmonitor->m_data[0] = i;     //��� 0x01
        BMS_to_Upmonitor->m_data[1] = 0xFF;  //(uint8)(ChageAndBMU_Temp.BMU_Temp);           //�����¶�   1��ֱ���
        BMS_to_Upmonitor->m_data[2] = 0xFF;  //������ǹ�¶�1 
        BMS_to_Upmonitor->m_data[3] = 0xFF;  //������ǹ�¶�2
        #if(Chag_Temp_Enable == 1)            
        BMS_to_Upmonitor->m_data[4] = ChageAndBMU_Temp.Chage_Temp1;  //�����ǹ�¶�1
        BMS_to_Upmonitor->m_data[5] = ChageAndBMU_Temp.Chage_Temp2;  //�����ǹ�¶�2
        #else
        BMS_to_Upmonitor->m_data[4] = 0xFF;  //�����ǹ�¶�1
        BMS_to_Upmonitor->m_data[5] = 0xFF;  //�����ǹ�¶�2
        #endif               
        BMS_to_Upmonitor->m_data[6] = 0xFF;  //�����ǹ�¶�3           
        BMS_to_Upmonitor->m_data[7] = 0xFF;  //�����ǹ�¶�4 
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
        UpMonitor_DelayTimeus(100);
      break;*/
    }
  }
  
  //0x1850C0F4    ���ϵͳ������Ϣ
  BMS_to_Upmonitor->m_ID = 0x1850C0F4;     	
	BMS_to_Upmonitor->m_data[0] = (uint8)((g_DataColletInfo.DataCollet_Current_Filter + 750)*10);        //��������ֵ   0.1A�ֱ���  ƫ������-750
	BMS_to_Upmonitor->m_data[1] = ((uint16)((g_DataColletInfo.DataCollet_Current_Filter + 750)*10))>>8;
	BMS_to_Upmonitor->m_data[2] = (uint8)((CurrLimit.Curr_Charge_Cons + 750)*10);     //����������ֵ   0.1A�ֱ���  ƫ������-750  ��δд��          
	BMS_to_Upmonitor->m_data[3] = ((uint16)((CurrLimit.Curr_Charge_Cons + 750)*10))>>8;
	BMS_to_Upmonitor->m_data[4] = 0xFF;  //(uint8)((Current_Limit.ConstantDischargeCurrent + 750)*10);     //�ŵ��������ֵ   0.1A�ֱ���  ƫ������-750   ��δд��
	BMS_to_Upmonitor->m_data[5] = 0xFF;  //((uint16)((Current_Limit.ConstantDischargeCurrent + 750)*10))>>8;  
	BMS_to_Upmonitor->m_data[6] = 0xFF;  //(uint8)((Current_Limit.FeedbackCurrent + 750)*10);          //�������������ֵ   0.1A�ֱ���  ƫ������-750   ��δд��
	BMS_to_Upmonitor->m_data[7] = 0xFF;  //((uint16)((Current_Limit.FeedbackCurrent + 750)*10))>>8;       
  while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
  UpMonitor_DelayTimeus(100);

  //0x1860C0F4    ���ϵͳ��Ե��Ϣ
  BMS_to_Upmonitor->m_ID = 0x1860C0F4;     	
  BMS_to_Upmonitor->m_data[0] = 0xFF;//(uint8)(InsulInfo.insulation_Vposit*10);    //���Եص�ѹ   0.1V�ֱ���
  BMS_to_Upmonitor->m_data[1] = 0xFF;//((uint16)(InsulInfo.insulation_Vposit*10))>>8;           
  BMS_to_Upmonitor->m_data[2] = 0xFF;//(uint8)(InsulInfo.insulation_Vnegt*10);           //���Եص�ѹ   0.1V�ֱ���
  BMS_to_Upmonitor->m_data[3] = 0xFF;//((uint16)(InsulInfo.insulation_Vnegt*10))>>8;              
  BMS_to_Upmonitor->m_data[4] = (uint8)(g_InsulInfo.Insul_Resis_Pos*10);           //���Եص���ֵ   0.1V�ֱ���
  BMS_to_Upmonitor->m_data[5] = ((uint16)(g_InsulInfo.Insul_Resis_Pos*10))>>8;               
  BMS_to_Upmonitor->m_data[6] = (uint8)(g_InsulInfo.Insul_Resis_Neg*10);            //���Եص���ֵ  0.1V�ֱ��ʣ�δд��
  BMS_to_Upmonitor->m_data[7] = ((uint16)(g_InsulInfo.Insul_Resis_Neg*10))>>8;
  while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
  UpMonitor_DelayTimeus(100);
     
  //0x1870C0F4    ���SOC��SOH��Ϣ
  for(i=0;i<2;i++)
  {
    BMS_to_Upmonitor->m_ID = 0x1870C0F4;
    switch(i)
    {
      case 0:     //SOC,SOC,���γ�/�ŵ���
      	BMS_to_Upmonitor->m_data[0] = i;     //��� 0x00
      	BMS_to_Upmonitor->m_data[1] = (uint8)(g_SOCInfo.SOC_ValueRead*100);      //SOC_Read      1%�ֱ���
      	BMS_to_Upmonitor->m_data[2] = (uint8)(g_SOCInfo.SOC_ValueVoltGet*100);      //SOC_Volt      1��ֱ���             
      	BMS_to_Upmonitor->m_data[3] = (uint8)(g_BMSMonitor_SOH.SOH*100);     //SOH           1%�ֱ���
      	BMS_to_Upmonitor->m_data[4] = 0xFF;                                     //���γ����
      	BMS_to_Upmonitor->m_data[5] = 0xFF;  
      	BMS_to_Upmonitor->m_data[6] = (uint8)(EnergyInfo.Energy_Once_DisCharge*10);    //���ηŵ���
      	BMS_to_Upmonitor->m_data[7] = ((uint16)(EnergyInfo.Energy_Once_DisCharge*10))>>8;        
        while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
        UpMonitor_DelayTimeus(100);
      break;
      
      case 1:    //�����¶ȼ�����/�����ǹ�¶�
        BMS_to_Upmonitor->m_data[0] = i;      //��� 0x01
        BMS_to_Upmonitor->m_data[1] = (uint8)(EnergyInfo.Energy_Total_Charge*10);            //�ۼƳ�����   0.1kWh�ֱ���
        BMS_to_Upmonitor->m_data[2] = ((uint16)(EnergyInfo.Energy_Total_Charge*10)) >> 8; 
        BMS_to_Upmonitor->m_data[3] = ((uint32)(EnergyInfo.Energy_Total_Charge*10)) >> 16;             
        BMS_to_Upmonitor->m_data[4] = (uint8)(EnergyInfo.Energy_Total_DisCharge*10);           //�ۼƷŵ����   0.1kWh�ֱ���
        BMS_to_Upmonitor->m_data[5] = ((uint16)(EnergyInfo.Energy_Total_DisCharge*10)) >> 8;              
        BMS_to_Upmonitor->m_data[6] = ((uint32)(EnergyInfo.Energy_Total_DisCharge*10)) >> 16;         
        BMS_to_Upmonitor->m_data[7] = 0xFF;
        while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
        UpMonitor_DelayTimeus(100);
      break;
    }
  }
  
  //0x1880C0F4    ��ع�����Ϣ
  for(i=0;i<2;i++)
  {
    Positive = Port_StateGet(Relay_Positive_PORT,Relay_Positive_pin);
    BMS_to_Upmonitor->m_ID = 0x1880C0F4;
    switch(i)
    {
      case 0:     
      	BMS_to_Upmonitor->m_data[0] = i;     //���  0x00
      	#if(ENABLE_RELAYADHESION_JUDGE == 1)    //�̵����жϹ���ʹ��
      	  BMS_to_Upmonitor->m_data[1] = (Positive & 0x03) + ((0x01 << 2) & 0x0C) + ((0x01 << 4) & 0x30) + ((0x01 << 6) & 0xC0);      //�̵���״̬ 0 �򿪣�1�ر�
      	  BMS_to_Upmonitor->m_data[2] = 0xFF;              
        	BMS_to_Upmonitor->m_data[3] = (g_Flt_Charge.Level_Volt_Cell_High & 0x03) + ((g_Flt_DisChg.Level_Volt_Cell_Low  << 2) & 0x0C) + ((g_Flt_DisChg.Level_Volt_Cell_Diff_High << 4) & 0x30) + ((g_Flt_Charge.Level_Volt_Sys_High  << 6) & 0xC0);     //�����ع�ѹ/Ƿѹ/ѹ�������ѹ����
        	BMS_to_Upmonitor->m_data[4] = (g_Flt_DisChg.Level_Volt_Sys_Low  & 0x03) + ((g_Flt_DisChg.Level_Insul << 2) & 0x0C) + ((g_Flt_DisChg.Level_Temp_High << 4) & 0x30) + ((g_Flt_DisChg.Level_Temp_Low << 6) & 0xC0);     //��ѹ���ͣ���Ե���ϣ��ŵ��¶ȹ���/����
        	BMS_to_Upmonitor->m_data[5] = (g_Flt_DisChg.Level_Temp_Diff_High & 0x03) + ((g_Flt_Charge.Level_Temp_High<< 2) & 0x0C) + ((g_Flt_Charge.Level_Temp_Low << 4) & 0x30) + ((g_Flt_Charge.Level_Temp_Diff_High << 6) & 0xC0);   //�ŵ��²���󣬳���¶ȹ���/���ͣ�����²����
        	BMS_to_Upmonitor->m_data[6] = (0x00 & 0x03) + ((0x00 << 2) & 0x0C) + ((g_Flt_Charge.Level_Current_Charge_High << 4) & 0x30) + ((g_Flt_DisChg.Level_Current_DisCharge_High << 6) & 0xC0);   //SOC�ߣ�SOC�ͣ����������󣬷ŵ��������                                     
        	BMS_to_Upmonitor->m_data[7] = (0x00 & 0x03) + ((0x00 << 2) & 0x0C) + ((0x00 << 4) & 0x30) + (0b11 << 6) ;  //���ǹ�¶ȹ��ߣ�δд��������Ӵ����¶ȹ��ߣ�δд������ѹ��������       
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          UpMonitor_DelayTimeus(100);
        #else                         //�̵����жϹ���δʹ��
          BMS_to_Upmonitor->m_data[1] = 0x00;
      	  BMS_to_Upmonitor->m_data[2] = 0xFF;              
        	BMS_to_Upmonitor->m_data[3] = 0x00;  //�����ع�ѹ/Ƿѹ/ѹ�������ѹ����
        	BMS_to_Upmonitor->m_data[4] = 0x00;  //��ѹ���ͣ���Ե���ϣ��ŵ��¶ȹ���/����
        	BMS_to_Upmonitor->m_data[5] = 0x00;  //�ŵ��²���󣬳���¶ȹ���/���ͣ�����²����
        	BMS_to_Upmonitor->m_data[6] = 0x00;  //SOC�ߣ�SOC�ͣ����������󣬷ŵ��������                                     
        	BMS_to_Upmonitor->m_data[7] = 0x00;  //���ǹ�¶ȹ��ߣ�����Ӵ����¶ȹ��ߣ���ѹ��������       
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
          UpMonitor_DelayTimeus(100);
        #endif
      	
      break;
      
      case 1:    
        BMS_to_Upmonitor->m_data[0] = i;      //��� 0x01
        BMS_to_Upmonitor->m_data[1] = 0xFF;   //BMSоƬ�¶ȹ���
        BMS_to_Upmonitor->m_data[2] = 0xFF;//(DiscFlt.HIVL_ECT0_Fault & 0x01) + ((DiscFlt.HIVL_ECT1_Fault << 1) & 0x02) + ((DiscFlt.HIVL_ECT2_Fault << 2) & 0x04) + ((DiscFlt.HIVL_ECT3_Fault << 3) & 0x08) + (0b11111 << 4) ;      //��������
        //#if(SysModuleNum<2)
          BMS_to_Upmonitor->m_data[3] = ((State_Offline.CSSU1) & 0x01) + ((State_Offline.VCU << 1) & 0x02) + ((State_Offline.HVU << 2) & 0x04) + ((0x01 << 3) & 0x08) + (0b11111 << 4);     //ͨ�Ź���  0000 ������0001 CSSU���ߣ�0010 VUC���ߣ�0100 HVU���ߣ�1000 TBOX����(��ʱû����Ϊ0x01)          
       /* #elif(SysModuleNum<3)
          BMS_to_Upmonitor->m_data[3] = ((State_Offline.CSSU1 | State_Offline.CSSU2) & 0x01) + ((State_Offline.VCU << 1) & 0x02) + ((State_Offline.HVU << 2) & 0x04) + ((0x01 << 3) & 0x08) + (0b11111 << 4);     //ͨ�Ź���  0000 ������0001 CSSU���ߣ�0010 VUC���ߣ�0100 HVU���ߣ�1000 TBOX����(��ʱû����Ϊ0x01)          
        #elif(SysModuleNum<4)
          BMS_to_Upmonitor->m_data[3] = ((State_Offline.CSSU1 | State_Offline.CSSU2 | State_Offline.CSSU3) & 0x01) + ((State_Offline.VCU << 1) & 0x02) + ((State_Offline.HVU << 2) & 0x04) + ((0x01 << 3) & 0x08) + (0b11111 << 4);     //ͨ�Ź���  0000 ������0001 CSSU���ߣ�0010 VUC���ߣ�0100 HVU���ߣ�1000 TBOX����(��ʱû����Ϊ0x01)          
        #elif(SysModuleNum<5)
          BMS_to_Upmonitor->m_data[3] = ((State_Offline.CSSU1 | State_Offline.CSSU2 | State_Offline.CSSU3 | State_Offline.CSSU4) & 0x01) + ((State_Offline.VCU << 1) & 0x02) + ((State_Offline.HVU << 2) & 0x04) + ((0x01 << 3) & 0x08) + (0b11111 << 4);     //ͨ�Ź���  0000 ������0001 CSSU���ߣ�0010 VUC���ߣ�0100 HVU���ߣ�1000 TBOX����(��ʱû����Ϊ0x01)          
        #elif(SysModuleNum<6)
          BMS_to_Upmonitor->m_data[3] = ((State_Offline.CSSU1 | State_Offline.CSSU2 | State_Offline.CSSU3 | State_Offline.CSSU4 | State_Offline.CSSU5) & 0x01) + ((State_Offline.VCU << 1) & 0x02) + ((State_Offline.HVU << 2) & 0x04) + ((0x01 << 3) & 0x08) + (0b11111 << 4);     //ͨ�Ź���  0000 ������0001 CSSU���ߣ�0010 VUC���ߣ�0100 HVU���ߣ�1000 TBOX����(��ʱû����Ϊ0x01)          
        #elif(SysModuleNum<7)
          BMS_to_Upmonitor->m_data[3] = ((State_Offline.CSSU1 | State_Offline.CSSU2 | State_Offline.CSSU3 | State_Offline.CSSU4 | State_Offline.CSSU5 | State_Offline.CSSU6) & 0x01) + ((State_Offline.VCU << 1) & 0x02) + ((State_Offline.HVU << 2) & 0x04) + ((0x01 << 3) & 0x08) + (0b11111 << 4);     //ͨ�Ź���  0000 ������0001 CSSU���ߣ�0010 VUC���ߣ�0100 HVU���ߣ�1000 TBOX����(��ʱû����Ϊ0x01)          
        #endif */
        BMS_to_Upmonitor->m_data[4] = 0xFF;           
        BMS_to_Upmonitor->m_data[5] = 0xFF;              
        BMS_to_Upmonitor->m_data[6] = 0xFF;         
        BMS_to_Upmonitor->m_data[7] = 0xFF;
        while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
        UpMonitor_DelayTimeus(100);
      break;
    }
  }
  /*
  //1890C0F4 BMS�������Ϣ
  for(i=0;i<2;i++)
  {
    BMS_to_Upmonitor->m_ID = 0x1890C0F4;
    switch(i)
    {
      case 0:     
        //BMS
      	BMS_to_Upmonitor->m_data[0] = i;     //��� 0x00
      	//�����ֹԭ��
      	BMS_to_Upmonitor->m_data[1] = (Charge_Error.BMS_ChagInf.Upto_GivenSOC & 0x03) + ((Charge_Error.BMS_ChagInf.Upto_GivenSysVolt << 2) & 0x0C) + ((Charge_Error.BMS_ChagInf.Upto_GivenCellVolt << 4) & 0x30) + ((Charge_Error.BMS_ChagInf.Charger_Suspend << 6) & 0xC0);  
      	//����ԭ��    
      	BMS_to_Upmonitor->m_data[2] = (Charge_Error.BMS_ChagInf.Check_Self & 0x03) + ((Charge_Error.BMS_ChagInf.Cur_Mismatch << 2) & 0x0C) + ((Charge_Error.BMS_ChagInf.Volt_Mismatch << 4) & 0x30) + ((Charge_Error.BMS_ChagInf.Jueyuan_Fault << 6) & 0xC0);                    
      	BMS_to_Upmonitor->m_data[3] = (Charge_Error.BMS_ChagInf.Connect_Overtemp & 0x03) + ((Charge_Error.BMS_ChagInf.BMS_Overtemp << 2) & 0x0C) + ((Charge_Error.BMS_ChagInf.Sys_Overtime << 4) & 0x30) + ((Charge_Error.BMS_ChagInf.Relay_Off << 6) & 0xC0); 
      	BMS_to_Upmonitor->m_data[4] = (Charge_Error.BMS_ChagInf.Connect_CC2_Fault & 0x03) + ((Charge_Error.BMS_ChagInf.Other_Fault << 2) & 0x0C) + ((Charge_Error.BMS_ChagInf.Chag_OverCur << 4) & 0x30) + ((Charge_Error.BMS_ChagInf.Volt_Error << 6) & 0xC0);  
        //��ʱ
      	BMS_to_Upmonitor->m_data[5] = (Charge_Error.BMS_Connect_Overtime.CRM00_Overtime & 0x03) + ((Charge_Error.BMS_Connect_Overtime.CRMAA_Overtime << 2) & 0x0C) + ((Charge_Error.BMS_Connect_Overtime.CTSCML_Overtime << 4) & 0x30) + ((Charge_Error.BMS_Connect_Overtime.CRO_Overtime << 6) & 0xC0);   
      	BMS_to_Upmonitor->m_data[6] = (Charge_Error.BMS_Connect_Overtime.CCS_Overtime & 0x03) + ((Charge_Error.BMS_Connect_Overtime.CST_Overtime << 2) & 0x0C) + ((Charge_Error.BMS_Connect_Overtime.CSD_Overtime << 4) & 0x30) + ((0b00 << 6) & 0xC0);
      	BMS_to_Upmonitor->m_data[7] = 0xFF;       
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
        UpMonitor_DelayTimeus(100);
      break;
      
      case 1:   
        //����
      	BMS_to_Upmonitor->m_data[0] = i;     //��� 0x01
      	//�����ֹԭ��
      	BMS_to_Upmonitor->m_data[1] = (Charge_Error.Charger_ChagInf.Normal_Suspend & 0x03) + ((Charge_Error.Charger_ChagInf.Artificial_Suspend << 2) & 0x0C) + ((Charge_Error.Charger_ChagInf.Fault_Suspend << 4) & 0x30) + ((Charge_Error.Charger_ChagInf.BMS_Suspend << 6) & 0xC0);  
      	//����ԭ��    
      	BMS_to_Upmonitor->m_data[2] = (Charge_Error.Charger_ChagInf.Charger_Overtemp & 0x03) + ((Charge_Error.Charger_ChagInf.Chag_Connect << 2) & 0x0C) + ((Charge_Error.Charger_ChagInf.Charger_Inner_Overtemp << 4) & 0x30) + ((Charge_Error.Charger_ChagInf.Energy_Transfer << 6) & 0xC0);                    
      	BMS_to_Upmonitor->m_data[3] = (Charge_Error.Charger_ChagInf.Charger_Emergency_Stop & 0x03) + ((Charge_Error.Charger_ChagInf.Other_Fault << 2) & 0x0C) + ((Charge_Error.Charger_ChagInf.Cur_Error << 4) & 0x30) + ((Charge_Error.Charger_ChagInf.Volt_Error << 6) & 0xC0); 
      	BMS_to_Upmonitor->m_data[4] = 0xFF;  
        //��ʱ
      	BMS_to_Upmonitor->m_data[5] = (Charge_Error.Charger_Connect_Overtime.BRM_Overtime & 0x03) + ((Charge_Error.Charger_Connect_Overtime.BCP_Overtime << 2) & 0x0C) + ((Charge_Error.Charger_Connect_Overtime.BRO_Overtime << 4) & 0x30) + ((Charge_Error.Charger_Connect_Overtime.BCS_Overtime << 6) & 0xC0);   
      	BMS_to_Upmonitor->m_data[6] = (Charge_Error.Charger_Connect_Overtime.BCL_Overtime & 0x03) + ((Charge_Error.Charger_Connect_Overtime.BST_Overtime << 2) & 0x0C) + ((Charge_Error.Charger_Connect_Overtime.BSD_Overtime << 4) & 0x30) + ((0b00 << 6) & 0xC0);
      	BMS_to_Upmonitor->m_data[7] = 0xFF; 
          while(CAN_ToUpMonitor(BMS_to_Upmonitor)); 
        UpMonitor_DelayTimeus(100);
      break;
    }
  }
  */
   /*
  //�޸ĺ�
  //������֡����CANpro�����룺0xC4CE07A0�������룺000F0000
  //0x1899C0F4�����ݲ��Ա��ģ�,CANpro�����룺0xC4CE07A0,�����룺0x00000000
  BMS_to_Upmonitor.m_ID = 0x1899C0F4;
	BMS_to_Upmonitor.m_data[0] = (uint8)(EnergyInfo.Energy_Total_Charge*10);       //�Ӱ���ѹֵ���Ӱ���ѹ�ܺͣ�0.1V�ֱ���
	BMS_to_Upmonitor.m_data[1] = ((uint16)(EnergyInfo.Energy_Total_Charge*10))>>8;
	BMS_to_Upmonitor.m_data[2] = (uint8)(EnergyInfo.Energy_Total_DisCharge*10);                //��ѹģ���ܵ�ѹֵ�� 0.1V�ֱ���
	BMS_to_Upmonitor.m_data[3] = ((uint16)(EnergyInfo.Energy_Total_DisCharge*10)) >> 8; 
	BMS_to_Upmonitor.m_data[4] = (uint8)g_Batt_VoltMesg.Single_Max_Volt;        //������ߵ�ѹ���ֱ��ʣ�0.0001V��
	BMS_to_Upmonitor.m_data[5] = (g_Batt_VoltMesg.Single_Max_Volt>>8); 
	BMS_to_Upmonitor.m_data[6] = (uint8)(g_Batt_VoltMesg.Single_Min_Volt);      //������͵�ѹ���ֱ��ʣ�0.0001V)
	BMS_to_Upmonitor.m_data[7] = (g_Batt_VoltMesg.Single_Min_Volt>>8);
  Return_Value = BMS_CAN3SendMsg_Monitor(&BMS_to_Upmonitor); 
      
  //0x1898C0F4�����ݲ��Ա��ģ�,CANpro�����룺0xC4C607A0,�����룺0x00000000
  BMS_to_Upmonitor.m_ID = 0x1898C0F4;
	BMS_to_Upmonitor.m_data[0] = (uint8)((SOC_DATA.Filter_Current+400)*10);       //�ܵ������ֱ��ʣ�0.1A��ƫ������400��
	BMS_to_Upmonitor.m_data[1] = ((uint16)((SOC_DATA.Filter_Current+400)*10))>>8;
	BMS_to_Upmonitor.m_data[2] = SOC_LEP_DATA.SOC_t;                             //����SOC��ʼ�����λ,1:SOC��ʼ��;2:SOC���
	BMS_to_Upmonitor.m_data[3] = SOC_LEP_DATA.EE_Value;                          //EEprom������λ,1:EEEprom������ȡ;2:EEE���ڴ���ֵ      //����
	BMS_to_Upmonitor.m_data[4] = (uint8)(g_SOCInfo.SOC_ValueRead*100);                 //���SOCֵ���ֱ��ʣ�1%��
	BMS_to_Upmonitor.m_data[5] = (uint8)(g_SOCInfo.SOC_ValueVoltGet*100);                 //����ѹ�����SOCֵ���ֱ��ʣ�1%��
	BMS_to_Upmonitor.m_data[6] = (uint8)(g_SOCInfo.SOC_Init);                          //�������1:����;0:�޴�
	BMS_to_Upmonitor.m_data[7] = ((uint16)(g_SOCInfo.SOC_Init))>>8;                                           //����
  Return_Value = BMS_CAN3SendMsg_Monitor(&BMS_to_Upmonitor);  
  */
  //Task_Flag_Counter.BMS_to_UpMonitor++;  
}


/*=======================================================================
 *������:      Task_BMUToUpMonitor(void) 
 *����:        BMS collect information to UpMonitor
 *����:        ��       
 *���أ�       ��
 *˵����       BMS���Ͳɼ��ĵ�ѹ��Ϣ��Ϣ����λ����
========================================================================*/ 
void Task_BMUToUpMonitor(void)
{
  uint8  i,j;
  CANFRAME  BMS_to_Upmonitor;
  uint8 batt,batt1; 
  
  batt=(NUM1_Batper_true+NUM2_Batper_true+NUM3_Batper_true+NUM4_Batper_true+NUM5_Batper_true)/3;
  batt1=(NUM1_Batper_true+NUM2_Batper_true+NUM3_Batper_true+NUM4_Batper_true+NUM5_Batper_true)%3;
     
  BMS_to_Upmonitor.m_ID = BMS_Send_Information1;       
	BMS_to_Upmonitor.m_IDE = 1;
	BMS_to_Upmonitor.m_RTR = 0;
	BMS_to_Upmonitor.m_dataLen = 8;
	BMS_to_Upmonitor.m_priority = 6;
  for(i = 0; i <batt ; i++) 
  {
    BMS_to_Upmonitor.m_data[0] = (uint8)(i/NUM_IC);            //��Ҫ����-6804�ı�� 0-3
    BMS_to_Upmonitor.m_data[1] = (uint8)(i%NUM_IC);            //ÿ��6804�ɼ���ѹ�ı��  0-3  
    BMS_to_Upmonitor.m_data[2] = (uint8)g_LTC6811_VoltInfo.CellVolt[i*3];
    BMS_to_Upmonitor.m_data[3] = (g_LTC6811_VoltInfo.CellVolt[i*3]>>8)&0X00FF;
    BMS_to_Upmonitor.m_data[4] = (uint8)g_LTC6811_VoltInfo.CellVolt[i*3+1];
    BMS_to_Upmonitor.m_data[5] = (g_LTC6811_VoltInfo.CellVolt[i*3+1]>>8)&0X00FF;
    BMS_to_Upmonitor.m_data[6] = (uint8)g_LTC6811_VoltInfo.CellVolt[i*3+2];
    BMS_to_Upmonitor.m_data[7] = (g_LTC6811_VoltInfo.CellVolt[i*3+2]>>8)&0X00FF;
    while(CAN_ToUpMonitor(&BMS_to_Upmonitor));
  }  
  switch(batt1) 
  {
    case 1:
    BMS_to_Upmonitor.m_data[0] = (uint8)(i/NUM_IC);
    BMS_to_Upmonitor.m_data[1] = (uint8)(i%NUM_IC);                             //ÿ��6804�ɼ���ѹ�ı�� 
    BMS_to_Upmonitor.m_data[2] = (uint8)g_LTC6811_VoltInfo.CellVolt[i*3];
    BMS_to_Upmonitor.m_data[3] = (g_LTC6811_VoltInfo.CellVolt[i*3]>>8)&0X00FF;
    BMS_to_Upmonitor.m_data[4] = 0xFF;
    BMS_to_Upmonitor.m_data[5] = 0xFF;
    BMS_to_Upmonitor.m_data[6] = 0xFF;
    BMS_to_Upmonitor.m_data[7] = 0xFF;
    while(CAN_ToUpMonitor(&BMS_to_Upmonitor));
    break;
    
    case 2:
    BMS_to_Upmonitor.m_data[0] = (uint8)(i/NUM_IC);
    BMS_to_Upmonitor.m_data[1] = (uint8)(i%NUM_IC);                             //ÿ��6804�ɼ���ѹ�ı�� 
    BMS_to_Upmonitor.m_data[2] = (uint8)g_LTC6811_VoltInfo.CellVolt[i*3];
    BMS_to_Upmonitor.m_data[3] = (g_LTC6811_VoltInfo.CellVolt[i*3]>>8)&0X00FF;
    BMS_to_Upmonitor.m_data[4] = (uint8)g_LTC6811_VoltInfo.CellVolt[i*3+1];
    BMS_to_Upmonitor.m_data[5] = (g_LTC6811_VoltInfo.CellVolt[i*3+1]>>8)&0X00FF;
    BMS_to_Upmonitor.m_data[6] = 0xFF;
    BMS_to_Upmonitor.m_data[7] = 0xFF;
     while(CAN_ToUpMonitor(&BMS_to_Upmonitor));
    break;
    
  default:
    break; 
  }   
                    
  BMS_to_Upmonitor.m_ID = BMS_Send_Information2;       
	BMS_to_Upmonitor.m_IDE = 1;
	BMS_to_Upmonitor.m_RTR = 0;
	BMS_to_Upmonitor.m_dataLen = 8;
	BMS_to_Upmonitor.m_data[0] = (uint8)g_LTC6811_VoltInfo.CellVolt_Max;
  BMS_to_Upmonitor.m_data[1] = (g_LTC6811_VoltInfo.CellVolt_Max>>8)&0x00FF;
  BMS_to_Upmonitor.m_data[2] = g_LTC6811_VoltInfo.CellVolt_MaxNode;
  BMS_to_Upmonitor.m_data[3] = (uint8)g_LTC6811_VoltInfo.CellVolt_Min;
  BMS_to_Upmonitor.m_data[4] = (g_LTC6811_VoltInfo.CellVolt_Min>>8)&0x00FF;
  BMS_to_Upmonitor.m_data[5] = g_LTC6811_VoltInfo.CellVolt_MinNode;
  BMS_to_Upmonitor.m_data[6] = 0xFF;  
  BMS_to_Upmonitor.m_data[7] = 0xFF;       
  while(CAN_ToUpMonitor(&BMS_to_Upmonitor)); 
 
  BMS_to_Upmonitor.m_ID = BMS_Send_Information3;      
	BMS_to_Upmonitor.m_IDE = 1;
	BMS_to_Upmonitor.m_RTR = 0;
	BMS_to_Upmonitor.m_dataLen = 8;
	BMS_to_Upmonitor.m_priority = 6;
	for( i=0; i< ((NUM_Tem+6) / 7) ;i++)         //ly ����Э���޸�
  {
    BMS_to_Upmonitor.m_data[0] = i;
    //memset( &pMsgTran.data,0xFF,8 );       /* 8��data����Ϊ40 */
    if( i < 1 )                      //������չ���޸Ĵ˴�
    {
      for(j=1; j < NUM_IC*2; j++) 
      {
        BMS_to_Upmonitor.m_data[j] = g_LTC6811_TempInfo.CellTemp[j-1+i*7] + 40;
      }   
    } 
    else 
    {
      for( j = 1 ; j <= (NUM_Tem% 7);j++ ) 
      {
        BMS_to_Upmonitor.m_data[j] = g_LTC6811_TempInfo.CellTemp[j-1+i*7] + 40; 
      }
    }
    while(CAN_ToUpMonitor(&BMS_to_Upmonitor)); 
  }
  
  BMS_to_Upmonitor.m_ID = BMS_Send_Information4;       // ��ػ�����Ϣ1;
	BMS_to_Upmonitor.m_IDE = 1;
	BMS_to_Upmonitor.m_RTR = 0;
	BMS_to_Upmonitor.m_dataLen = 8;
	BMS_to_Upmonitor.m_priority = 6;
  BMS_to_Upmonitor.m_data[0] = g_LTC6811_TempInfo.CellTemp_Max + 40;
	BMS_to_Upmonitor.m_data[1] = g_LTC6811_TempInfo.CellTemp_MaxNode;   
	BMS_to_Upmonitor.m_data[2] = g_LTC6811_TempInfo.CellTemp_Min + 40;
	BMS_to_Upmonitor.m_data[3] = g_LTC6811_TempInfo.CellTemp_MinNode;   
	BMS_to_Upmonitor.m_data[4] = g_LTC6811_TempInfo.CellTemp_Tatoltemp;
	BMS_to_Upmonitor.m_data[5] = g_LTC6811_TempInfo.CellTemp_Tatoltemp>>8;   
	BMS_to_Upmonitor.m_data[6] = 0xFF;
	BMS_to_Upmonitor.m_data[7] = 0xFF;       	 
  while(CAN_ToUpMonitor(&BMS_to_Upmonitor)); 
 
 /* BMS_to_Upmonitor.m_ID = BMS_Send_Single_Volt;       
	BMS_to_Upmonitor.m_IDE = 1;
	BMS_to_Upmonitor.m_RTR = 0;
	BMS_to_Upmonitor.m_dataLen = 8;
	BMS_to_Upmonitor.m_priority = 6;
  for(j = 0; j < Monitor_Stand.LTC_Chip_Num; j++) 
  {
  	  for(i = 0;i<4;i++) 
  	  {  
        	BMS_to_Upmonitor.m_data[0] = j+1;
        	BMS_to_Upmonitor.m_data[1] = i;
        	BMS_to_Upmonitor.m_data[2] = Volt_Data_T.Signal_Vol[j][i*3];   
        	BMS_to_Upmonitor.m_data[3] = Volt_Data_T.Signal_Vol[j][i*3]>>8;
        	BMS_to_Upmonitor.m_data[4] = Volt_Data_T.Signal_Vol[j][i*3+1];
        	BMS_to_Upmonitor.m_data[5] = Volt_Data_T.Signal_Vol[j][i*3+1]>>8;
        	BMS_to_Upmonitor.m_data[6] = Volt_Data_T.Signal_Vol[j][i*3+2];
        	BMS_to_Upmonitor.m_data[7] = Volt_Data_T.Signal_Vol[j][i*3+2]>>8;       
          Return_Value= MSCAN1SendMsg(&BMS_to_Upmonitor); 
  	  } 
  } */

 /* BMS_to_Upmonitor.m_ID = BMS_Send_Pack_Temp;             //оƬ�¶ȡ����߿�·����ѹ
	BMS_to_Upmonitor.m_IDE = 1;
	BMS_to_Upmonitor.m_RTR = 0;
	BMS_to_Upmonitor.m_dataLen = 8;
	BMS_to_Upmonitor.m_priority = 6;
  for(i =0;i<Monitor_Stand.LTC_Chip_Num;i++)
  {
      BMS_to_Upmonitor.m_data[0] = i+1;
    	BMS_to_Upmonitor.m_data[1] = 0;	
    	BMS_to_Upmonitor.m_data[2] = Temp_Data_T.Pack_Temper[i][0]+40;   
    	BMS_to_Upmonitor.m_data[3] = Temp_Data_T.Pack_Temper[i][1]+40;
    	BMS_to_Upmonitor.m_data[4] = Temp_Data_T.Pack_Temper[i][2]+40;
    	BMS_to_Upmonitor.m_data[5] = Temp_Data_T.Pack_Temper[i][3]+40;
    	BMS_to_Upmonitor.m_data[6] = 40;
    	BMS_to_Upmonitor.m_data[7] = 40;       
      Return_Value= MSCAN2SendMsg(&BMS_to_Upmonitor); 
  } */

  BMS_to_Upmonitor.m_ID = BMS_Send_Information5;             //���߿�·
	BMS_to_Upmonitor.m_IDE = 1;
	BMS_to_Upmonitor.m_RTR = 0;
	BMS_to_Upmonitor.m_dataLen = 8;
	BMS_to_Upmonitor.m_priority = 6;
  BMS_to_Upmonitor.m_data[0] = g_LTC6811_TempInfo.ICTemp_OverState;
	BMS_to_Upmonitor.m_data[1] = g_LTC6811_OpwireInfo.OpenwireErr;	
	BMS_to_Upmonitor.m_data[2] = g_LTC6811_VoltInfo.CellVolt_Total;   
	BMS_to_Upmonitor.m_data[3] = g_LTC6811_VoltInfo.CellVolt_Total>>8;
	BMS_to_Upmonitor.m_data[4] = g_LTC6811_VoltInfo.CellVolt_Total>>16;
	BMS_to_Upmonitor.m_data[5] = (uint8)g_IsoDetect.insulation_TotalVolt;
	BMS_to_Upmonitor.m_data[6] = ((uint16)g_IsoDetect.insulation_TotalVolt)>>8;
	BMS_to_Upmonitor.m_data[7] = ((uint32)g_IsoDetect.insulation_TotalVolt)>>16;       
  while(CAN_ToUpMonitor(&BMS_to_Upmonitor));
   
  BMS_to_Upmonitor.m_ID = BMS_Send_Information6;             //���߿�·
	BMS_to_Upmonitor.m_IDE = 1;
	BMS_to_Upmonitor.m_RTR = 0;
	BMS_to_Upmonitor.m_dataLen = 8;
	BMS_to_Upmonitor.m_priority = 6;
	for(i = 0; i < NUM_IC ; i++)
	{
    BMS_to_Upmonitor.m_data[i*2] = g_LTC6811_OpwireInfo.OpenwireLocation[i];                   // ���߿�·
  	BMS_to_Upmonitor.m_data[i*2 + 1] = (uint8)((g_LTC6811_OpwireInfo.OpenwireLocation[i]>>8)&0x00FF);	       
	}
	while(CAN_ToUpMonitor(&BMS_to_Upmonitor));
      
}
