/*******************************************************************
*
*    FILENAME(�ļ���)     :    Include.H
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
#ifndef __Include_H__
#define __Include_H__
/*******************************************************************
*    Debug switch Section
*    ���Կ���
*******************************************************************/

/*******************************************************************
*    Include File Section
*    ����ͷ�ļ�
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
*    �궨�� 
*******************************************************************/

/*******************************************************************
*    Struct Define Section
*    �ṹ�嶨��
*******************************************************************/

/*******************************************************************
*    Global Varible Declare
*    ȫ�ֱ�������
*******************************************************************/

/*******************************************************************
*    Prototype Declare Section
*    �ڲ�����ԭ������
*******************************************************************/
extern u32 MsCnt;

#endif

