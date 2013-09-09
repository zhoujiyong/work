/*******************************************************************
*
*    FILENAME             :  RTC.H
*
*    COPYRIGHT            :  深圳市讯方通信技术有限公司
*
*    MODULE NAME          :
*
*    CPU                  :  stm32F103ZET6
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
#ifndef __RTC_H__
#define __RTC_H__
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
//用于时间
#define BKP_TIME_SET		BKP_DR1	//RTC设置与否标志
#define BKP_TIME_YEAR		BKP_DR2
#define BKP_TIME_MONTH		BKP_DR3
#define BKP_TIME_DAY		BKP_DR4
#define BKP_TIME_DATE		BKP_DR5

//没有设置时间下的默认时间
#define DEFAULT_TIME_YEAR	12
#define DEFAULT_TIME_MONTH	1
#define DEFAULT_TIME_DAY	4
#define DEFAULT_TIME_HOUR	20
#define DEFAULT_TIME_MINUTE	37
#define DEFAULT_TIME_SECOND	00
/*******************************************************************
*    Struct Define Section
*******************************************************************/
//时间结构体
typedef struct _t_sct{
u8 year;	//年	0-99
u8 month;	//月
u8 day;		//日
u8 hour;	//时间
u8 minute;	//分
u8 sec;		//秒
u8 date;	//星期
}T_STRUCT;
/*******************************************************************
*    Prototype Declare Section
*******************************************************************/
void RTCInit(void);
//RTC
extern void GetRTCTime(T_STRUCT*);
extern void SetRTCTime(T_STRUCT*);
extern u8 GetDate(T_STRUCT* time);
void GetTimeStr(u8 *time);
void AdjustTimeStr(u8 *time);

void RtcTask(void *parg);
#endif
