/*=======================================================================
 *Subsystem:   ���
 *File:        Task_BmsAndUpMonitor.h
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#ifndef _TASK_BMS_AND_UP_MONITOR_H_
#define _TASK_BMS_AND_UP_MONITOR_H_  

  #include  "TypeDefinition.h"  
  #include  "CAN.h"  
  
 //��λ�����ݱ궨
 //��ط�ֵ�궨
  //#define    Monitor_BMS_Volt   0x1810C0F4     //��ص�ѹ��ֵ��Ϣ        
  typedef struct 
  { //00
    uint16  Volt_Cell_High1;  //�����ѹ��ѹһ����ֵ�� ��λ 0.0001V/Bit
    uint16  Volt_Cell_High2;  //�����ѹ��ѹ������ֵ�� ��λ 0.0001V/Bit
    //uint16  Volt_Cell_High3;  //�����ѹ��ѹ������ֵ�� ��λ 0.0001V/Bit
    //01
    uint16  Volt_Cell_Low1; //�����ѹǷѹһ����ֵ�� ��λ 0.0001V/Bit
    uint16  Volt_Cell_Low2; //�����ѹǷѹ������ֵ�� ��λ 0.0001V/Bit
    //uint16  Volt_Cell_Low3; //�����ѹǷѹ������ֵ�� ��λ 0.0001V/Bit
    //02
    uint16  Volt_Sys_High1;  //������ѹ��ֵ1��
    uint16  Volt_Sys_High2;  //������ѹ��ֵ2��
    //uint16  Volt_Sys_High3;  //������ѹ��ֵ3��
    //03
    uint16  Volt_Sys_Low1; //�����Ƿѹ��ֵ1��
    uint16  Volt_Sys_Low2; //�����Ƿѹ��ֵ2��
    //uint16  Volt_Sys_Low3; //�����Ƿѹ��ֵ3��
    //04
    uint16  Volt_Cell_Diff1;  //����ѹ��һ����ֵ�� ��λ 0.0001V/Bit
    //uint16  Volt_Cell_Diff1;  //����ѹ�������ֵ�� ��λ 0.0001V/Bit
    //uint16  Volt_Cell_Diff1;  //����ѹ��������ֵ�� ��λ 0.0001V/Bit
    //05
    //uint16  System_Diff1_Volt;  //ϵͳѹ��һ����ֵ�� ��λ 0.1V/Bit
    //uint16  System_Diff2_Volt;  //ϵͳѹ�������ֵ�� ��λ 0.1V/Bit
    //uint16  System_Diff3_Volt;  //ϵͳѹ��������ֵ�� ��λ 0.1V/Bit
  }BMSMonitor_Volt_T;
  extern BMSMonitor_Volt_T BMSMonitor_Volt;
  
  //#define    Monitor_BMS_Temp   0x1811C0F4     //�¶���ֵ��Ϣ 
  typedef struct 
  { //00
    uint8  Temp_Charge_High1;   //�������¶ȹ���һ����ֵ�� ��λ 1��/Bit  ƫ������-40
    uint8  Temp_Charge_High2;   //�������¶ȹ��¶�����ֵ�� ��λ 1��/Bit  ƫ������-40
    //uint8  Temp_Charge_High3;   //�������¶ȹ���������ֵ�� ��λ 1��/Bit  ƫ������-40
    
    uint8  Temp_Charge_Low1;  //�������¶ȵ���һ����ֵ�� ��λ 1��/Bit  ƫ������-40
    uint8  Temp_Charge_Low2;  //�������¶ȵ��¶�����ֵ�� ��λ 1��/Bit  ƫ������-40
    //uint8  Temp_Charge_Low3;  //�������¶ȵ���������ֵ�� ��λ 1��/Bit  ƫ������-40
    //01
    uint8  Temp_DisCharge_High1;    //����ŵ��¶ȹ���һ����ֵ�� ��λ 1��/Bit   ƫ������-40
    uint8  Temp_DisCharge_High2;    //����ŵ��¶ȹ��¶�����ֵ�� ��λ 1��/Bit   ƫ������-40
    //uint8  Temp_DisCharge_High3;    //����ŵ��¶ȹ���������ֵ�� ��λ 1��/Bit   ƫ������-40
    
    uint8  Temp_DisCharge_Low1;   //����ŵ��¶ȵ���һ����ֵ�� ��λ 1��/Bit   ƫ������-40
    uint8  Temp_DisCharge_Low2;   //����ŵ��¶ȵ��¶�����ֵ�� ��λ 1��/Bit   ƫ������-40
    //uint8  Temp_DisCharge_Low3;   //����ŵ��¶ȵ���������ֵ�� ��λ 1��/Bit   ƫ������-40
    //02
    uint8  Temp_Charge_Diff1;     //�������²�һ����ֵ�� ��λ 1��/Bit    ƫ������-40
    //uint8  Charge_Diff2_Temp;     //�������²������ֵ�� ��λ 1��/Bit    ƫ������-40
    //uint8  Charge_Diff3_Temp;     //�������²�������ֵ�� ��λ 1��/Bit    ƫ������-40
    
    uint8  Temp_DisCharge_Diff1;  //����ŵ��²�һ����ֵ�� ��λ 1��/Bit    ƫ������-40
    //uint8  DisCharge_Diff2_Temp;  //����ŵ��²������ֵ�� ��λ 1��/Bit    ƫ������-40
    //uint8  DisCharge_Diff3_Temp;  //����ŵ��²�������ֵ�� ��λ 1��/Bit    ƫ������-40
   
   /* //03
    uint8  ChargeGun_Over1_Temp; //���ǹ����һ����ֵ�� ��λ 1��/Bit    ƫ������-40
    uint8  ChargeGun_Over2_Temp; //���ǹ���¶�����ֵ�� ��λ 1��/Bit    ƫ������-40
    uint8  ChargeGun_Over3_Temp; //���ǹ����������ֵ�� ��λ 1��/Bit    ƫ������-40
    
    uint8  Chip_Over1_Temp; //BMSоƬ����һ����ֵ�� ��λ 1��/Bit    ƫ������-40
    uint8  Chip_Over2_Temp; //BMSоƬ���¶�����ֵ�� ��λ 1��/Bit    ƫ������-40
    uint8  Chip_Over3_Temp; //BMSоƬ����������ֵ�� ��λ 1��/Bit    ƫ������-40
    */
  }BMSMonitor_Temp_T;
  extern BMSMonitor_Temp_T BMSMonitor_Temp;
  
 
  //#define    Monitor_BMS_Current   0x1812C0F4     //������ֵ��SOC��ֵ��Ϣ 
  typedef struct 
  { //00
    uint16  Current_DisCharge_High1;  //�ŵ������ֵ1����0.1A/λ     ƫ����:-750
    uint16  Current_DisCharge_High2;  //�ŵ������ֵ2����0.1A/λ     ƫ����:-750
    //uint16  Current_DisCharge_High3;  //�ŵ������ֵ3����0.1A/λ     ƫ����:-750
    //01
    uint16  Current_Charge_High1;     //��������ֵ1����0.1A/λ      ƫ����:-750
    uint16  Current_Charge_High2;     //��������ֵ2����0.1A/λ      ƫ����:-750
    //uint16  Current_Charge_High3;     //��������ֵ3����0.1A/λ      ƫ����:-750
    /*//02
    uint8   SOC_Low1;   //��SOC1��     1%/λ
    uint8   SOC_Low2;   //��SOC2��
    uint8   SOC_Low3;   //��SOC3��
    
    uint8   SOC_High1;  //��SOC1��
    uint8   SOC_High2;  //��SOC2��
    uint8   SOC_High3;  //��SOC3��
    */
  }BMSMonitor_Curr_T;
  extern BMSMonitor_Curr_T BMSMonitor_Curr;
  
  //#define    Monitor_BMS_Insulation   0x1813C0F4     //��Ե������ֵ��Ϣ 
  typedef struct 
  { //00
    uint16 Insulation_Resis1;     // ��Ե����һ����ֵ�� ��λ 0.1K��/Bit;
    uint16 Insulation_Resis2;     // ��Ե���������ֵ�� ��λ 0.1K��/Bit;
    uint16 Insulation_Resis3;     // ��Ե����������ֵ�� ��λ 0.1K��/Bit;  
  
  }BMSMonitor_Insu_T;
  extern BMSMonitor_Insu_T BMSMonitor_Insu; 
  
  //��������λ����SOC��ʼ��/�����
  typedef struct 
  { 
    uint8   SOC_t; //���ڷ�������λ����ͷ�ļ���  
  
  }BMSMonitor_SOC_T;
  extern BMSMonitor_SOC_T BMSMonitor_SOC; 
  
  //��������λ����SOHֵ
  typedef struct 
  { 
    uint8   SOH; //���ڷ�������λ����ͷ�ļ���  
  
  }BMSMonitor_SOH_T;
  extern BMSMonitor_SOH_T BMSMonitor_SOH; 
  
  typedef struct
  {
     uint16 LT;   //!!!!!
  
  }BMSMonitor_New_LT_T;
  extern BMSMonitor_New_LT_T BMSMonitor_New_LT;

//---------------------------------------------------------------------------------------------------- 
  
  //#define    Monitor_BMS_SOC   0x1915F4C0         //��������Ϣ�궨˵��
  typedef struct 
  { //00
    uint16  SOC_Init;             //SOC��ʼֵ�� ��λ 1%/Bit��
    union
    {
      
     /* uint8  Relay1_Station;       //�̵������ر���
      struct 
      {
        uint8 Relay_Positive:2;    //�����̵���
        uint8 Relay_Negtive:2;     //�����̵���
        uint8 Relay_Precharge:2;   //Ԥ��̵���
        uint8 Relay_Charge:2;      //���̵���
        
      }Monitor_BMS_DelayBit; */
      
     int  Relay1_Station;       //�̵������ر���
      struct 
      {
        int Relay_Positive:2;    //�����̵���
        int Relay_Negtive:2;     //�����̵���
        int Relay_Precharge:2;   //Ԥ��̵���
        int Relay_Charge:2;      //���̵���
        
      }Monitor_BMS_DelayBit;
    }Monitor_BMS_Delay_T;
    
    uint16 Ref_Volt;               //�ο���ѹ�궨(0~65536)
    uint16 Ref_Current;            //������������б��
    //01
    uint16 Volt_Resis;             //��ѹ����з�ѹ����ı궨
    uint32 Pack_Total_Capacity;    //�궨SOH�е�������(2000*�����)
    //02
    uint16 BMS_Running_Time;       //BMS����ʱ��
      
  }MonitorBMS_SOC_T;
  extern MonitorBMS_SOC_T MonitorBMS_SOC;
  
   
  //#define    Monitor_to_BMSFF   0x19FFF4C0
  typedef struct 
  {
    uint8 Communication;   //BMS����λ��ͨ���źţ�0xAA�� ͨ�ſ�ʼ�� 0x00�� ������ͨ�ţ�
    uint8 Bootloader_Time;      //���ݸ��� 1~5�� ��1 ���� 500ms��  
    uint8 CellMessageSend_judge;   //�������Ϣ�����жϱ��      
    uint16 None2;
    uint16 None3;
  }MonitorBMS_Start_T;
  extern MonitorBMS_Start_T MonitorBMS_Start; 
  /*
  //ʵʱ����
  //1890C0F4 BMS�����֮�����ͨ��
  //BMS
  typedef struct
  {
    //�����ֹԭ��
    uint8 Upto_GivenSOC;           //�ﵽ����SOC
    uint8 Upto_GivenSysVolt;       //�ﵽ������ѹ
    uint8 Upto_GivenCellVolt;      //�ﵽ���������ѹ
    uint8 Charger_Suspend;         //����������ֹ
    //����ԭ��
    uint8 Check_Self;         //�Լ�ͨ��
    uint8 Cur_Mismatch;       //����ƥ��
    uint8 Volt_Mismatch;      //��ѹƥ��
    uint8 Jueyuan_Fault;      //��Ե����
    uint8 Connect_Overtemp;   //������������¹���
    uint8 BMS_Overtemp;       //BMS������¹���
    uint8 Connect_CC2_Fault;      //�����������CC2�ڼ�����
    uint8 Sys_Overtime;       //������ѹ����
    uint8 Relay_Off;          //��ѹ�̵�������
    //uint8 CC2_Fault;          //CC2�ڹ���
    uint8 Other_Fault;        //��������
    uint8 Chag_OverCur;       //����������
    uint8 Volt_Error;         //��ѹ�Ƿ��쳣 
  }BMS_ChagInf_T;
  
  typedef struct
  { //��ʱ
    uint8 CRM00_Overtime;
    uint8 CRMAA_Overtime;
    uint8 CTSCML_Overtime;
    uint8 CRO_Overtime;
    uint8 CCS_Overtime;
    uint8 CST_Overtime;
    uint8 CSD_Overtime;
  }BMS_Connect_Overtime_T;
  
  //����
  typedef struct
  {
    //�����ֹԭ��
    uint8 Normal_Suspend;           //������ֹ
    uint8 Artificial_Suspend;       //�˹���ֹ
    uint8 Fault_Suspend;            //������ֹ
    uint8 BMS_Suspend;              //BMS������ֹ
    //����ԭ��
    uint8 Charger_Overtemp;         //�������¹���
    uint8 Chag_Connect;             //�������������
    uint8 Charger_Inner_Overtemp;   //�����ڲ����¹���
    uint8 Energy_Transfer;          //�����������
    uint8 Charger_Emergency_Stop;   //�����Ƿ�ͣ
    uint8 Other_Fault;              //��������
    uint8 Cur_Error;                //����ƥ��
    uint8 Volt_Error;               //��ѹ�Ƿ��쳣
  }Charger_ChagInf_T;
  
  typedef struct
  { //��ʱ
    uint8 BRM_Overtime;
    uint8 BCP_Overtime;
    uint8 BRO_Overtime;
    uint8 BCS_Overtime;
    uint8 BCL_Overtime;
    uint8 BST_Overtime;
    uint8 BSD_Overtime;   
  }Charger_Connect_Overtime_T;
  
  typedef struct
  {
     BMS_ChagInf_T BMS_ChagInf;
     BMS_Connect_Overtime_T BMS_Connect_Overtime;
     Charger_ChagInf_T Charger_ChagInf;
     Charger_Connect_Overtime_T Charger_Connect_Overtime;
  }Charge_Error_T;
  extern Charge_Error_T Charge_Error;
 */
  void Bms_to_Up_Monitor(void);
  void UpMonitor_to_Bms(pCANFRAME);

#endif