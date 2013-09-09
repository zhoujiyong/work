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
#ifndef __ZIGBEE_H__
#define __ZIGBEE_H__
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
#define ZIGBEE_USART USART3
#define ZIGBEE_BUF_SIZE 100
#define ZIGBEE_VALID_DAT_SIZE 10

#define ZIGBEE_USART_NUM 1
/*******************************************************************
*    Struct Define Section
*******************************************************************/
typedef struct zigbee_buf{
	u16 idx;
	u16 len;
	u8 buf[ZIGBEE_BUF_SIZE];

	struct {
		u8 busy	: 1;
		u8 RecOk: 1;
	}flag;
}zigbee_buf_t;

typedef struct valid_pack{
	u8 addr;
	u8 cmd;
	u8 ack;
	u8 len;
	u8 dat[ZIGBEE_VALID_DAT_SIZE];
}valid_pack_t;
/*******************************************************************
*    Prototype Declare Section
*******************************************************************/
extern zigbee_buf_t *pzigbeeTxBuf;

void zigbee_init(void);
void zigbee_rx_irq(void);
void zigbee_tx_irq(void);
int zigbee_send_data(u8 *buf, u8 len);
void ZigbeeRxTask(void *parg);

#endif
