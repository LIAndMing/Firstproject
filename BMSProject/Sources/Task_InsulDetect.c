 /*=======================================================================
 *Subsystem:   ���
 *File:        Insulation.c
 *Author:      WenMing
 *Description: �ӿ�
               ��ѹ��⿪�أ��P               PB0                 
               ��ԵV+���أ�                   PB1
               ��ԵV-���أ�                   PB2
               ��Ե��ѹ�����⿪������       PB3
               ��Ե��ѹ�����⿪������       PB4
               ��ѹ���                       PAD00
               V+��ѹ���:                    PAD01
               V-��ѹ���:                    PAD08
               ����Ƶ��:                      2MHz
 =============================================================================
 * History:    �޸���ʷ��¼�б�
 * 1. Date:    
      Author:  
      Modification:
===============================================================================*/
 
 
#include"Task_InsulDetect.h"
#include"ADC_Config.h"


  IsoResist IsoDetect;        //�����Ե���ṹ��
  
/*=======================================================================
 *������:      VoltageDetect
 *����:        ��ذ���ѹ����ʼ������ѹ��⣬��Ե��⣩
 *����:        ��         
                      
 *���أ�       ��
 *˵����       ADC ʱ��Ƶ�ʣ�2MHz
========================================================================*/
  void VoltageDetectInit(void) 
  {
     HighVoltS1_Dir = 1;
     HighVoltS1 = INS_SwitchOFF;      //��Ե���V+���عر�
     
     HighVoltS2_Dir = 1; 
     HighVoltS2 = INS_SwitchOFF;      //��Ե���V-���عر�
     
     memset(&IsoDetect,0,sizeof(IsoResist));     
  }

/*=======================================================================
 *������:      InsulationDetect
 *����:        ��ذ���Ե���
 *����:        ��         
                      
 *���أ�       ��
 *˵����       ADC ʱ��Ƶ�ʣ�2MHz
========================================================================*/
  void Task_InsulationDetect(void) 
  {
      uint8 count,i,j;
      static uint8 Time_Flag,Time_Cnt;
      uint32 SumVpositive,SumVnegtive,total_VOL,total_VOL1;
      static uint16 VoltFlag1,VoltFlag2;
      float Vpositive_1,Vnegtive_1;
      uint16 VposBuff[12],VnegBuff[12],VposBuff1[12],VnegBuff1[12],VposBuff2[12],VnegBuff2[12];
      uint16 Max_Volt,Max_Volt1,Min_Volt,Min_Volt1;
      
      //Task_PITCNT.InsulationDetect_PITCNT[0] = PITCNT0; 
      //Task_PITCNT.Runtime_flag = 0;

      if(Time_Flag == 0) 
      { 
        Time_Cnt = 0;           
        for(count = 0; count < 12; count++)//����12����ȥ�����ֵ�����ֵ 
        {
          VposBuff[count] = ADCValue(HVPositiveChannel);   //���Ե��̣�PAD1
          
          VnegBuff[count] = ADCValue(HVNegtiveChannel);    //���Ե��̣�PAD8
          
          //delayus(500);
          
        }    
        
        Max_Volt = 0;                 
        Max_Volt1 = 0; 
        Min_Volt = 0xFFFF;
        Min_Volt1 = 0xFFFF;
        SumVpositive =0;
        SumVnegtive =0;
        
        for(i =0; i<12; i++)            // ����ÿһ�������е������Сֵ(12��)
        {
           if(VposBuff[i] >=Max_Volt)
           {
              Max_Volt = VposBuff[i];
           } 
           if(VposBuff[i] <= Min_Volt)
           {
              Min_Volt = VposBuff[i];
           }
           
           if(VnegBuff[i] >Max_Volt1)
           {
              Max_Volt1 = VnegBuff[i];
           } 
           if(VnegBuff[i] < Min_Volt1)
           {
              Min_Volt1 = VnegBuff[i];
           }
        }
        
        for(count = 0; count < 12; count++)      //������ֵȫ�������� //
        {
           SumVpositive += VposBuff[count];
           SumVnegtive += VnegBuff[count];
        }
                
        SumVpositive = SumVpositive - Max_Volt - Min_Volt;
        SumVnegtive = SumVnegtive - Max_Volt1 - Min_Volt1; 
                
        IsoDetect.insulation_Vposit = SumVpositive/10.0;
        IsoDetect.insulation_Vnegt = SumVnegtive/10.0;               // ȥ�������Сֵ֮����ƽ��ֵ
                
        SumVpositive =0;
        SumVnegtive =0;
      
        IsoDetect.insulation_TotalVolt = ((IsoDetect.insulation_Vposit + IsoDetect.insulation_Vnegt) * Stand_Volt)/4096.0/24.9*(10024.9); //��ѹ 
       
      }
       
      if(IsoDetect.insulation_TotalVolt <= LowVoltageDec)            // �ж���ѹֵ�Ƿ���Ҫ���о�Ե���
      {
          IsoDetect.insulation_grade = 0x00;
          IsoDetect.insulation_curr = 0xaa;
          IsoDetect.insulation_resist = 0x2710;    //��ͨ��Э�� 10M
      } 
      else
      {                                            // ����ѹ����LowVoltageDec������ֵ         
         if(IsoDetect.insulation_Vposit >= IsoDetect.insulation_Vnegt)                 //����>=����
         {           
           Time_Flag = 1;                
           if(Time_Cnt == 0)
           {
             HighVoltS1 = INS_SwitchON;                // ��Ե���V+���رպ�,�պϺ�ͣһ��ʱ���ڼ���ѹֵ���ӳ�ʱ�����  //
             HighVoltS2 = INS_SwitchOFF;
                            // ��Ե���V-���ضϿ� //
           }
          
           if(Time_Cnt >= 1) 
           {             
             Time_Flag = 0;
             Time_Cnt = 0;
                                      
             for(count = 0; count < DetectCount; count++)       // ��ΪֻҪ�����ĵ�ѹ�������Ƿ����ȥ�� //
             {
                VposBuff1[count] = ADCValue(HVPositiveChannel);   //���Ե��̣�PAD1
                VnegBuff1[count] = ADCValue(HVNegtiveChannel);    //���Ե��̣�PAD8
               
             }           
              Max_Volt = 0;                 
              Max_Volt1 = 0; 
              Min_Volt = 0xFFFF;
              Min_Volt1 = 0xFFFF;
              for(i =0; i< DetectCount;i++)            // ����ÿһ�������е������Сֵ //
              {
                 if(VposBuff1[i] >Max_Volt)
                 {
                    Max_Volt = VposBuff1[i];
                 } 
                 if(VposBuff1[i] < Min_Volt)
                 {
                    Min_Volt = VposBuff1[i];
                 }
                 
                 if(VnegBuff1[i] >Max_Volt1)
                 {
                    Max_Volt1 = VnegBuff1[i];
                 } 
                 if(VnegBuff1[i] < Min_Volt1)
                 {
                    Min_Volt1 = VnegBuff1[i];
                 }
              }
              
              for(count = 0; count < 12; count++)           // ������ֵȫ�������� //
              {
                 SumVpositive += VposBuff1[count];
                 SumVnegtive += VnegBuff1[count];
                 
              }                                             // ��ȥ�����Сֵ����ʮ������ƽ��ֵ//
              SumVpositive = SumVpositive - Max_Volt - Min_Volt;
              SumVnegtive = SumVnegtive - Max_Volt1 - Min_Volt1; 
              
              Vpositive_1 = SumVpositive/10.0;
              Vnegtive_1 = SumVnegtive/10.0;                // ȥ�������Сֵ֮����ƽ��ֵ// 
              
              
              SumVpositive=0;
              SumVnegtive=0;    
              
              HighVoltS1 = INS_SwitchOFF;
              HighVoltS2 = INS_SwitchOFF; 
              if(IsoDetect.insulation_Vposit<0.1)
              {
                IsoDetect.insulation_resist = (uint16)0x2710;   //��������
                IsoDetect.insulation_resist_N = (uint16)0x2710; //��������
                IsoDetect.insulation_resist_P = (uint16)(abs(IsoDetect.insulation_Vposit-Vpositive_1)*1.0/Vpositive_1*(1+IsoDetect.insulation_Vposit/IsoDetect.insulation_Vnegt) * Bias_Resitance);//�������� 
              }
              else if(IsoDetect.insulation_Vnegt<0.1)
              {
                IsoDetect.insulation_resist = (uint16)(abs(IsoDetect.insulation_Vposit-Vpositive_1)/Vpositive_1*(1+IsoDetect.insulation_Vnegt/IsoDetect.insulation_Vposit) * Bias_Resitance); //��������
                IsoDetect.insulation_resist_N = (uint16)(abs(IsoDetect.insulation_Vposit-Vpositive_1)*1.0/Vpositive_1*(1+IsoDetect.insulation_Vnegt/IsoDetect.insulation_Vposit) * Bias_Resitance); //��������
                IsoDetect.insulation_resist_P = (uint16)0x2710; //�������� 
              }
              else
              {
                IsoDetect.insulation_resist = (uint16)(abs(IsoDetect.insulation_Vposit-Vpositive_1)/Vpositive_1*(1+IsoDetect.insulation_Vnegt/IsoDetect.insulation_Vposit) * Bias_Resitance); //��������
                IsoDetect.insulation_resist_N = (uint16)(abs(IsoDetect.insulation_Vposit-Vpositive_1)*1.0/Vpositive_1*(1+IsoDetect.insulation_Vnegt/IsoDetect.insulation_Vposit) * Bias_Resitance); //��������
                IsoDetect.insulation_resist_P = (uint16)(abs(IsoDetect.insulation_Vposit-Vpositive_1)*1.0/Vpositive_1*(1+IsoDetect.insulation_Vposit/IsoDetect.insulation_Vnegt) * Bias_Resitance);//�������� 
              }
           }
         }  
          
         else                                   //����<����
         {  
           Time_Flag = 1; 
           if(Time_Cnt == 0)
           {                    
             HighVoltS1 = INS_SwitchOFF;              //V+���ضϿ�
             HighVoltS2 = INS_SwitchON;               //V-���رպ�
           }
           if(Time_Cnt>0) 
           {         
             Time_Flag = 0;
             Time_Cnt = 0;
           
             for(count = 0; count < DetectCount; count++)    // ֻҪ�˸����ĵ�ѹ�������Ƿ����ȥ�� //
             {
               VposBuff2[count] = ADCValue(HVPositiveChannel);  //���Ե��̣�PAD1
               VnegBuff2[count] = ADCValue(HVNegtiveChannel);   //���Ե��̣�PAD8            
             }  
                        
              Max_Volt = 0;                 
              Max_Volt1 = 0; 
              Min_Volt = 0xFFFF;
              Min_Volt1 = 0xFFFF;
              for(i =0; i< DetectCount;i++)            // ����ÿһ�������е������Сֵ //
              {
                 if(VposBuff2[i] >Max_Volt)
                 {
                    Max_Volt = VposBuff2[i];
                 } 
                 if(VposBuff2[i] < Min_Volt)
                 {
                    Min_Volt = VposBuff2[i];
                 }
                 
                 if(VnegBuff2[i] >Max_Volt1)
                 {
                    Max_Volt1 = VnegBuff2[i];
                 } 
                 if(VnegBuff2[i] < Min_Volt1)
                 {
                    Min_Volt1 = VnegBuff2[i];
                 }
              }
              
              for(count = 0; count < 12; count++)      // ������ֵȫ�������� //
              {
                 SumVpositive += VposBuff2[count];
                 SumVnegtive += VnegBuff2[count];
             
              }
                                            // ��ȥ�����Сֵ����ʮ������ƽ��ֵ //
              SumVpositive = SumVpositive - Max_Volt - Min_Volt;
              SumVnegtive = SumVnegtive - Max_Volt1 - Min_Volt1; 
              Vpositive_1 = SumVpositive/10.0;
              Vnegtive_1 = SumVnegtive/10.0;              // ȥ�������Сֵ֮����ƽ��ֵ// 
              
              
              HighVoltS1 = INS_SwitchOFF;
              HighVoltS2 = INS_SwitchOFF; 
              
              
              SumVpositive=0;
              SumVnegtive=0;
              if(IsoDetect.insulation_Vposit<0.1)
              {
                IsoDetect.insulation_resist = (abs(IsoDetect.insulation_Vposit-Vnegtive_1)*1.0/Vnegtive_1*(1+IsoDetect.insulation_Vposit/IsoDetect.insulation_Vnegt) * Bias_Resitance);      
                IsoDetect.insulation_resist_P = (uint16)(abs(IsoDetect.insulation_Vnegt-Vnegtive_1)*1.0/Vnegtive_1*(1+IsoDetect.insulation_Vposit/IsoDetect.insulation_Vnegt) * Bias_Resitance);//��������
                IsoDetect.insulation_resist_N = (uint16)0x2710;
              }
              else if(IsoDetect.insulation_Vnegt<0.1)
              {
                IsoDetect.insulation_resist = (uint16)0x2710;      
                IsoDetect.insulation_resist_P = (uint16)0x2710;//��������
                IsoDetect.insulation_resist_N = (uint16)(abs(IsoDetect.insulation_Vnegt-Vnegtive_1)/Vnegtive_1*(1+IsoDetect.insulation_Vnegt/IsoDetect.insulation_Vposit) * Bias_Resitance); //��������
              }
              else
              {                
                IsoDetect.insulation_resist = (abs(IsoDetect.insulation_Vposit-Vnegtive_1)*1.0/Vnegtive_1*(1+IsoDetect.insulation_Vposit/IsoDetect.insulation_Vnegt) * Bias_Resitance);      
                IsoDetect.insulation_resist_P = (uint16)(abs(IsoDetect.insulation_Vnegt-Vnegtive_1)*1.0/Vnegtive_1*(1+IsoDetect.insulation_Vposit/IsoDetect.insulation_Vnegt) * Bias_Resitance);//��������
                IsoDetect.insulation_resist_N = (uint16)(abs(IsoDetect.insulation_Vnegt-Vnegtive_1)/Vnegtive_1*(1+IsoDetect.insulation_Vnegt/IsoDetect.insulation_Vposit) * Bias_Resitance); //��������
              }
           }         
        }          
      //�жϾ�Ե����
      if(IsoDetect.insulation_resist > 0x2710)                   //10M
      {
        IsoDetect.insulation_grade = 0x00;
        IsoDetect.insulation_curr = 0xaa;
        IsoDetect.insulation_resist = 0x2710;                    //��ͨ��Э�� 10M
      }    
      else 
      {
          if(IsoDetect.insulation_resist > Resistance_Alarm1)       //�޹���
          {
            IsoDetect.insulation_grade = 0x00;
            IsoDetect.insulation_curr = 0xaa;      
          } 
          if(IsoDetect.insulation_resist < Resistance_Alarm2)       //��߾�Ե����
          {
            IsoDetect.insulation_grade = 0x02;
            IsoDetect.insulation_curr = 0x55;
          }
          else                                                      //�μ�����
          {
            IsoDetect.insulation_grade = 0x01;
            IsoDetect.insulation_curr = 0x55;
          }   
      }  
    }   
    
    Time_Cnt++;    
    //Task_Flag_Counter.Counter_Insulation++;
    
    //Task_PITCNT.InsulationDetect_PITCNT[1] = PITCNT0;
    //Task_Runtime.InsulationDetect_Runtime = (Task_PITCNT.Runtime_flag*1280 + Task_PITCNT.InsulationDetect_PITCNT[0] - Task_PITCNT.InsulationDetect_PITCNT[1])*7.8125*0.001;
      //Task_PITCNT.Runtime_flag = 0;
  }