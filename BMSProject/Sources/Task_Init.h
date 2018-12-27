#ifndef  TASK_CREATE_
#define  TASK_CREATE_

  #include  "TypeDefinition.h"
  
  #define ARRAY_SIZE   24                // �����ջ��С�� 

  typedef struct                         // ����ṹ�壻
  {
      uint8 flags;                       // �����ʶ��  
      void (*handle)();                  // ���������ĺ���ָ�룻 
  }TASK;   
  extern TASK tasks[ARRAY_SIZE]; 

  TASK Task_Init(uint8 FLAGS, void (*HANDLE)());  //�����ṹ���ʼ��
  void Task_Handle(void);                         //������ѯ
  void Task_Create();                             //������Ĵ���

#endif