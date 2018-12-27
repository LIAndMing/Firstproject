/*=======================================================================
 *Subsystem:   ���
 *File:        RS485.C
 *Author:      WenMing
 *Description: ͨ�ţ�SCI2.
               �ӿڣ�PJ0��RXD����PJ1��TXD��
               �����ʣ�
               ���ԣ��۲�RS485_Receive��������ʾ�������Ƿ�һ��,�Լ�IICģ��ʱ�����ʾ��
 ========================================================================
 * History:           �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸��߼��޸����ݼ���
 * 1. Date:           
      Author:         
      Modification:   
========================================================================*/
#include "RS485.h"  
#include "EEDFLASH.h" 
#include "BattInfoParaStructure.h"  
#include "Task_SOC.h"
#include "Task_FaultLevelJudge.h"
     
RS485  RS485_Receive;         
      
/*=======================================================================
 *������:      RS485_DataMemset(void)
 *����:        ��ʼ��485����
 *����:        ��       
 *���أ�       ��
 *˵����       
========================================================================*/
void RS485_DataMemset(void) 
{
    RS485_EnableDir = 1;
    RS485_Enable = 1; 
}

/*=======================================================================
 *������:      RS485_DataReceive
 *����:        SCI���ݽ���
 *����:        ��       
 *���أ�       ��
 *˵����       
========================================================================*/
void RS485_DataReceice(void) 
{ 

 /*----------------------------------------˫�ֽڽ���-----------------------------------*/

    RS485_Receive.TxData_couple.TX2.BMS_SOH = 0; //��  
    RS485_Receive.TxData_couple.TX2.RunningTime = (uint16)(EEprom_Data.BmsRunTime_min/60.0);           
    RS485_Receive.TxData_couple.TX2.BMS_Current = (uint16)(DataColletInfo.DataCollet_Current_Filter*10);                  
    RS485_Receive.TxData_couple.TX2.BMS_SOC = (uint16)(SOCInfo.SOC_ValueRead*100);                                                    
    RS485_Receive.TxData_couple.TX2.Pack_Hightemp = TempInfo.CellTemp_Max-20;               
    RS485_Receive.TxData_couple.TX2.Pack_Lowtemp = TempInfo.CellTemp_Min-20;                
    RS485_Receive.TxData_couple.TX2.Pack_Volt = DataColletInfo.SysVolt_Total/10;                 
    RS485_Receive.TxData_couple.TX2.Single_Maxvolt = (VoltInfo.CellVolt_Max +5)/10;      
    RS485_Receive.TxData_couple.TX2.Single_Lowvolt = (VoltInfo.CellVolt_Min +5)/10;        
    RS485_Receive.TxData_couple.TX2.iso_resistance = VoltInfo.CellVolt_MaxNode+1;    //��ߵ���ڵ��   
    RS485_Receive.TxData_couple.TX2.Lease_Time = VoltInfo.CellVolt_MinNode+1;        //��͵���ڵ��
                          
/*------------------------------------------���ֽڽ���---------------------------------*/       
   
      
    RS485_Receive.TxData_single.TX1.Alam_Hightemp = (uint8)Fault_DisCharge.Level_Temp_High;
    RS485_Receive.TxData_single.TX1.Alam_Lowtemp = (uint8)Fault_DisCharge.Level_Temp_Low;     
    
    //RS485_Receive.TxData_single.TX1.Alam_SOC = (uint8)DiscFlt.LowSOC;  
    RS485_Receive.TxData_single.TX1.Alam_Pack_highvolt =  (uint8)Fault_Charge.Level_Volt_Sys_High;                        
    RS485_Receive.TxData_single.TX1.Alam_Pack_Lowvolt = (uint8)Fault_DisCharge.Level_Volt_Sys_Low;
    RS485_Receive.TxData_single.TX1.Alam_single_highvolt =  (uint8)Fault_Charge.Level_Volt_Cell_High;                      
    RS485_Receive.TxData_single.TX1.Alam_single_lowvolt = (uint8)Fault_DisCharge.Level_Volt_Cell_Low;
    RS485_Receive.TxData_single.TX1.Alam_Charge_highcurr = (uint8)Fault_Charge.Level_Current_Charge_High;                       
    RS485_Receive.TxData_single.TX1.Alam_Discharge_highcurr = (uint8)Fault_DisCharge.Level_Current_DisCharge_High;
    RS485_Receive.TxData_single.TX1.Alam_communi_fail = 0; //���߿�·
    RS485_Receive.TxData_single.TX1.Alam_tenancy = 0;  //δ��  
    RS485_Receive.TxData_single.TX1.Acc_offline = 0;   //δ��
    
/*--------------------------------------------------------------------------------------*/
    //Task_Flag_Counter.Counter_485Receive++; 
 }    

/*=======================================================================
 *������:      Rs485_Transfer
 *����:        SACI���ʹ���
 *����:        rs:Ҫ���ͽṹ����ֽ�ָ��       
 *���أ�       ��
 *˵����       
========================================================================*/
void Rs485_Transfer() 
{          
   uint8 i;
   PTT_PTT1 = ~PTT_PTT1;
   RS485_DataReceice();
   delay(10);
   SCI2_send_Nbyte(Array_couple, RS485_Receive.TxData_couple.SCI_Content2);  
   SCI2_send_Nbyte(Array_single, RS485_Receive.TxData_single.SCI_Content1); 

   //Task_Flag_Counter.Counter_485Send++;   
   return;
}