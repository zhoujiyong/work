/*******************************************************************
*
*    FILENAME             :
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
*    Debug switch Section
*******************************************************************/

/*******************************************************************
*    Include File Section
*******************************************************************/
#include "include.h"
/*******************************************************************
*    Macro Define Section 
*******************************************************************/


/*******************************************************************
*    Struct Define Section
*******************************************************************/


/*******************************************************************
*    Prototype Declare Section
*******************************************************************/

/*******************************************************************
*    Global Varible Declare
*******************************************************************/

/*******************************************************************
*    File Static Variable Define Section
*******************************************************************/

/*******************************************************************
*    Function Define Section
*******************************************************************/

/*****************************************************************
*    Function Name           :    RCC_Configuration
*
*    Parameters              :    void
*
*    Description             :    系统配置  
*
*    Returns                 :    none 
*
*    Global Variable         :
*    File Static Variable    :
*    Function Static Variable:
*----------------------------------------------------------------
*    Revision History
*    No.    Date        Revised by        Item        Description
*
*****************************************************************/
void RCC_Configuration(void)
{   
    RCC_ClocksTypeDef RCC_ClockFreq;
    /*  源自system_stm32f10x.c文件,只需要调用此函数,则可完成RCC的配置.具体请看2_RCC  */
	SystemInit();

	RCC_GetClocksFreq(&RCC_ClockFreq);
	
	/* 这个配置可使外部晶振停振的时候,产生一个NMI中断,不需要用的可屏蔽掉*/
	//RCC_ClockSecuritySystemCmd(ENABLE);
}

void SystemDemo_Init(void)
{
    //由于使用UCOS,一下的初始化虽然可以在OS运行之前运行,但注意别使能任何中断.
    RCC_Configuration();

	/*  Leds port Init  */
    Leds_Init();
	//LED1_ON();

	BspInit();

	zigbee_init();

	com_init();

	mem_init(SRAMIN);

	protocol_init();
	zjy_pro_register();
	syf_pro_register();

	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS1_8tq,CAN_BS2_7tq,5,CAN_Mode_Normal);//CAN初始化环回模式,波特率450Kbps
}
