/*=======================================================================
 *Subsystem:   ���
 *File:        Task_Create.C
 *Author:      Wenming
 *Description: ͨ�ţ�
               �ӿڣ�
               �����ʣ�
 ========================================================================
 * History:    �޸���ʷ��¼�б�
 * 1. Date:
      Author:
      Modification:
========================================================================*/

#include "Task_Init.h" 

TASK tasks[ARRAY_SIZE];            //�����ջ���     
/*=======================================================================
 *������:      Task_Init(uint8 FLAGS, void (*HANDLE)())  
 *����:        ���񴴽�����
 *����:        ��       
 *���أ�       ��
 *˵����       ÿ�����񴴽���Ҫһ��ʱ���ʱ�ı�־λ�뺯��ָ�룻
========================================================================*/
TASK Task_Init(uint8 FLAGS, void (*HANDLE)())             // ����������ṹ�庯����
{
	TASK task;
	task.flags = FLAGS;
	task.handle = HANDLE;
	return (task);
}

/*=======================================================================
 *������:      Task_Handle(void)  
 *����:        ������ѯ
 *����:        ��       
 *���أ�       ��
 *˵����       �����������麯����������ѯִ������
========================================================================*/
void Task_Handle(void)                               // ����ִ�к�����������Ĵ���˳��ִ������                     
{
  uint8 i;
  for (i = 0; i < ARRAY_SIZE; i++)
  {   
     if (tasks[i].flags==1)                    // ��ѯ����ʱ���Ƿ񵽣�
     {             
        (*tasks[i].handle)();                  // ִ������
        tasks[i].flags = 0;                    // ����������λ              
     }        
  }  
}
   
/*=======================================================================
 *������:      Task_Create(void)  
 *����:        �����񴴽�����
 *����:        ��       
 *���أ�       ��
 *˵����       �����������麯����������ѯִ������
========================================================================*/  
void Task_Create()                                      // ������������
{  
   //BMS���ݽ��ռ���������     
   /*tasks[0] = CreateTask(0, TaskVAndTDataProcess);    //��ѹ�¶ȴ����� 
   tasks[1] = CreateTask(0, Task_Soccal);               //SOC����
   tasks[2] = CreateTask(0, Dis_ChargeCurrent);         //��ŵ����������
   tasks[3] = CreateTask(0, Task_FailLevel_Judge);      //��ŵ���ϵȼ��ж�   
   tasks[4] = CreateTask(0, BMSCheckSelf);              //BMS�Լ�  
   tasks[5] = CreateTask(0, InsulationDetect);          //��Ե��� 
   
   
   
   //���͸�VCU����Ϣ
   tasks[6] = CreateTask(0, Task_ToVCU_BasicInfo);      //200ms 
   tasks[7] = CreateTask(0, Task_ToVCU_Temp);           //200ms
   tasks[8] = CreateTask(0, Task_ToVCU_Volt);           //200ms
   tasks[9] = CreateTask(0, Task_ToVCU_Error);          //200ms  
   
   //���͸��������Ϣ
   //tasks[11] = CreateTask(0, Task_To_SlowCharge);       //1s
   
   //�������
   tasks[10] = CreateTask(0, Task_Charge);              //250ms  
   //tasks[12] = CreateTask(0, Out_Queue_FromCharge);   //50ms ������ݳ��ӽ�����ȡ����         
   tasks[11] = CreateTask(0, BMSToCharge_BHM);          //250ms       
   tasks[12] = CreateTask(0, BMSToCharge_BRM);          //250ms  
   tasks[13] = CreateTask(0, BMSToCharge_BCP);          //500ms 
   tasks[14] = CreateTask(0, BMSToCharge_BRO);          //250ms
   tasks[15] = CreateTask(0, BMSToCharge_BCL);          //50ms       
   tasks[16] = CreateTask(0, BMSToCharge_BCS);          //250ms  
   tasks[17] = CreateTask(0, BMSToCharge_BSM);          //250ms 
   tasks[18] = CreateTask(0, BMSToCharge_BST);          //10ms
   tasks[19] = CreateTask(0, BMSToCharge_BSD);          //250ms 
   tasks[20] = CreateTask(0, BMSToCharge_BEM);          //250ms    
   
   //����λ��ͨ������        
   tasks[21] = CreateTask(0, Bms_to_Up_Monitor);        //�ϴ����� 
   tasks[22] = CreateTask(0, Task_BootLoader);          //boot����1s    
   tasks[23] = CreateTask(0, PowerONandOFF_Control);    //��Ƭ�����µ����
   
   //tasks[24] = CreateTask(0, GPIO_test);    //��Ƭ�����µ����
   */
}



