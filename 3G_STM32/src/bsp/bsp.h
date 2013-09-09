/*******************************************************************
*
*    FILENAME(�ļ���)     :    BSP.H
*
*    COPYRIGHT(����Ȩ)    :    ������Ѷ��ͨ�ż������޹�˾
*
*    MODULE NAME(�����)  :
*
*    CPU                  :    stm32F101CBT6
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
#ifndef __DSP_H__
#define __DSP_H__
/*******************************************************************
*    Debug switch Section
*    ���Կ���
*******************************************************************/

/*******************************************************************
*    Include File Section
*    ����ͷ�ļ�
*******************************************************************/
#include "..\\include.h"
/*******************************************************************
*    Macro Define Section
*    �궨�� 
*******************************************************************/
#define USC_CLOCK  RCC_APB2Periph_GPIOC
#define USC_PORT   GPIOC
#define USC_PIN    GPIO_Pin_14

#define ONKEY_CLOCK  RCC_APB2Periph_GPIOB
#define ONKEY_PORT   GPIOB
#define ONKEY_PIN    GPIO_Pin_14

#define DCDC5V_CLOCK  RCC_APB2Periph_GPIOB
#define DCDC5V_PORT   GPIOB
#define DCDC5V_PIN    GPIO_Pin_15

#define DCDC3V3_CLOCK  RCC_APB2Periph_GPIOA
#define DCDC3V3_PORT   GPIOA
#define DCDC3V3_PIN    GPIO_Pin_8

#define ZIGBEE_3V3_CLOCK  RCC_APB2Periph_GPIOC
#define ZIGBEE_3V3_PORT   GPIOC
#define ZIGBEE_3V3_PIN    GPIO_Pin_15

#define ZIGBEE_RST_CLOCK  RCC_APB2Periph_GPIOB
#define ZIGBEE_RST_PORT   GPIOB
#define ZIGBEE_RST_PIN    GPIO_Pin_2

#define WKUP_ZIGBEE_CLOCK  RCC_APB2Periph_GPIOB
#define WKUP_ZIGBEE_PORT   GPIOB
#define WKUP_ZIGBEE_PIN    GPIO_Pin_13

#define SAM_S0_CLOCK  RCC_APB2Periph_GPIOB
#define SAM_S0_PORT   GPIOB
#define SAM_S0_PIN    GPIO_Pin_0

#define SAM_S1_CLOCK  RCC_APB2Periph_GPIOB
#define SAM_S1_PORT   GPIOB
#define SAM_S1_PIN    GPIO_Pin_1

#define CANS_CLOCK  RCC_APB2Periph_GPIOC
#define CANS_PORT   GPIOC
#define CANS_PIN    GPIO_Pin_13

#define USC_OFF()	(USC_PORT->BRR = USC_PIN)
#define USC_ON()	(USC_PORT->BSRR = USC_PIN)

#define DCDC3V3_OFF()	(DCDC3V3_PORT->BRR = DCDC3V3_PIN)
#define DCDC3V3_ON()	(DCDC3V3_PORT->BSRR = DCDC3V3_PIN)

#define DCDC5V_OFF()	(DCDC5V_PORT->BRR = DCDC5V_PIN)
#define DCDC5V_ON()	    (DCDC5V_PORT->BSRR = DCDC5V_PIN)

#define ZIGBEE_3V3_OFF()	(ZIGBEE_3V3_PORT->BRR = ZIGBEE_3V3_PIN)
#define ZIGBEE_3V3_ON()	    (ZIGBEE_3V3_PORT->BSRR = ZIGBEE_3V3_PIN)

#define WKUP_ZIGBEE_OFF()	(WKUP_ZIGBEE_PORT->BRR = WKUP_ZIGBEE_PIN)
#define WKUP_ZIGBEE_ON()	(WKUP_ZIGBEE_PORT->BSRR = WKUP_ZIGBEE_PIN)

#define ONKEY_CLR()	    (ONKEY_PORT->BRR = ONKEY_PIN)
#define ONKEY_SET()	    (ONKEY_PORT->BSRR = ONKEY_PIN)

#define SAM_S0_CLR()	    (SAM_S0_PORT->BRR = SAM_S0_PIN)
#define SAM_S0_SET()	    (SAM_S0_PORT->BSRR = SAM_S0_PIN)

#define SAM_S1_CLR()	    (SAM_S1_PORT->BRR = SAM_S1_PIN)
#define SAM_S1_SET()	    (SAM_S1_PORT->BSRR = SAM_S1_PIN)

#define ZIGBEE_RST_CLR()	    (ZIGBEE_RST_PORT->BRR = ZIGBEE_RST_PIN)
#define ZIGBEE_RST_SET()	    (ZIGBEE_RST_PORT->BSRR = ZIGBEE_RST_PIN)

#define CANS_CLR()	    (CANS_PORT->BRR = CANS_PIN)
#define CANS_SET()	    (CANS_PORT->BSRR = CANS_PIN)
/*******************************************************************
*    Struct Define Section
*    �ṹ�嶨��
*******************************************************************/
enum {OFF=1, ON};
/*******************************************************************
*    Global Varible Declare
*    ȫ�ֱ�������
*******************************************************************/

/*******************************************************************
*    Prototype Declare Section
*    �ڲ�����ԭ������
*******************************************************************/
void BspInit(void);
void PowerTask(void *parg);
void ZigbeePowerOn(void);
void ZigbeePowerOff(void);

#endif




