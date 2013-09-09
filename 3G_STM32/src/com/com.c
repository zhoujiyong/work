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
#include "com.h"

/*******************************************************************
*    Macro Define Section 
*******************************************************************/
#define COM_DEBUG  1

#define COM_RX_TIMEOUT 5
/*******************************************************************
*    Struct Define Section
*******************************************************************/
com_buf_t comRxBuf;
com_buf_t *pcomRxBuf=&comRxBuf;
com_buf_t comTxBuf;
com_buf_t *pcomTxBuf=&comTxBuf;

static timer_t com_rx_timer;
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
*    Function Name           :  com_init 
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
void com_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    USART_ClockInitTypeDef USART_ClockInitStructure;

    /*  使能串口1，PA，AFIO总线  */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | 
                           RCC_APB2Periph_AFIO  |
                           RCC_APB2Periph_USART1, 
                           ENABLE);

    /* A9 USART1_Tx */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; /*  推挽输出-TX  */
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* A10 USART1_Rx  */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;/*  浮空输入-RX  */
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
    USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
    USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
    USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;

    USART_ClockInit(COM_USART, &USART_ClockInitStructure);

    /*  Initialize the COM_USART  */
    USART_Init(COM_USART, &USART_InitStructure);
    /* Enable COM_USART Receive interrupt */
    USART_ITConfig(COM_USART, USART_IT_RXNE, ENABLE);
    /*  Disable COM_USART Tranmit interrupt  */
    USART_ITConfig(COM_USART, USART_IT_TXE, DISABLE);
    /* Enable the USARTx */
    USART_Cmd(COM_USART, ENABLE);

	timer_set(&com_rx_timer, COM_RX_TIMEOUT);

	memset(pcomRxBuf, 0, sizeof(com_buf_t));
	memset(pcomTxBuf, 0, sizeof(com_buf_t));
}
/*****************************************************************
*    Function Name           :  com_rx_irq 
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
void com_rx_irq(void)
{
	if(pcomRxBuf->idx<COM_BUF_SIZE)
	{
		timer_restart(&com_rx_timer);
		pcomRxBuf->buf[pcomRxBuf->idx++] = USART_ReceiveData(COM_USART);
	}
	else
		IOT_DEBUGF(("%s():ComRxBuf overflew\r\n", __func__));
}

/*****************************************************************
*    Function Name           :   com_tx_irq
*
*    Parameters              :   
*                                
*    Description             :   该函数放在串口中断中发送数据  
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
void com_tx_irq(void)
{
	if(pcomTxBuf->idx < pcomTxBuf->len)
	{
		USART_SendData(COM_USART, pcomTxBuf->buf[pcomTxBuf->idx]);
		pcomTxBuf->idx++;
	}
	else
	{
		USART_ITConfig(COM_USART, USART_IT_TXE, DISABLE);
		memset((u8 *)pcomTxBuf, 0, sizeof(com_buf_t));
	}   
}
/*****************************************************************
*    Function Name           :   com_send_data
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
int com_send_data(u8 *buf, u8 len)
{
	CPU_SR         cpu_sr;

	if(len <= COM_BUF_SIZE)
	{
		while(pcomTxBuf->flag.busy==1);
		//{
			OS_ENTER_CRITICAL();
			pcomTxBuf->flag.busy = 1;
			memcpy(pcomTxBuf->buf, buf, len);
			pcomTxBuf->len = len;
			pcomTxBuf->idx = 0;
			OS_EXIT_CRITICAL();
			USART_ITConfig(COM_USART, USART_IT_TXE, ENABLE);
			return 0;
		//}
	}
	return -1;  
}
/*****************************************************************
*    Function Name           :   ComRxTask
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
void ComRxTask(void *parg)
{
	u16 i, j, k;
	u16 l_len, p_len;
	protocol_t *ppro;
	parket_t packet;

	for(;;)
	{
		if(timer_expired(&com_rx_timer))
		{
			timer_reset(&com_rx_timer);
			if(pcomRxBuf->idx > 0)
			{
				USART_ITConfig(COM_USART, USART_IT_RXNE, DISABLE);
				pcomRxBuf->len = pcomRxBuf->idx;
				pcomRxBuf->idx = 0;
				for(i=0;i<pcomRxBuf->len;i++)
				{
					ppro = protocol_find(&pcomRxBuf->buf[i], 2);
					if(ppro!=NULL)
					{
						if(ppro->get_packet(&packet, &pcomRxBuf->buf[i])==0)
						{
							packet.hw = COM_USART_NUM;  /* 指定串口 */
							ppro->parse_packet(&packet);
							p_len = packet.p_len;
							j = i + p_len;
							l_len = pcomRxBuf->len - j;
							for(k=i;k<i+l_len;k++,j++)
							{
								pcomRxBuf->buf[k] = pcomRxBuf->buf[j];
							}
							pcomRxBuf->len -= p_len;
						}
					}
					else
					{
						ppro = protocol_find(&pcomRxBuf->buf[i], 1);
						if(ppro!=NULL)
						{
							if(ppro->get_packet(&packet, &pcomRxBuf->buf[i])==0)
							{
								packet.hw = COM_USART_NUM;  /* 指定串口 */
								ppro->parse_packet(&packet);
								p_len = packet.p_len;
								j = i + p_len;
								l_len = pcomRxBuf->len - j;
								for(k=i;k<i+l_len;k++,j++)
								{
									pcomRxBuf->buf[k] = pcomRxBuf->buf[j];
								}
								pcomRxBuf->len -= p_len;
							}	
						}	
					}
				}
				if(pcomRxBuf->len>0 && pcomRxBuf->len <= ZIGBEE_BUF_SIZE)
				{
					zigbee_send_data(pcomRxBuf->buf, pcomRxBuf->len);
				}
				memset((u8 *)pcomRxBuf, 0, sizeof(zigbee_buf_t));
				USART_ITConfig(COM_USART, USART_IT_RXNE, ENABLE);
			}
		}
		OSTimeDly(2);		
	}
}

