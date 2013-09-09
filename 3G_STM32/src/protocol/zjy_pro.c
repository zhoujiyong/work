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
#include "zjy_pro.h" 

/*****************************************************************
*    Function Name           :   zjy_pro_check_data
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
//chksum包含头
int zjy_pro_check_data(u8 *dat)
{
	u8 pack_len;
	u8 chksum;

	if(dat[ZJY_PRO_SOI_OFT]==ZJY_PRO_SOI)
	{
		/* 此处可能存在人为隐患 */
		pack_len = dat[ZJY_PRO_LEN_OFT]+2; /* 加上头和len本身 */
		chksum = Get_Crc8(dat, pack_len-1);	 /* 除去校验本身 */
		IOT_DEBUGF(("chksum=%02x\r\n",chksum));
		if(chksum == dat[pack_len-1])
			return 0;
	}
	return -1;
}
/*****************************************************************
*    Function Name           :   zjy_pro_send_packet
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
int zjy_pro_send_packet(parket_t *ppacket)
{
	zjy_private_t *pprivate = (zjy_private_t *)ppacket->private;
	u8 len = pprivate->len + ZJY_PRO_LEN_FIX_LEN + 2;
	u8 *buf;
	u8 idx=0;
	int ret=-1;

	buf = mymalloc(SRAMIN, len);
	//buf = malloc(len);
	if(buf!=NULL)
	{
		buf[idx++] = ZJY_PRO_SOI;
		buf[idx++] = pprivate->len + ZJY_PRO_LEN_FIX_LEN;
		buf[idx++] = pprivate->addr;
		buf[idx++] = pprivate->cmd;
		buf[idx++] = pprivate->ack;
		memcpy(&buf[idx], pprivate->dat, pprivate->len);
		idx += pprivate->len;
		buf[idx++] = Get_Crc8(buf, idx);
		if(ppacket->hw==ZIGBEE_USART_NUM)
			ret = zigbee_send_data(buf, idx);
		else
			ret = com_send_data(buf, idx);
		myfree(SRAMIN, buf);
		//free(buf);
	}
	return ret;
}
/*****************************************************************
*    Function Name           :   zjy_pro_get_packet
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
int zjy_pro_get_packet(parket_t *ppacket, u8 *buf)
{
	zjy_private_t *pprivate;

	if(zjy_pro_check_data(buf)<0)
		return -1;
	pprivate = mymalloc(SRAMIN, sizeof(zjy_private_t));
	//pprivate = malloc(sizeof(syf_private_t));
	if(pprivate==NULL)
		return -1;
	pprivate->addr = buf[ZJY_PRO_ADDR_OFT];
	pprivate->cmd = buf[ZJY_PRO_CMD_OFT];
	pprivate->ack = buf[ZJY_PRO_ACK_OFT];
	pprivate->len = buf[ZJY_PRO_LEN_OFT] - ZJY_PRO_LEN_FIX_LEN;
	if(pprivate->len > ZJY_PRIVATE_DATA_SIZE)
	{
		myfree(SRAMIN, pprivate);
		//free(pprivate);
		return -1;	
	}
	else
	{
		memcpy(pprivate->dat, &buf[ZJY_PRO_DAT_OFT], pprivate->len);
	}

	ppacket->p_len = pprivate->len + ZJY_PRO_LEN_FIX_LEN + 2;	
	ppacket->private = pprivate;	
	return 0;
}
/*****************************************************************
*    Function Name           :   zjy_pro_parse_packet
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
int zjy_pro_parse_packet(parket_t *ppacket)
{
	static u8 state;
	zjy_private_t *pprivate = (zjy_private_t *)ppacket->private;

	switch(pprivate->cmd)
	{
		case CMD_3G_IOT_POWER_ON:
			state = ON;
			OSMboxPost(PowerMbox, (void*)&state);
			pprivate->ack = ACK_OK;
			zjy_pro_send_packet(ppacket);
			break;
		case CMD_3G_IOT_POWER_OFF:
			state = OFF;
			OSMboxPost(PowerMbox, (void*)&state);
			pprivate->ack = ACK_OK;
			zjy_pro_send_packet(ppacket);
			break;
		case CMD_ZIGBEE_POWER_ON:
			ZigbeePowerOn();
			pprivate->ack = ACK_OK;
			zjy_pro_send_packet(ppacket);
			break;
		case CMD_ZIGBEE_POWER_OFF:
			ZigbeePowerOff();
			pprivate->ack = ACK_OK;
			zjy_pro_send_packet(ppacket);
			break;
		default :break;
	}
	myfree(SRAMIN, pprivate);
	//free(pprivate);
	return 0;	
}
/*****************************************************************
*    Function Name           :   zjy_pro_register
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
void zjy_pro_register(void)
{
	protocol_t zjy_pro;
	
	zjy_pro.soi[0] = ZJY_PRO_SOI;
	zjy_pro.soi[1] = 0;	
	zjy_pro.eoi[0] = 0;
	zjy_pro.eoi[1] = 0;
	zjy_pro.get_packet = zjy_pro_get_packet;
	zjy_pro.send_packet = zjy_pro_send_packet;
	zjy_pro.parse_packet = zjy_pro_parse_packet;
	zjy_pro.get_packet_len = NULL;
	protocol_register(&zjy_pro);
}

