/*******************************************************************
*
*    FILENAME             :
*
*    COPYRIGHT            :    ������Ѷ��ͨ�ż������޹�˾
*
*    MODULE NAME          :
*
*    CPU                  :    stm32F103RBT6
*
*    RTOS                 :    ucos-II
*
*    CreateDate           :
*
*    AUTHOR               :
*
*    Abstract Description :
*
*--------------------------Revision History------------------------
*
*    No    Version    Date    Revise By    Item    Description
*
*******************************************************************/

/*******************************************************************
*    Debug switch Section
*******************************************************************/

/*******************************************************************
*    Include File Section
*******************************************************************/
#include "include.h"
/*******************************************************************
*    Macro Define Section 
*******************************************************************/

/*******************************************************************
*    Struct Define Section
*******************************************************************/
/*-----------  �����ջ  ------------------*/
OS_STK  StartTaskStk[START_STK_SIZE];    
OS_STK  ZigbeeRxTaskStk[ZIGBEE_RX_STK_SIZE];
OS_STK  ComRxTaskStk[COM_RX_STK_SIZE];
OS_STK  PowerTaskStk[POWER_STK_SIZE];
/*******************************************************************
*    Prototype Declare Section
*******************************************************************/
void RCC_Configuration(void);
void NVIC_Configuration(void);
void StartTask(void *p_arg);
void SystemDemo_Init(void);
/*******************************************************************
*    Global Varible Declare
*******************************************************************/
OS_EVENT *ZigbeeMutex;
OS_EVENT *PowerMbox;
/*******************************************************************
*    File Static Variable Define Section
*******************************************************************/

/*******************************************************************
*    Function Define Section
*******************************************************************/
int main(void)
{
    u8 os_err;
	
    SystemDemo_Init();
    /*  UCOS INIT~  */
    OSInit();				
        
    os_err = OSTaskCreate( StartTask,	
                      (void          * ) 0,						
                      (OS_STK        * )&StartTaskStk[START_STK_SIZE - 1],	
                      (INT8U           ) START_TASK_PRIO
                     );
	os_err = os_err;
    OSStart(); 

    while(1);
}

u8 val=0;
u8 used=0;
void StartTask(void *p_arg)
{
	u8 os_err;
	
	IOT_DEBUGF(("StartTask\r\n"));
	/*  ʹ��SYSTICK�ж�  */
	OS_CPU_SysTickInit();	
	
#if (OS_TASK_STAT_EN > 0)
    OSStatInit(); /*�ⶫ�����Բ���CPUʹ���� */
#endif
    
	/*------------------  ��������ʼ��  -------------*/
	ZigbeeMutex = OSMutexCreate(ZIGBEE_MUTEX_PRIO, &os_err);
    /*------------------  �ź�����ʼ��  -------------*/

    /*------------------  �����ʼ��  --------------*/
	PowerMbox = OSMboxCreate((void *)0);
	/* ���ж� */
    NVIC_Configuration();
	/*-----------------  zigbee��������  --------------------------*/
    os_err = OSTaskCreate( ZigbeeRxTask,     /*  Zigbee Rx Task  */
               (void          * ) 0,	
               (OS_STK        * )&ZigbeeRxTaskStk[ZIGBEE_RX_STK_SIZE - 1],
               (INT8U           )ZIGBEE_RX_TASK_PRIO	
                 );
	/*-----------------  ���ڽ�������  --------------------------*/
	os_err = OSTaskCreate( ComRxTask,     /*  Com Rx Task  */
               (void          * ) 0,	
               (OS_STK        * )&ComRxTaskStk[COM_RX_STK_SIZE - 1],
               (INT8U           )COM_RX_TASK_PRIO	
                 );
	/*-----------------  ��Դ��������  --------------------------*/
	os_err = OSTaskCreate( PowerTask,     /*  Power Task  */
               (void          * ) 0,	
               (OS_STK        * )&PowerTaskStk[POWER_STK_SIZE - 1],
               (INT8U           )POWER_TASK_PRIO	
                 );

	os_err = os_err;
	UILED1Blink(2000, 50);
    for(;;)
    {	
		//OSTaskSuspend(OS_PRIO_SELF);	
		//Can_Send_Msg(&val, 1);
		//if(++val==8)
		//		val=0;
		//OSTimeDlyHMSM(0, 0, 1, 0);
		used = mem_perused(SRAMIN);
		OSTimeDlyHMSM(0, 0, 0, 2);
    }
}



