/*******************************************************************
*
*    FILENAME(�ļ���)     :    MAIN.H
*
*    COPYRIGHT(����Ȩ)    :    ������Ѷ��ͨ�ż������޹�˾
*
*    MODULE NAME(�����)  :
*
*    CPU                  :    stm32F103ZET6
*
*    RTOS(����ϵͳ)       :    
*
*    CreateDate(����ʱ��) :
*
*    AUTHOR(����)         :    
*
*    Abstract Description 
*    ժҪ����             :
*
*--------------------------Revision History------------------------
*
*    No    Version    Date    Revise By    Item    Description
*
*******************************************************************/

/*******************************************************************
*    Multi-Include-Prevent Section
*******************************************************************/
#ifndef __MAIN_H__
#define __MAIN_H__
/*******************************************************************
*    Debug switch Section
*    ���Կ���
*******************************************************************/

/*******************************************************************
*    Include File Section
*    ����ͷ�ļ�
*******************************************************************/
#include "ucosii\ucos_ii.h"
/*******************************************************************
*    Macro Define Section
*    �궨�� 
*******************************************************************/
/* �����ջ��С */
#define START_STK_SIZE	            256
#define ZIGBEE_RX_STK_SIZE          256
#define COM_RX_STK_SIZE             256
#define POWER_STK_SIZE				64

/* �������ȼ� */
#define START_TASK_PRIO	             5
#define ZIGBEE_MUTEX_PRIO  			 6
#define ZIGBEE_RX_TASK_PRIO  		 7
#define COM_RX_TASK_PRIO  			 9
#define POWER_TASK_PRIO  			 11
/*******************************************************************
*    Struct Define Section
*    �ṹ�嶨��
*******************************************************************/

/*******************************************************************
*    Global Varible Declare
*    ȫ�ֱ�������
*******************************************************************/
/*-----------  �����ջ  ------------------*/
extern OS_STK  StartTaskStk[START_STK_SIZE];    
extern OS_STK  ZigbeeRxTaskStk[ZIGBEE_RX_STK_SIZE];
extern OS_STK  ComRxTaskStk[COM_RX_STK_SIZE];
/*******************************************************************
*    Prototype Declare Section
*    �ڲ�����ԭ������
*******************************************************************/
extern OS_EVENT *ZigbeeMutex;
extern OS_EVENT *PowerMbox;
#endif


  
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
