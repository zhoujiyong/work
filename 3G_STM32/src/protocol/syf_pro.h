#ifndef __SYF_PRO__H__
#define __SYF_PRO__H__

#include "..\\include.h"

#define SYF_PRO_SOI_H 0xff
#define SYF_PRO_SOI_L 0x55
#define SYF_PRO_EOI_H 0xff
#define SYF_PRO_EOI_L 0xaa

#define SYF_PRO_CMD_OFT 2
#define SYF_PRO_ACK_OFT 4
#define SYF_PRO_LEN_OFT 6
#define SYF_PRO_DAT_OFT 8

#define SYF_PRO_FIX_LEN 12
#define SYF_PRO_CMD_3G_IOT_POWER_CTR 114
#define SYF_PRO_ACK_3G_IOT_POWER_CTR 117
#define SYF_PRO_CMD_GET_ZIGBEE_ADDR 115
#define SYF_PRO_ACK_GET_ZIGBEE_ADDR 116

#define SYF_PRIVATE_DATA_SIZE 100
typedef struct syf_private{
	u16 cmd;
	u16 ack;
	u16 len;
	//u16 addr;
	u8 dat[SYF_PRIVATE_DATA_SIZE];
	u16 snum;
}syf_private_t;

void syf_pro_register(void);

#endif     
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
