/*******************************************************************
*
*    FILENAME(文件名)     :    Include.H
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
#ifndef __Include_H__
#define __Include_H__
/*******************************************************************
*    Debug switch Section
*    调试开关
*******************************************************************/

/*******************************************************************
*    Include File Section
*    包含头文件
*******************************************************************/
#include "main.h"
#include "mytype.h"
#include "publicFuc.h"
#include "STM32Lib\\stm32f10x.h"
#include "ucosii\\ucos_ii.h"
#include "publicFuc.h"
#include "leds\\leds.h"
#include "key\\key.h"
#include "zigbee\\zigbee.h"
#include "crc8_16\\crc8_16.h"
#include "timer\\timer.h"
#include "com\\com.h"
#include "bsp\\bsp.h"
#include "can\\can.h"
#include "protocol\\protocol.h"
#include "protocol\\zjy_pro.h"
#include "protocol\\syf_pro.h"
#include "malloc\\malloc.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
/*******************************************************************
*    Macro Define Section
*    宏定义 
*******************************************************************/

/*******************************************************************
*    Struct Define Section
*    结构体定义
*******************************************************************/

/*******************************************************************
*    Global Varible Declare
*    全局变量声明
*******************************************************************/

/*******************************************************************
*    Prototype Declare Section
*    内部函数原型声明
*******************************************************************/
extern u32 MsCnt;

#endif

