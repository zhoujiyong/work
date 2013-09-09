/*******************************************************************
*
*    FILENAME(文件名)     :    MAIN.H
*
*    COPYRIGHT(著作权)    :    深圳市讯方通信技术有限公司
*
*    MODULE NAME(组件名)  :
*
*    CPU                  :    stm32F103ZET6
*
*    RTOS(操作系统)       :    
*
*    CreateDate(创建时间) :
*
*    AUTHOR(作者)         :    
*
*    Abstract Description 
*    摘要描述             :
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
*    调试开关
*******************************************************************/

/*******************************************************************
*    Include File Section
*    包含头文件
*******************************************************************/
#include "ucosii\ucos_ii.h"
/*******************************************************************
*    Macro Define Section
*    宏定义 
*******************************************************************/
/* 任务堆栈大小 */
#define START_STK_SIZE	            256
#define ZIGBEE_RX_STK_SIZE          256
#define COM_RX_STK_SIZE             256
#define POWER_STK_SIZE				64

/* 任务优先级 */
#define START_TASK_PRIO	             5
#define ZIGBEE_MUTEX_PRIO  			 6
#define ZIGBEE_RX_TASK_PRIO  		 7
#define COM_RX_TASK_PRIO  			 9
#define POWER_TASK_PRIO  			 11
/*******************************************************************
*    Struct Define Section
*    结构体定义
*******************************************************************/

/*******************************************************************
*    Global Varible Declare
*    全局变量声明
*******************************************************************/
/*-----------  任务堆栈  ------------------*/
extern OS_STK  StartTaskStk[START_STK_SIZE];    
extern OS_STK  ZigbeeRxTaskStk[ZIGBEE_RX_STK_SIZE];
extern OS_STK  ComRxTaskStk[COM_RX_STK_SIZE];
/*******************************************************************
*    Prototype Declare Section
*    内部函数原型声明
*******************************************************************/
extern OS_EVENT *ZigbeeMutex;
extern OS_EVENT *PowerMbox;
#endif


  
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
