/*******************************************************************
*
*    FILENAME(文件名)     :    Bsp.c
*
*    COPYRIGHT(著作权)    :    深圳市讯方通信技术有限公司
*
*    MODULE NAME(组件名)  :
*
*    CPU                  :    stm32F101CB
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
*    Debug switch Section
*    调试开关
*******************************************************************/

/*******************************************************************
*    Include File Section
*    包含头文件
*******************************************************************/
#include "bsp.h"
/*******************************************************************
*    Macro Define Section
*    宏定义
*******************************************************************/
#define BSP_TIMEOUT 5000
/*******************************************************************
*    Struct Define Section
*    结构体定义
*******************************************************************/

/*******************************************************************
*    Global variables defined
*    全局变量定义
*******************************************************************/
static timer_t bsp_timer;
static u8 ZigPowerOnFlag=0;
static u8 BoardPwerOnFlag=0;
/*******************************************************************
*    External Varible Declare
*    外部变量声明
*******************************************************************/

/*******************************************************************
*    Function Define Section
*    函数定义部分
*******************************************************************/

/*****************************************************************
*    Function Name(函数名)   :   BspInit()
*
*    Parameters(参数)        :   
*
*    Description(描述)       :
*
*    Returns(返回值)         :
*
*    Global Variable         
*    全局变量                :
*    File Static Variable    
*    本文件静态变量          :
*    Function Static Variable
*    函数静态变量            :
*----------------------------------------------------------------
*    Revision History(修改历史记录)
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/*  推挽输出  */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/*  50M时钟速度  */
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;	/*  开漏输出  */
	GPIO_Init(ONKEY_PORT, &GPIO_InitStructure);
	ONKEY_SET();

	/* 给zigbee上电 */
	ZigbeePowerOn();

	/* 使能zigbee */
	SAM_S1_SET();
	SAM_S0_CLR();
	ZIGBEE_RST_SET();

	/* 使能CAN */
	CANS_CLR();

	timer_set(&bsp_timer, BSP_TIMEOUT);
}
/*****************************************************************
*    Function Name(函数名)   :   BoardPowerOn()
*
*    Parameters(参数)        :   
*
*    Description(描述)       :
*
*    Returns(返回值)         :
*
*    Global Variable         
*    全局变量                :
*    File Static Variable    
*    本文件静态变量          :
*    Function Static Variable
*    函数静态变量            :
*----------------------------------------------------------------
*    Revision History(修改历史记录)
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
*    Function Name(函数名)   :   BoardPowerOff()
*
*    Parameters(参数)        :   
*
*    Description(描述)       :
*
*    Returns(返回值)         :
*
*    Global Variable         
*    全局变量                :
*    File Static Variable    
*    本文件静态变量          :
*    Function Static Variable
*    函数静态变量            :
*----------------------------------------------------------------
*    Revision History(修改历史记录)
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
*    Function Name(函数名)   :   ZigbeePowerOn()
*
*    Parameters(参数)        :   
*
*    Description(描述)       :
*
*    Returns(返回值)         :
*
*    Global Variable         
*    全局变量                :
*    File Static Variable    
*    本文件静态变量          :
*    Function Static Variable
*    函数静态变量            :
*----------------------------------------------------------------
*    Revision History(修改历史记录)
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
*    Function Name(函数名)   :   ZigbeePowerOff()
*
*    Parameters(参数)        :   
*
*    Description(描述)       :
*
*    Returns(返回值)         :
*
*    Global Variable         
*    全局变量                :
*    File Static Variable    
*    本文件静态变量          :
*    Function Static Variable
*    函数静态变量            :
*----------------------------------------------------------------
*    Revision History(修改历史记录)
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
*    Function Name(函数名)   :   PowerTask()
*
*    Parameters(参数)        :   
*
*    Description(描述)       :
*
*    Returns(返回值)         :
*
*    Global Variable         
*    全局变量                :
*    File Static Variable    
*    本文件静态变量          :
*    Function Static Variable
*    函数静态变量            :
*----------------------------------------------------------------
*    Revision History(修改历史记录)
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

