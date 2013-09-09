/*******************************************************************
*
*    FILENAME(�ļ���)     :    Bsp.c
*
*    COPYRIGHT(����Ȩ)    :    ������Ѷ��ͨ�ż������޹�˾
*
*    MODULE NAME(�����)  :
*
*    CPU                  :    stm32F101CB
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
*    Debug switch Section
*    ���Կ���
*******************************************************************/

/*******************************************************************
*    Include File Section
*    ����ͷ�ļ�
*******************************************************************/
#include "bsp.h"
/*******************************************************************
*    Macro Define Section
*    �궨��
*******************************************************************/
#define BSP_TIMEOUT 5000
/*******************************************************************
*    Struct Define Section
*    �ṹ�嶨��
*******************************************************************/

/*******************************************************************
*    Global variables defined
*    ȫ�ֱ�������
*******************************************************************/
static timer_t bsp_timer;
static u8 ZigPowerOnFlag=0;
static u8 BoardPwerOnFlag=0;
/*******************************************************************
*    External Varible Declare
*    �ⲿ��������
*******************************************************************/

/*******************************************************************
*    Function Define Section
*    �������岿��
*******************************************************************/

/*****************************************************************
*    Function Name(������)   :   BspInit()
*
*    Parameters(����)        :   
*
*    Description(����)       :
*
*    Returns(����ֵ)         :
*
*    Global Variable         
*    ȫ�ֱ���                :
*    File Static Variable    
*    ���ļ���̬����          :
*    Function Static Variable
*    ������̬����            :
*----------------------------------------------------------------
*    Revision History(�޸���ʷ��¼)
*    No.    Date        Revised by        Item        Description
*
*****************************************************************/
void BspInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* clock enable  */
	RCC_APB2PeriphClockCmd(DCDC3V3_CLOCK, ENABLE);
	RCC_APB2PeriphClockCmd(DCDC5V_CLOCK, ENABLE);
	RCC_APB2PeriphClockCmd(ONKEY_CLOCK, ENABLE);
	RCC_APB2PeriphClockCmd(WKUP_ZIGBEE_CLOCK, ENABLE);
	RCC_APB2PeriphClockCmd(SAM_S0_CLOCK, ENABLE);
	RCC_APB2PeriphClockCmd(ZIGBEE_3V3_CLOCK, ENABLE);
	RCC_APB2PeriphClockCmd(ZIGBEE_RST_CLOCK, ENABLE);
	RCC_APB2PeriphClockCmd(CANS_CLOCK, ENABLE);

	DCDC5V_OFF();
	DCDC3V3_OFF();

	GPIO_InitStructure.GPIO_Pin = DCDC3V3_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/*  �������  */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/*  50Mʱ���ٶ�  */
	GPIO_Init(DCDC3V3_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = DCDC5V_PIN;
	GPIO_Init(DCDC5V_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = ZIGBEE_3V3_PIN;
	GPIO_Init(ZIGBEE_3V3_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = ZIGBEE_RST_PIN;
	GPIO_Init(ZIGBEE_RST_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SAM_S0_PIN|SAM_S1_PIN;
	GPIO_Init(SAM_S0_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = CANS_PIN;
	GPIO_Init(CANS_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = ONKEY_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;	/*  ��©���  */
	GPIO_Init(ONKEY_PORT, &GPIO_InitStructure);
	ONKEY_SET();

	/* ��zigbee�ϵ� */
	ZigbeePowerOn();

	/* ʹ��zigbee */
	SAM_S1_SET();
	SAM_S0_CLR();
	ZIGBEE_RST_SET();

	/* ʹ��CAN */
	CANS_CLR();

	timer_set(&bsp_timer, BSP_TIMEOUT);
}
/*****************************************************************
*    Function Name(������)   :   BoardPowerOn()
*
*    Parameters(����)        :   
*
*    Description(����)       :
*
*    Returns(����ֵ)         :
*
*    Global Variable         
*    ȫ�ֱ���                :
*    File Static Variable    
*    ���ļ���̬����          :
*    Function Static Variable
*    ������̬����            :
*----------------------------------------------------------------
*    Revision History(�޸���ʷ��¼)
*    No.    Date        Revised by        Item        Description
*
*****************************************************************/
void BoardPowerOn(void)
{
	CPU_SR         cpu_sr;

	if(BoardPwerOnFlag==0)
	{
		DCDC5V_ON();
		OSTimeDly(300);
		DCDC3V3_ON();
		OSTimeDly(1000);

		ONKEY_SET();
		OSTimeDly(50);
		ONKEY_CLR();
		OSTimeDly(100);
		ONKEY_SET();
	
		OS_ENTER_CRITICAL();
		BoardPwerOnFlag = 1;
		OS_EXIT_CRITICAL();
	}
	else
	{
		ONKEY_CLR();
		OSTimeDly(200);
		ONKEY_SET();
	}
}
/*****************************************************************
*    Function Name(������)   :   BoardPowerOff()
*
*    Parameters(����)        :   
*
*    Description(����)       :
*
*    Returns(����ֵ)         :
*
*    Global Variable         
*    ȫ�ֱ���                :
*    File Static Variable    
*    ���ļ���̬����          :
*    Function Static Variable
*    ������̬����            :
*----------------------------------------------------------------
*    Revision History(�޸���ʷ��¼)
*    No.    Date        Revised by        Item        Description
*
*****************************************************************/
void BoardPowerOff(void)
{
	CPU_SR         cpu_sr;

	DCDC5V_OFF();
	DCDC3V3_OFF();
	OSTimeDly(10);

	OS_ENTER_CRITICAL();
	BoardPwerOnFlag = 0;
	OS_EXIT_CRITICAL();
}
/*****************************************************************
*    Function Name(������)   :   ZigbeePowerOn()
*
*    Parameters(����)        :   
*
*    Description(����)       :
*
*    Returns(����ֵ)         :
*
*    Global Variable         
*    ȫ�ֱ���                :
*    File Static Variable    
*    ���ļ���̬����          :
*    Function Static Variable
*    ������̬����            :
*----------------------------------------------------------------
*    Revision History(�޸���ʷ��¼)
*    No.    Date        Revised by        Item        Description
*
*****************************************************************/
void ZigbeePowerOn(void)
{
	CPU_SR         cpu_sr;

	ZIGBEE_3V3_ON();
	OS_ENTER_CRITICAL();
	ZigPowerOnFlag = 1;
	OS_EXIT_CRITICAL();
}
/*****************************************************************
*    Function Name(������)   :   ZigbeePowerOff()
*
*    Parameters(����)        :   
*
*    Description(����)       :
*
*    Returns(����ֵ)         :
*
*    Global Variable         
*    ȫ�ֱ���                :
*    File Static Variable    
*    ���ļ���̬����          :
*    Function Static Variable
*    ������̬����            :
*----------------------------------------------------------------
*    Revision History(�޸���ʷ��¼)
*    No.    Date        Revised by        Item        Description
*
*****************************************************************/
void ZigbeePowerOff(void)
{
	CPU_SR         cpu_sr;

	ZIGBEE_3V3_OFF();
	OS_ENTER_CRITICAL();
	ZigPowerOnFlag = 0;
	OS_EXIT_CRITICAL();
	timer_restart(&bsp_timer);
}
/*****************************************************************
*    Function Name(������)   :   PowerTask()
*
*    Parameters(����)        :   
*
*    Description(����)       :
*
*    Returns(����ֵ)         :
*
*    Global Variable         
*    ȫ�ֱ���                :
*    File Static Variable    
*    ���ļ���̬����          :
*    Function Static Variable
*    ������̬����            :
*----------------------------------------------------------------
*    Revision History(�޸���ʷ��¼)
*    No.    Date        Revised by        Item        Description
*
*****************************************************************/
void PowerTask(void *parg)
{
	u8 err;
	u8 *msg;

	parg = parg;
	for(;;)
	{
		msg = (u8 *)OSMboxPend(PowerMbox, 4, &err);
		if(err != OS_TIMEOUT)
		{
			if(msg!=NULL)
			{
				if(*msg==ON)
				{
					BoardPowerOn();
				}
				else if(*msg==OFF)
				{
					BoardPowerOff();
				}
			}
		}
		else
		{
			if(DebounceKey()==KEY_WKUP_VAL)
			{
				BoardPowerOn();
			}
			if(ZigPowerOnFlag==0 && timer_expired(&bsp_timer))
			{
				timer_reset(&bsp_timer);
				ZigbeePowerOn();
			}
		}
	}	
}

