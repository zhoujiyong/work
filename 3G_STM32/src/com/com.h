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
*    Multi-Include-Prevent Section
*******************************************************************/
#ifndef __COM_H__
#define __COM_H__
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
#define COM_USART USART1
#define COM_BUF_SIZE 100

#define COM_USART_NUM 2
/*******************************************************************
*    Struct Define Section
*******************************************************************/
typedef struct com_buf{
	u8 idx;
	u8 len;
	u8 buf[COM_BUF_SIZE];

	struct {
		u8 busy	: 1;
		u8 RecOk: 1;
	}flag;
}com_buf_t;
/*******************************************************************
*    Prototype Declare Section
*******************************************************************/
extern com_buf_t *pcomTxBuf;

void com_init(void);
void com_rx_irq(void);
void com_tx_irq(void);
int com_send_data(u8 *buf, u8 len);
void ComRxTask(void *parg);
#endif
