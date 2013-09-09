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
*    Description             :    ϵͳ����  
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
    /*  Դ��system_stm32f10x.c�ļ�,ֻ��Ҫ���ô˺���,������RCC������.�����뿴2_RCC  */
	SystemInit();

	RCC_GetClocksFreq(&RCC_ClockFreq);
	
	/* ������ÿ�ʹ�ⲿ����ͣ���ʱ��,����һ��NMI�ж�,����Ҫ�õĿ����ε�*/
	//RCC_ClockSecuritySystemCmd(ENABLE);
}

void SystemDemo_Init(void)
{
    //����ʹ��UCOS,һ�µĳ�ʼ����Ȼ������OS����֮ǰ����,��ע���ʹ���κ��ж�.
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

	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS1_8tq,CAN_BS2_7tq,5,CAN_Mode_Normal);//CAN��ʼ������ģʽ,������450Kbps
}
