#ifndef __PROTOCOL__H__
#define __PROTOCOL__H__

#include "..\\include.h"

#define PROTOCOL_BUF_SIZE 3

typedef struct parket{
	u8 hw;
	u16 p_len;
	void *private;	
}parket_t;

typedef struct protocol{
	u8 soi[2];
	u8 eoi[2];
	int (*get_packet)(parket_t*, u8 *);
	int (*send_packet)(parket_t*);
	int (*parse_packet)(parket_t*);
	int (*get_packet_len)(parket_t*);
}protocol_t;

void protocol_init(void);
int protocol_register(protocol_t *ppro);
protocol_t *protocol_find(u8 *soi, u8 len);

#endif     
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
           
