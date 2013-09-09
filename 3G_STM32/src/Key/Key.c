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
#include "Key.h"
#include "..\\Leds\\Leds.h"
/*******************************************************************
*    Macro Define Section 
*******************************************************************/

/*******************************************************************
*    Struct Define Section
*******************************************************************/

/*******************************************************************
*    Prototype Declare Section
*******************************************************************/
extern void RCC_Configuration(void);
extern void NVIC_Configuration(void);
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
*    Function Name           :  KeyInit  
*
*    Parameters              :   
*
*    Description             :     
*
*    Returns                 :     
*
*    Global Variable         :
*    File Static Variable    :
*    Function Static Variable:
*----------------------------------------------------------------
*    Revision History
*    No.    Date        Revised by        Item        Description
*
*****************************************************************/
void KeyInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;				   //����ṹ��
	//ʹ�õ�ʱ��ʹ��
	RCC_APB2PeriphClockCmd(KEY_WKUP_CLOCK, ENABLE);

	//WKUP
	GPIO_InitStructure.GPIO_Pin = KEY_WKUP_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	/*  ��������  */
	GPIO_Init(KEY_WKUP_PORT, &GPIO_InitStructure);
}

/*****************************************************************
*    Function Name           :  PollingKey  
*
*    Parameters              :   
*
* 	 Description             :  �а��������������ѭ����ʹ�ú���
* 								�����޷����У�������Ч�Ľ������
* 								��������оƬ����оƬ������JTAG��
* 								ʧЧ
*    Returns                 :     
*
*    Global Variable         :
*    File Static Variable    :
*    Function Static Variable:
*----------------------------------------------------------------
*    Revision History
*    No.    Date        Revised by        Item        Description
*
*****************************************************************/
u8 PollingKey(void)	//δȥ��
{
	/*  Independent key scan  */
	if(KEY_WKUP_IN())
	{
	   return 1;
	}

	return 0;
}
/*****************************************************************
*    Function Name(������)   :   DebounceKey()
*
*    Parameters(����)        :   
*
*    Description(����)       :	 ����ȥ����
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
u8 DebounceKey(void)
{
	u8 key_temp_val;
	static u8 old_key_keep;

	key_temp_val =  PollingKey();	//��ȡɨ���ֵ
	
	if(key_temp_val>0)
	{
		if(old_key_keep == 0)
		{
			old_key_keep = 1;
			return key_temp_val;
		}
		else 
			return 0; 
	}
	else 
	{
		old_key_keep = 0;
		return 0;
	}
}
/*****************************************************************
*    Function Name(������)   :   KeyTask()
*
*    Parameters(����)        :   
*
*    Description(����)       :	 ��������
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
void KeyTask(void *p_arg)
{
	u8 key_tmp;	
	for(;;)
	{	 
        OSTimeDlyHMSM(0,0,0,10);
		key_tmp = DebounceKey();
        if(key_tmp==0)continue;
        else
		{
			 //OSMboxPostOpt(KeyMbox,(void*)&key_tmp,OS_POST_OPT_BROADCAST);
		}           
	}
}


