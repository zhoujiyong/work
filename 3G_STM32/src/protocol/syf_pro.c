/*******************************************************************
*
*    DESCRIPTION:
*
*    AUTHOR:
*
*    HISTORY:
*
*    DATE:2011-4-14
*
*******************************************************************/
#include "syf_pro.h" 

//static u16 ZigbeeAddr=0;
/*****************************************************************
*    Function Name           :   syf_pro_check_data
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
int syf_pro_check_data(u8 *dat)
{
	u16 eoi_oft;
	u16 dat_len = UcharToUint(dat[SYF_PRO_LEN_OFT], dat[SYF_PRO_LEN_OFT+1]);

	eoi_oft = dat_len + SYF_PRO_FIX_LEN - 2;
	if(dat[eoi_oft]==SYF_PRO_EOI_H && dat[eoi_oft+1]==SYF_PRO_EOI_L)
		return 0;
	return -1;
}
/*****************************************************************
*    Function Name           :   syf_pro_send_packet
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
int syf_pro_send_packet(parket_t *ppacket)
{
	syf_private_t *pprivate = (syf_private_t *)ppacket->private;
	u8 len = pprivate->len + SYF_PRO_FIX_LEN;
	u8 *buf;
	u8 idx=0;
	int ret=-1;

	buf = mymalloc(SRAMIN, len);
	if(buf!=NULL)
	{
		buf[idx++] = SYF_PRO_SOI_H;
		buf[idx++] = SYF_PRO_SOI_L;
		buf[idx++] = pprivate->cmd>>8;
		buf[idx++] = pprivate->cmd;
		buf[idx++] = pprivate->ack>>8;
		buf[idx++] = pprivate->ack;
		buf[idx++] = pprivate->len>>8;
		buf[idx++] = pprivate->len;
		memcpy(&buf[idx], pprivate->dat, pprivate->len);
		idx += pprivate->len;
		buf[idx++] = pprivate->snum>>8;
		buf[idx++] = pprivate->snum;
		buf[idx++] = SYF_PRO_EOI_H;
		buf[idx++] = SYF_PRO_EOI_L;
		if(ppacket->hw==ZIGBEE_USART_NUM)
			ret = zigbee_send_data(buf, idx);
		else
			ret = com_send_data(buf, idx);
		myfree(SRAMIN, buf);
	}
	return ret;
}
/*****************************************************************
*    Function Name           :   syf_pro_get_packet
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
int syf_pro_get_packet(parket_t *ppacket, u8 *buf)
{
	syf_private_t *pprivate;
	u16 snum_oft;
	
	if(syf_pro_check_data(buf)<0)
		return -1;	
	pprivate = mymalloc(SRAMIN, sizeof(syf_private_t));
	if(pprivate==NULL)
		return -1;
	pprivate->cmd = UcharToUint(buf[SYF_PRO_CMD_OFT], buf[SYF_PRO_CMD_OFT+1]);
	pprivate->ack = UcharToUint(buf[SYF_PRO_ACK_OFT], buf[SYF_PRO_ACK_OFT+1]);
	pprivate->len = UcharToUint(buf[SYF_PRO_LEN_OFT], buf[SYF_PRO_LEN_OFT+1]);
	snum_oft = SYF_PRO_DAT_OFT + pprivate->len;
	pprivate->snum = UcharToUint(buf[snum_oft], buf[snum_oft+1]);
	if(pprivate->len<=SYF_PRIVATE_DATA_SIZE)
	{	
		memcpy(pprivate->dat, &buf[SYF_PRO_DAT_OFT], pprivate->len);
	}
	else
	{
		myfree(SRAMIN, pprivate);
		return -1;
	}

	ppacket->p_len = pprivate->len + SYF_PRO_FIX_LEN;
	ppacket->private = pprivate;	
	return 0;
}
/*****************************************************************
*    Function Name           :   syf_pro_parse_packet
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
int syf_pro_parse_packet(parket_t *ppacket)
{
	static u8 state;
	syf_private_t *pprivate = (syf_private_t *)ppacket->private;
	u16 tmp;

	switch(pprivate->cmd)
	{
		case SYF_PRO_CMD_3G_IOT_POWER_CTR:
			if(pprivate->len)
			{
				tmp = UcharToUint(pprivate->dat[2], pprivate->dat[3]);
				if(tmp>0)
				{
					state = ON;
					OSMboxPost(PowerMbox, (void*)&state);
				}
				else
				{
					state = OFF;
					OSMboxPost(PowerMbox, (void*)&state);
				}
			}
			pprivate->cmd = SYF_PRO_ACK_3G_IOT_POWER_CTR;
			syf_pro_send_packet(ppacket);
			break;
		case SYF_PRO_CMD_GET_ZIGBEE_ADDR:
			ppacket->hw = ZIGBEE_USART_NUM;
			syf_pro_send_packet(ppacket);
			break;
		case SYF_PRO_ACK_GET_ZIGBEE_ADDR:
			ppacket->hw = COM_USART_NUM;
			syf_pro_send_packet(ppacket);
			break;
		default :break;
	}
	myfree(SRAMIN, pprivate);
	return 0;	
}
/*****************************************************************
*    Function Name           :   syf_pro_init
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
void syf_pro_register(void)
{
	protocol_t syf_pro;
	
	syf_pro.soi[0] = SYF_PRO_SOI_H;
	syf_pro.soi[1] = SYF_PRO_SOI_L;	
	syf_pro.eoi[0] = SYF_PRO_EOI_H;
	syf_pro.eoi[1] = SYF_PRO_EOI_L;
	syf_pro.get_packet = syf_pro_get_packet;
	syf_pro.send_packet = syf_pro_send_packet;
	syf_pro.parse_packet = syf_pro_parse_packet;
	syf_pro.get_packet_len = NULL;
	protocol_register(&syf_pro);
}

