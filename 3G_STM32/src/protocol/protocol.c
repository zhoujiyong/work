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
#include "protocol.h" 
#include <string.h>

//static 
protocol_t ProtocolBuf[PROTOCOL_BUF_SIZE];
//static 
u8 Idx=0;

/*****************************************************************
*    Function Name           :   protocol_init
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
void protocol_init(void)
{
	Idx = 0;
	memset(ProtocolBuf, 0, sizeof(protocol_t));
}

/*****************************************************************
*    Function Name           :   protocol_register
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
int protocol_register(protocol_t *ppro)
{
	memcpy(&ProtocolBuf[Idx++], ppro, sizeof(protocol_t));

	return Idx;
}

/*****************************************************************
*    Function Name           :   protocol_find
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
protocol_t *protocol_find(u8 *soi, u8 len)
{
	u8 i;

	for(i=0;i<Idx;i++)
	{
		if(memcmp(ProtocolBuf[i].soi, soi, len)==0)
			return &ProtocolBuf[i];
	}
	return NULL;
}



