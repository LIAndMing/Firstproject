/*=======================================================================
 *Subsystem:   ���
 *File:        Task_SOC.c
 *Author:      WenYuhao
 *Description: 
 ========================================================================
 * History:    
 * 1. Date:
      Author:
      Modification:  
========================================================================*/

#include  "TypeDefinition.h"
#include  "Task_SOC.h"
#include  "BattInfoConfig.h"
#include  "BattInfoParaStructure.h"
#include  "Task_DataProcess.h"
#include  "stdtypes.h"
#include  "ADC.h"
#include  "ADC_Config.h"
#include  "Filter_Function.h"
#include  "Roll_Task.h"
#include  "Task_BmsAndUpMonitor.h"
#include  "WorkModeJudge.h"

SOCInfo_T     SOCInfo;
EnergyInfo_T  EnergyInfo;


static float  inition_soc(float v);
static uint8  SOC_StaticTime(uint8 readhour, uint8 readminite, float current);
static void   SOC_AhIntegral(float current, uint16 Voltagemin, uint16 Voltagemax, uint16 SampleTime);
static float  Energy_TotalCal(uint8 mode,float ah1,uint32 Total_V,float Total_E,float current);
/*=======================================================================
 *������:      Task_SOC(void)
 *����:        SOC�ļ��㺯��
 *����:        ��       
 *���أ�       ��
 *˵����       ��������Ҫ��SOCֵ
========================================================================*/
void Task_SOC(void)                                
{       
  uint8 Value=0;    
  
  //�ɼ�����
  DataColletInfo.DataCollet_Current_Filter = FilterFunction_Median(ADC_Current, DataColletInfo.DataCollet_Current_Filter);// ȡ����ʵ����ֵ�������˲�
  
  //SOC�Ĺ��� 
  if(VoltInfo.CellVolt_Min != 0 || VoltInfo.CellVolt_Max != 0xFFFF)     // ������Ч���ж�
  {                                    
    //SOCInfo.SOC_CheckTable_Flag = SOC_StaticTime(24, 60, DataColletInfo.DataCollet_Current_Filter);//�����µ���С��0.5Aʱ���
    SOC_AhIntegral(DataColletInfo.DataCollet_Current_Filter, VoltInfo.CellVolt_Min, VoltInfo.CellVolt_Max, SOC_Period); //����SOCֵ 
  }
  else
  {
    SOCInfo.SOC_Init = 0;
  }
}

/*=======================================================================
 *������:      SOC_AhIntegral(float current, uint16 Voltagemin, uint16 Voltagemax, uint16 SampleTime)
 *����:        SOC��ʱ���ַ�����
 *����:        Volatagemin��������С��ѹ
               Voltagemax����������ѹ
               SampleTime������ִ��ʱ��
 *���أ�       ��
 *˵����       SOCInfo.SOC_ValueRead��SOC�ϴ�����ʾ���ϵĶ�ȡֵ��SOC�ϴδ洢ֵ
               soc_Volt���������С��ѹȡȨֵ�õ���SOCֵ
               ˼·��Ϊ�˱����ϴδ洢ֵ�Ͳ��õ���У��ֵ�������ƫ�
                     ���ü�Ȩ�صİ취���洢ֵ�Ͳ��ֵ����ƫ�ƣ���ʼʱƫ
                     ��洢ֵ��������ƫ����ֵ���洢�ڳ��ʱ����SOC>90%
                     ��SOC<20%�ǽ���У����
========================================================================*/  
static
uint8 QQ[5];
static
void SOC_AhIntegral(float current, uint16 Voltagemin, uint16 Voltagemax, uint16 SampleTime)  //100ms
{        
  float T=0;
  float AH=0,ah1=0;
  float Vmin = 0, Vmax = 0;
  static float Cellcap;
  Vmin = Voltagemin/10000.0;
  Vmax = Voltagemax/10000.0;
  T = SampleTime/1000.0;// 100msʱ��ת��Ϊ��
       
  if((SOCInfo.SOC_Init <= 0)||(SOCInfo.SOC_Init >= 100)) //EFPROM���ֵΪ0ʱ�����ֵ(�״������õ�)
  { 
      //SOC_LEP_DATA.SOC_t = 1;                         //CAN�ⷢ���
      SOCInfo.SOC_LowestVoltGet = inition_soc(Vmin);        //����С��ѹ���õ���SOC 
      SOCInfo.SOC_HighestVoltGet = inition_soc(Vmax);       //������ѹ���õ���SOC 
      SOCInfo.SOC_ValueVoltGet = SOCInfo.SOC_LowestVoltGet/(1+SOCInfo.SOC_LowestVoltGet-SOCInfo.SOC_HighestVoltGet);
      SOCInfo.SOC_ValueRead = SOCInfo.SOC_ValueVoltGet;  //�״γ�ʼ��SOCֵ���״ε�һ���ԽϺ�  
      SOCInfo.SOC_ValueInitDiff = 0; 
      SOCInfo.SOC_ValueRealtimeDiff = SOCInfo.SOC_ValueInitDiff ;          
      SOCInfo.SOC_Init =  (uint16)(SOCInfo.SOC_ValueRead*100);
      *(uint16*)(0x0D10) =  (uint16)(SOCInfo.SOC_Init); //SOCInfo.SOC_Init

  }
  else if(((SOCInfo.SOC_ValueRead < 0.2)||(SOCInfo.SOC_ValueRead > 0.9)) && ((SOCInfo.SOC_StandingTime >= 4)||(SOCInfo.SOC_CheckTable_Flag == 1)))
  {   //��SOCС��20%�����90%,�ҵ�ؾ���ʱ�����3Сʱ,���ڳ���״̬�µ���С��0.5A����ʱ�����3Сʱ���в��     
      //SOC_LEP_DATA.SOC_t = 2;
      SOCInfo.SOC_StandingTime = 0; 
      SOCInfo.SOC_CheckTable_StartTime = ((uint16)24<<8) | (60);      
      SOCInfo.SOC_LowestVoltGet = inition_soc(Vmin);        //����С��ѹ���õ���SOC 
      SOCInfo.SOC_HighestVoltGet = inition_soc(Vmax);       //������ѹ���õ���SOC 
      SOCInfo.SOC_ValueVoltGet = SOCInfo.SOC_LowestVoltGet/(1+SOCInfo.SOC_LowestVoltGet - SOCInfo.SOC_HighestVoltGet);
      SOCInfo.SOC_ValueInitDiff  = SOCInfo.SOC_ValueRead - SOCInfo.SOC_ValueVoltGet;
      SOCInfo.SOC_ValueRealtimeDiff = SOCInfo.SOC_ValueInitDiff ;
  } 
   
  //��ʱ���֣���Ҫ���������С��
  if(current>=0.5 || current<=-0.5)
  {  
    //if(SOCInfo.SOC_CalTime % 300 == 0)//5min�����¶Ȳ�һ������
    //{
    //   SOCInfo.SOC_CalTime = 0;
    //   Cellcap = CellCapacity_Cal(g_Batt_TempMesg.aveTemp);  
    //}
    Cellcap = CELL_CAPACITY;
    
    AH = current*T/3600.0/Cellcap;  //SOCֵ  
    ah1 = current*T/3600.0;
  }
  //���״̬     
  if(g_WorkStateJudge.WorkState == MODE_CHARGE)    //��� 
  {      
    if(VoltInfo.CellVolt_Max >= (CELL_VOLT_MAX-0.01)*10000)                         
    //if(VoltInfo.CellVolt_Max >= (MaxVolt_Cal(g_Batt_TempMesg.aveTemp)-0.02)*10000)          //����ѹ�ﵽ(���ֵ-0.02)ʱ�����г��ĩ�˵�У��    
    {     
      SOCInfo.SOC_HighestVoltGet = 1.0;                             // �ߵ�ѹ����SOC��Ϊ1��
      if(SOCInfo.SOC_ValueRead > SOCInfo.SOC_ValueVoltGet)          //���״̬max_V�ﵽ(���ֵ-0.01)Vʱ,R>V,����SOC_deta��SOC_K��ֵ
      {                                                             //soc_vδ��1ǰsoc_r>soc_v,���ڶ��ν���˺���ʱsoc_v=1,��ʱsoc_v>soc_r
        SOCInfo.SOC_ValueRealtimeDiff = SOCInfo.SOC_ValueRead - 1; 
        SOCInfo.SOC_ValueInitDiff  =  SOCInfo.SOC_ValueRealtimeDiff;
      }
      else if((1.0-SOCInfo.SOC_ValueVoltGet > 0.01) && (SOCInfo.SOC_HighestVoltGet>=1.0)) //���״̬max_V�ﵽ(���ֵ-0.02)Vʱ,R<V,����SOC_deta��SOC_K��ֵ 
      {
        SOCInfo.SOC_ValueRealtimeDiff = SOCInfo.SOC_ValueRead - 1; 
        SOCInfo.SOC_ValueInitDiff  =  SOCInfo.SOC_ValueRealtimeDiff;
      }      
    }             
   
    EnergyInfo.Energy_Total_Charge = Energy_TotalCal(g_WorkStateJudge.WorkState,ah1,DataColletInfo.SysVolt_Total,EnergyInfo.Energy_Total_Charge,current);
    //EnergyInfo.Energy_Once_DisCharge = 0;
  }            
  //�ŵ�״̬
  else     
  {   
    if(VoltInfo.CellVolt_Min <= CELL_VOLT_MIN*10000.0)//3.0V     
    {     
      SOCInfo.SOC_LowestVoltGet = 0;                                                    //�͵�ѹ��SOC��Ϊ0
      if(SOCInfo.SOC_ValueRead < SOCInfo.SOC_ValueVoltGet)                              //��ֹS_V��Ϊ0ʱ��S_R����Ϊ0
      {
        SOCInfo.SOC_ValueRealtimeDiff = SOCInfo.SOC_ValueRead;
        SOCInfo.SOC_ValueInitDiff  = SOCInfo.SOC_ValueRealtimeDiff;
      }
      else if((SOCInfo.SOC_ValueVoltGet > 0.01) && (SOCInfo.SOC_LowestVoltGet <= 0))      //��ֹS_V��Ϊ0ʱ��S_R��������
      {
        SOCInfo.SOC_ValueRealtimeDiff = SOCInfo.SOC_ValueRead;
        SOCInfo.SOC_ValueInitDiff  = SOCInfo.SOC_ValueRealtimeDiff;
      }
    }
    EnergyInfo.Energy_Total_DisCharge = Energy_TotalCal(g_WorkStateJudge.WorkState,ah1,DataColletInfo.SysVolt_Total,EnergyInfo.Energy_Total_DisCharge,current);          
                
    //SOC_LEP_DATA.ThisTime_Energy = SOC_LEP_DATA.ThisTime_Energy + Out_Energy;    
    
  } 
  
  SOCInfo.SOC_LowestVoltGet  = SOCInfo.SOC_LowestVoltGet-AH;        //��������С��ѹ�ۼ�SOC
  SOCInfo.SOC_HighestVoltGet = SOCInfo.SOC_HighestVoltGet-AH;       //����������ѹ�ۼ�SOC
  
  
  if(SOCInfo.SOC_HighestVoltGet >= 1)
     SOCInfo.SOC_HighestVoltGet = 1;
  else if (SOCInfo.SOC_LowestVoltGet <= 0)
     SOCInfo.SOC_LowestVoltGet = 0;  
  
  //���ƫ���ڵ�ѹ�ߵĵ�ص�SOC���ŵ�ƫ���ڵ͵�ѹ��ص�SOC
  SOCInfo.SOC_ValueVoltGet = SOCInfo.SOC_LowestVoltGet / (1 + SOCInfo.SOC_LowestVoltGet - SOCInfo.SOC_HighestVoltGet); 
  SOCInfo.SOC_ValueRead = SOCInfo.SOC_ValueVoltGet + SOCInfo.SOC_ValueRealtimeDiff;         //���ϴζ�ȡ��SOC�ۼ�SOC   

  if(SOCInfo.SOC_CalTime % 10==0) //SOC_ReadУ��
  {
    SOCInfo.SOC_CalTime = 0;
    if((abs(current)>=0.5) && ((abs(SOCInfo.SOC_ValueRealtimeDiff))>=0.001))
    {
       if( (g_WorkStateJudge.WorkState == MODE_CHARGE) && ((SOCInfo.SOC_ValueVoltGet >= 0.9)))     //��磬S_V>0.9
       {
         if(SOCInfo.SOC_ValueVoltGet < 1)
         {QQ[0]++;
            SOCInfo.SOC_ValueRealtimeDiff = SOCInfo.SOC_ValueRealtimeDiff - (abs(current)/(400.0*SYS_CAPACITY))*SOCInfo.SOC_ValueInitDiff ;    //S_V>0.9 && S_V<1     
         } 
         else  
         { QQ[1]++;
            SOCInfo.SOC_ValueRealtimeDiff = SOCInfo.SOC_ValueRealtimeDiff - (1.0/180.0)*SOCInfo.SOC_ValueInitDiff ;     //S_V>=1
         }
       }
       else if((SOCInfo.SOC_ValueVoltGet<0.2) && (g_WorkStateJudge.WorkState == MODE_DISCHARGE))        //�ŵ磬S_V<0.2
       {
         if(SOCInfo.SOC_ValueVoltGet > 0)
         {  QQ[2]++;
            SOCInfo.SOC_ValueRealtimeDiff = SOCInfo.SOC_ValueRealtimeDiff - (abs(current)/(400.0*SYS_CAPACITY))*SOCInfo.SOC_ValueInitDiff ;    //S_V>0 && S_V<0.2 
         } 
         else  
         {  QQ[3]++;
            SOCInfo.SOC_ValueRealtimeDiff = SOCInfo.SOC_ValueRealtimeDiff - (abs(current)/(120.0*SYS_CAPACITY))*SOCInfo.SOC_ValueInitDiff ;    //S_V<=0
         }      
       }
       else
       {   QQ[4]++;
          SOCInfo.SOC_ValueRealtimeDiff = SOCInfo.SOC_ValueRealtimeDiff - (abs(current)/(1200.0*SYS_CAPACITY))*SOCInfo.SOC_ValueInitDiff ;     //���S_V 0~0.9 || �ŵ�S_V 0.2~1
       }
    }
  }
  if(SOCInfo.SOC_ValueRead >= 1) 
  {
     SOCInfo.SOC_ValueRead = 1;
  }
  else if(SOCInfo.SOC_ValueRead <= 0) 
  {
     SOCInfo.SOC_ValueRead = 0;
  }     
  //EEprom_Data.Charge_Times = EEprom_Data.Charge_Times + (uint16)(EnergyInfo.Energy_Total_Charge/SysEnergy); //������ 
  SOCInfo.SOC_CalTime++; 
} 

/*=======================================================================
 *������:      CellCapacity_Cal
 *����:        ��ͬ�¶��µ�ص�������һ��
 *����:        temp������������ƽ���¶�                
 *���أ�       float���ڴ��¶���ʵ��õ��ĵ������
 *˵����       �ڲ�ͬ���¶��µ�ص�������һ��
========================================================================*/ 
float CellCapacity_Cal(uint8 tempave)
{
  uint8 i;
  float cap[6] = {26.1196, 32.4033, 36.3806, 41.9106, 44.5642, 47.2677}; //�������
  float temp[6] = {   -20,     -10,       0,      10,      25,      45}; //����¶�
  
  if(tempave<=temp[0])
  {
    return cap[0];
  }
  
  else if(tempave>=temp[5])
  {
    return cap[5];
  }
  else
  {
     for(i=0; i<6; i++)
     {
        if(tempave == temp[i])
        {
          return cap[i];
        }
        else if(tempave < temp[i+1] && (tempave > temp[i]))
        {
           return (cap[i]+(tempave-temp[i])*(cap[i+1]-cap[i])/(temp[i+1]-temp[i]));
        }
     }
  }
}

/*=======================================================================
 *������:      MaxVolt_Cal
 *����:        ��ͬ�¶��µ�ص�SOC����ʱ��ߵ�ѹ��ͬ
 *����:        temp������������ƽ���¶�                
 *���أ�       float���ڴ��¶���ʵ��õ��ĵ������
 *˵����       �ڲ�ͬ���¶���SOC�ն�У���ĵ�ѹ��һ��
========================================================================*/ 
/*
float MaxVolt_Cal(uint8 temp)
{
  if(temp>=10+40)
  {
    return 4.3;
  }
  else if(temp>=5+40)
  {
    return 4.2;
  }
  else
  {
    return 3.9;
  }
}
*/
/*=======================================================================
 *������:      float inition_soc(float v)
 *����:        SOC��ʼ�����������ȡSOCֵ
 *����:        ��       
 *���أ�       ��
 *˵����       ��ѹ��SOC��ֵ   soc��Ϊfloat��V��Ϊunsigned short 
========================================================================*/
static
float inition_soc(float v)
{  
  static float soc;
	static uint8 i;              
  #if(CELL_TYPE == 0x03) 
    //������﮵��
   //	float socdata[15]={  0 ,   0.1,    0.2,    0.3,    0.4,    0.5,    0.6,    0.7,    0.8,    0.9,   0.92,   0.94,   0.96,   0.98,     1 };	
   //	float uocdata[15]={2.9, 3.2013, 3.2432, 3.2733, 3.2776, 3.2857, 3.2930, 3.3189, 3.3191, 3.3196, 3.3200, 3.3205, 3.3210, 3.3220, 3.3312};
   //¥��100AH������﮵��
   static float socdata[11]={     0,    0.1,    0.2,    0.3,    0.4,    0.5,    0.6,    0.7,    0.8,    0.9,     1};
   static float uocdata[11]={3.1400, 3.2150, 3.2520, 3.2780, 3.2920, 3.2930, 3.2950, 3.3060, 3.3320, 3.3340,3.5470};

  	if(v < uocdata[0])
  	{
  	   soc=0 ;
  	} 
  	else if(v>uocdata[10]) 
  	{
  	   soc=1;    	  
  	} 
    else 
  	{ 	  
    	for(i=0;i<11;i++)
    	{  		    
    	 if (v==uocdata[i])
    	  {
    	  	 soc=socdata[i];
    	     break;
    	  } 	
    	  else if ((v<uocdata[i+1]) && (v>uocdata[i])) 
    	  {
  	    	soc=socdata[i]+(v-uocdata[i] )*(socdata[i+1]-socdata[i])/(uocdata[i+1]-uocdata[i]);
        	break;
    	  }	  	  	  
    	} 
  	}  
  #endif 
	return soc;    	
}

/*=======================================================================
 *������:      SOC_StaticTime
 *����:        ���㵱ϵͳΪ����ʱ��ϵͳSOC����ʱ��
 *����:        readTime:ʵʱ��ȡ��ʱ��
               current��ʵʱ���������Ĵ�С 
        
 *���أ�       uint8�������Ƿ���Ҫ���,1:���в��;0:����� 
 *˵����       ��ϵͳΪ����ʱ���жϵ���Ϊ0��SOC��[0~20]��[90~100]��Χ��ʱ�䳬��
               3Сʱʱ���в���ʼ��ʼ��
========================================================================*/
static
uint8 SOC_StaticTime(uint8 readhour, uint8 readminite, float current)
{
    static uint8 Time_firstflag;
    static uint8 cnt[2];
    if(abs(current)<0.5)
    {  
      if(++cnt[0]*SOC_Period/1000>20) //��20S�ڶ��ǵ���С��0.5A��ʼ��ʱ
      {  
        cnt[0] = 20; 
        cnt[1] = 0;
        if(Time_firstflag == 0)
        { 
           Time_firstflag = 1;
           SOCInfo.SOC_CheckTable_StartTime = ((uint16)readhour<<8) | readminite ;           
        }
        else
        {
          if((((uint16)readhour<<8) | readminite)-SOCInfo.SOC_CheckTable_StartTime >= 0)
          {
            if((((uint16)readhour<<8) | readminite)-SOCInfo.SOC_CheckTable_StartTime >= ((uint16)4<<8))//����4Сʱ���
            {
                return TRUE;
            }
          }
          else
          {
            if(((uint16)24<<8)-SOCInfo.SOC_CheckTable_StartTime+(((uint16)readhour<<8) | readminite) >= ((uint16)4<<8))
            {   
                return TRUE;
            }
          }
        }
      }
    }
    else if(++cnt[1]*SOC_Period/1000>2)//���������������2s��Ϊ���Ǵ��ڷǹ���״̬
    {
       cnt[0] = 0;
       cnt[1] = 2;
       Time_firstflag = 0;
    }
    return FALSE;
}

/*=======================================================================
 *������:      Energy_TotalCal(uint8 mode,float ah1,uint32 Total_V,float Total_E,float current)
 *����:        �����ۼ�����
 *����:        mode:����״̬
               ah1:ʵʱ��ʱ
               Total_V:�����ѹ�ܺ�(��λ��Ҫת��ΪV) 
               Total_E:������
               current:ʵʱ���� 
 *���أ�       Total_E:������
 
 *˵����       
========================================================================*/ 
static
float Energy_TotalCal(uint8 mode,float ah1,uint32 Total_V,float Total_E,float current)
{
  static float Energy=0;
  static float mode_n;
  
  if(mode == MODE_CHARGE)
  {
    mode_n = (-1);
  }
  else
  {
    mode_n = 1;
  }
  Energy = mode_n*ah1*(((float)Total_V)/10000.0)/1000.0; 
  if(abs(current) > 0.5) 
  {        
    Total_E = Total_E + Energy;//���������KWH
  }
  return (Total_E);
}

/*=======================================================================
 *������:      SOH_Cal
 *����:        SOH�ļ���
 *����:        ��                
 *���أ�       ��
 *˵����       ������ĳ������������ص�ʹ������˥��
========================================================================*/  
void SOH_Cal(void) 
{
   uint32 Bms_Life_Count;
     
   Bms_Life_Count = CELL_LIFE_CYCLE*SYS_CAPACITY;  //������(2000)*��������
   BMSMonitor_SOH.SOH = 1 - (EnergyInfo.Energy_Total_Charge/SYS_VOLT_NOMINAL/1000.0/CELL_LIFE_CYCLE*0.2);   //��ʼSOH-����ֵ(�궨ֵ��Bms_Life_Count)

}

//=======================================================================
//=============================δ���ú���================================
//=======================================================================
   
   //SOH_Cal();              //����SOHֵ      
   /*
   if(TimeCount%300 == 0)  //��һ��ʱ��洢���ݣ�30s�洢�ٶȣ�
   { 
       TimeCount = 0;
        //BMS����ʱ��ļ��� 
       DS3231_Read_Time();  //��ȡ��ʱ(��ȷ������)                                                                          
       EEprom_Data.SOC_Counter_Time = ((uint32)Read_IIC_Time.IIC_Read_Year<<24) +((uint32)Read_IIC_Time.IIC_Read_Month<<16)+ ((uint32)Read_IIC_Time.IIC_Read_Day<<8) + (uint32)(Read_IIC_Time.IIC_Read_Hour);
       
       Time_Interval = Read_IIC_Time.IIC_Read_Minute;       //��ʱBMS��ȡ��ʱ��(Сʱ)
       if(Time_Interval - SOC_DATA.Start_Run_Time >= 0)
       {
          Value = Time_Interval - SOC_DATA.Start_Run_Time;  //���ϵ絽����BMS����ʱ������
       }
       else
       {
          Value = 60 - SOC_DATA.Start_Run_Time + Time_Interval;
       }
       SOC_DATA.Start_Run_Time = Time_Interval; 
      
       SOC_DATA.Bms_Run_Time_min = Value + SOC_DATA.Bms_Run_Time_min;  //�ϵ絽����BMS����ʱ�������ۼ�
       
       EEprom_write(EEprom_Baseadrr);          
   }*/
       
  


