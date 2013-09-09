/*******************************************************************
*
*    FILENAME             :
*
*    COPYRIGHT            :    深圳市讯方通信技术有限公司
*
*    MODULE NAME          :
*
*    CPU                  :    stm32F103RBT6
*
*    RTOS                 :    
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
*    Multi-Include-Prevent Section
*******************************************************************/
#ifndef __PUBLICFUC_H__
#define __PUBLICFUC_H__
/*******************************************************************
*    Debug switch Section
*******************************************************************/

/*******************************************************************
*    Include File Section
*******************************************************************/
#include "STM32Lib\\stm32f10x.h"
#include "UCOSII\\ucos_ii.h"
#include "CFG.h"
#include "mytype.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
/*******************************************************************
*    Macro Define Section 
*******************************************************************/
#define TICK_ENABLE() (SysTick->CTRL = (SysTick->CTRL)|(1<<SYSTICK_ENABLE)|(1<<SYSTICK_TICKINT))
#define TICK_DISABLE() (SysTick->CTRL = (SysTick->CTRL)&(~((1<<SYSTICK_ENABLE)|(1<<SYSTICK_TICKINT))))

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL  0
#endif

/*  求两个数据的绝对差值  */
#define DIFF(a, b)    ((a)<=(b) ?(b-a):(a-b))
/*******************************************************************
*    Struct Define Section
*******************************************************************/

/*******************************************************************
*    Prototype Declare Section
*******************************************************************/
extern u8 cpu_id[12];

void Get_ID(void);
void ChNumtoStr(char *numStr,uchar num);
u16 ChStrtoNum(char *numStr);
void StringAddChar(u8 ch, char*str);
void StringShiftRightOne(char *str);
uint UcharToUint(uchar ah,uchar al);
u8 CharToAscii(u8 ch);
u8 AsciiToChar(u8 asc);
void DemoDly_ms_72M(uint ms);
void DemoDly_ms_8M(uint ms);
void TIM2_Delay_Config(void);
void delay_us(u16 cnt);
#endif

