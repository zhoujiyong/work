/*******************************************************************
*
*    FILENAME             :  °´¼ü³ÌÐò
*
*    COPYRIGHT            :
*
*    MODULE NAME          :
*
*    CPU                  :
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
#ifndef __KEY_H__
#define __KEY_H__
/*******************************************************************
*    Debug switch Section
*******************************************************************/

/*******************************************************************
*    Include File Section
*******************************************************************/
#include "..\\include.h"
/*******************************************************************
*    Macro Define Section 
*******************************************************************/
#define KEY_WKUP_CLOCK  RCC_APB2Periph_GPIOA
#define KEY_WKUP_PORT   GPIOA
#define KEY_WKUP_PIN    GPIO_Pin_0

#define KEY_WKUP_IN()             (!(KEY_WKUP_PORT->IDR&KEY_WKUP_PIN))

#define KEY_WKUP_VAL  1
/*******************************************************************
*    Struct Define Section
*******************************************************************/

/*******************************************************************
*    Prototype Declare Section
*******************************************************************/
u8 PollingKey(void);
u8 DebounceKey(void);
void KeyInit(void);
void LockCheck(void);

void KeyTask(void *parg);

#endif





