#ifndef __ZJY_PRO__H__
#define __ZJY_PRO__H__

#include "..\\include.h"

/* soi(1) + len(1) + addr(1) + cmd(1) + ack(1) + dat(n) + chksum(1) */
#define ZJY_PRO_SOI 0xff
#define ZJY_PRO_LEN_FIX_LEN 4

#define ZJY_PRO_SOI_OFT 0
#define ZJY_PRO_LEN_OFT 1
#define ZJY_PRO_ADDR_OFT 2
#define ZJY_PRO_CMD_OFT 3
#define ZJY_PRO_ACK_OFT 4
#define ZJY_PRO_DAT_OFT 5

#define ZJY_LOCAL_CAR_ADDR 1

/**************√¸¡Ó∂Œ****************/

/* √¸¡Ó¡–±Ì */
enum {
	CMD_3G_IOT_POWER_ON=1,	
	CMD_3G_IOT_POWER_OFF,
	CMD_ZIGBEE_POWER_ON,
	CMD_ZIGBEE_POWER_OFF,
};

/* œÏ”¶¬Î */
enum {
	ACK_NULL,
  	ACK_OK,
	ACK_ERR,
	ACK_NO_CMD,
};

#define ZJY_PRIVATE_DATA_SIZE 100
typedef struct zjy_private{
	u8 cmd;
	u8 ack;
	u8 addr;
	u8 len;
	u8 dat[ZJY_PRIVATE_DATA_SIZE];
}zjy_private_t;

void zjy_pro_register(void);

#endif     
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
