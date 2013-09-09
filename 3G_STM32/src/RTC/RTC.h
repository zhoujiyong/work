/*******************************************************************
*
*    FILENAME             :  RTC.H
*
*    COPYRIGHT            :  ������Ѷ��ͨ�ż������޹�˾
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
//����ʱ��
#define BKP_TIME_SET		BKP_DR1	//RTC��������־
#define BKP_TIME_YEAR		BKP_DR2
#define BKP_TIME_MONTH		BKP_DR3
#define BKP_TIME_DAY		BKP_DR4
#define BKP_TIME_DATE		BKP_DR5

//û������ʱ���µ�Ĭ��ʱ��
#define DEFAULT_TIME_YEAR	12
#define DEFAULT_TIME_MONTH	1
#define DEFAULT_TIME_DAY	4
#define DEFAULT_TIME_HOUR	20
#define DEFAULT_TIME_MINUTE	37
#define DEFAULT_TIME_SECOND	00
/*******************************************************************
*    Struct Define Section
*******************************************************************/
//ʱ��ṹ��
typedef struct _t_sct{
u8 year;	//��	0-99
u8 month;	//��
u8 day;		//��
u8 hour;	//ʱ��
u8 minute;	//��
u8 sec;		//��
u8 date;	//����
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
