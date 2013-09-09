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
#include "zigbee.h"

/*******************************************************************
*    Macro Define Section 
*******************************************************************/
#define ZIGBEE_DEBUG  1

#define ZIGBEE_RX_TIMEOUT 5
/*******************************************************************
*    Struct Define Section
*******************************************************************/
zigbee_buf_t zigbeeRxBuf;
zigbee_buf_t *pzigbeeRxBuf=&zigbeeRxBuf;
zigbee_buf_t zigbeeTxBuf;
zigbee_buf_t *pzigbeeTxBuf=&zigbeeTxBuf;

static timer_t zigbee_rx_timer;
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
*    Function Name           :  zigbee_init 
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
void zigbee_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);

    /* PB10 USART3_Tx */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; /*  推挽输出-TX  */
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* PB11 USART3_Rx  */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;/*  浮空输入-RX  */
    GPIO_Init(GPIOB, &GPIO_InitStructure);

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

    USART_ClockInit(ZIGBEE_USART, &USART_ClockInitStructure);

    /*  Initialize the ZIGBEE_USART  */
    USART_Init(ZIGBEE_USART, &USART_InitStructure);
    /* Enable ZIGBEE_USART Receive interrupt */
    USART_ITConfig(ZIGBEE_USART, USART_IT_RXNE, ENABLE);
    /*  Disable ZIGBEE_USART Tranmit interrupt  */
    USART_ITConfig(ZIGBEE_USART, USART_IT_TXE, DISABLE);
    /* Enable the USARTx */
    USART_Cmd(ZIGBEE_USART, ENABLE);

	timer_set(&zigbee_rx_timer, ZIGBEE_RX_TIMEOUT);

	memset(pzigbeeRxBuf, 0, sizeof(zigbee_buf_t));
	memset(pzigbeeTxBuf, 0, sizeof(zigbee_buf_t));
}

/*****************************************************************
*    Function Name           :  zigbee_rx_irq 
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
void zigbee_rx_irq(void)
{
	if(pzigbeeRxBuf->idx<ZIGBEE_BUF_SIZE)
	{
		timer_restart(&zigbee_rx_timer);
		pzigbeeRxBuf->buf[pzigbeeRxBuf->idx++] = USART_ReceiveData(ZIGBEE_USART);
	}
	else
		IOT_DEBUGF(("%s():Buf overflew\r\n", __func__));
}

/*****************************************************************
*    Function Name           :   zigbee_tx_irq
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
void zigbee_tx_irq(void)
{
	if(pzigbeeTxBuf->idx < pzigbeeTxBuf->len)
	{
		USART_SendData(ZIGBEE_USART, pzigbeeTxBuf->buf[pzigbeeTxBuf->idx]);
		pzigbeeTxBuf->idx++;
	}
	else
	{
		USART_ITConfig(ZIGBEE_USART, USART_IT_TXE, DISABLE);
		memset((u8 *)pzigbeeTxBuf, 0, sizeof(zigbee_buf_t));
	}   
}
/*****************************************************************
*    Function Name           :   zigbee_send_data
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
int zigbee_send_data(u8 *buf, u8 len)
{
	CPU_SR         cpu_sr;

	if(len <= ZIGBEE_BUF_SIZE)
	{
		while(pzigbeeTxBuf->flag.busy==1);
		//if(pzigbeeTxBuf->flag.busy==0)
		//{
			OS_ENTER_CRITICAL();
			pzigbeeTxBuf->flag.busy = 1;
			memcpy(pzigbeeTxBuf->buf, buf, len);
			pzigbeeTxBuf->len = len;
			pzigbeeTxBuf->idx = 0;
			OS_EXIT_CRITICAL();
			USART_ITConfig(ZIGBEE_USART, USART_IT_TXE, ENABLE);
			return 0;
		//}
	}
	return -1;  
}
/*****************************************************************
*    Function Name           :   ZigbeeRxTask
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
void ZigbeeRxTask(void *parg)
{
	u16 i, j, k;
	u16 l_len, p_len;
	protocol_t *ppro;
	parket_t packet;

	for(;;)
	{
		if(timer_expired(&zigbee_rx_timer))
		{
			timer_reset(&zigbee_rx_timer);
			if(pzigbeeRxBuf->idx > 0)
			{
				USART_ITConfig(ZIGBEE_USART, USART_IT_RXNE, DISABLE);
				pzigbeeRxBuf->len = pzigbeeRxBuf->idx;
				pzigbeeRxBuf->idx = 0;
				for(i=0;i<pzigbeeRxBuf->len;i++)
				{
					ppro = protocol_find(&pzigbeeRxBuf->buf[i], 2);
					if(ppro!=NULL)
					{
						if(ppro->get_packet(&packet, &pzigbeeRxBuf->buf[i])==0)
						{
							packet.hw = ZIGBEE_USART_NUM;  /* 指定串口 */
							ppro->parse_packet(&packet);
							p_len = packet.p_len;
							j = i + p_len;
							l_len = pzigbeeRxBuf->len - j;
							for(k=i;k<i+l_len;k++,j++)
							{
								pzigbeeRxBuf->buf[k] = pzigbeeRxBuf->buf[j];
							}
							pzigbeeRxBuf->len -= p_len;
						}
					}
					else
					{
						ppro = protocol_find(&pzigbeeRxBuf->buf[i], 1);
						if(ppro!=NULL)
						{
							if(ppro->get_packet(&packet, &pzigbeeRxBuf->buf[i])==0)
							{
								packet.hw = ZIGBEE_USART_NUM;  /* 指定串口 */
								ppro->parse_packet(&packet);
								p_len = packet.p_len;
								j = i + p_len;
								l_len = pzigbeeRxBuf->len - j;
								for(k=i;k<i+l_len;k++,j++)
								{
									pzigbeeRxBuf->buf[k] = pzigbeeRxBuf->buf[j];
								}
								pzigbeeRxBuf->len -= p_len;
							}	
						}	
					}
				}
				if(pzigbeeRxBuf->len>0)
				{
					com_send_data(pzigbeeRxBuf->buf, pzigbeeRxBuf->len);
				}
				memset((u8 *)pzigbeeRxBuf, 0, sizeof(zigbee_buf_t));
				USART_ITConfig(ZIGBEE_USART, USART_IT_RXNE, ENABLE);
			}
		}
		OSTimeDly(2);		
	}	
}

