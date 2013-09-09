/*******************************************************************
*
*    FILENAME             :
*
*    COPYRIGHT            :    深圳市讯方通信技术有限公司
*
*    MODULE NAME          :
*
*    CPU                  :    stm32F103RBT6
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
#include "publicFuc.h"
#include "string.h"
/*******************************************************************
*    Macro Define Section 
*******************************************************************/


/*******************************************************************
*    Struct Define Section
*******************************************************************/


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
*    Function Name           :   ChNumtoStr
*
*    Parameters              :
*
*    Description             :   此函数仅仅处理3位一下的数字
*                                把数字转化为字符输出
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
void ChNumtoStr(char *numStr,uchar num)
{
    *(numStr+2) =  num%10+0x30;
    *(numStr+1) = (num/10)%10+0x30;
    *(numStr+0) = (num/100)%10+0x30;
}

u16 ChStrtoNum(char *numStr)
{
    u16 num = 0;

    num += (*(numStr+0)-0x30)*100;
    num += (*(numStr+1)-0x30)*10;
    num += (*(numStr+2)-0x30)*1;

    return num;
}
/*****************************************************************
*    Function Name           :   CharToAscii
*
*    Parameters              :
*
*    Description             :   将数据转成ASCII
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
u8 CharToAscii(u8 ch)
{
	u8 asc;

	if(ch<10)
		asc = ch + '0';
	else
		asc = (ch - 10) + 'a';
	
	return asc;	
}
/*****************************************************************
*    Function Name           :   AsciiToChar
*
*    Parameters              :
*
*    Description             :   将ASCII转成HEX数据
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
u8 AsciiToChar(u8 asc)
{
	u8 ch;

	if(asc>='a'&&asc<='z')
		ch = asc  - 'a' + 10;
	else if(asc>='A'&&asc<='Z')
		ch = asc  - 'A' + 10;
	else
		ch = asc - '0';
	
	return ch;	
}
/*****************************************************************
*    Function Name           :   UcharToUint
*
*    Parameters              :
*
*    Description             :   将两个uchar转成一个uint数据
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
uint UcharToUint(uchar ah,uchar al)
{	
    u16	i_data;

    i_data = (u16)ah;
	i_data <<= 8;		/*  移位时后几位补零  */
	i_data = i_data|(u16)al; /*  得到uint值  */

    return(i_data);
}
/*****************************************************************
*    Function Name           :   StringAddChar
*
*    Parameters              :
*
*    Description             :   这个函数在字符串后面加字符,第一
*                                字符会被覆盖掉  
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
void StringAddChar(u8 ch, char*str)
{
	u16 i=0;
	/*  字符串长度,不包括结束服 0*/
    u8 strLength=0;

	strLength = strlen(str);
	for (i=0;i<(strLength-1);i++)
	{
		*(str+i) = *(str+i+1);
	}
	*(str+strLength-1) = ch;
}
/*****************************************************************
*    Function Name           :   StringShiftRightOne
*
*    Parameters              :
*
*    Description             :   字符串向右移,左边补零
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
void StringShiftRightOne(char *str)
{
	u8 i=0;
    /*  字符串长度,不包括结束服 0*/
    u8 strLength=0;

    strLength = strlen(str);
	for (i=strLength-1;i>0;i--)
	{
        *(str+i) = *(str+i-1);
	}
	*(str+0) = '0';
}

//延时72M
void DemoDly_ms_72M(uint ms)
{
    uint i,j;

    for (i=0;i<ms;i++)
    {
        for (j=0;j<30000;j++) 
        {
            ;
        }
    }
}

void DemoDly_ms_8M(uint ms)
{
    uint i,j;

    for (i=0;i<ms;i++)
    {
        for (j=0;j<3000;j++) 
        {
            ;
        }
    }
}


void TIM2_Delay_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	/* TIM2基础设置*/
	TIM_TimeBaseStructure.TIM_Period = 0;			//计数值   
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1;    	//预分频,此值+1为分频的除数
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;  	//
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//向上计数
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	/*使能预装载*/
	//TIM_ARRPreloadConfig(TIM2, ENABLE);
	/*预先清除所有中断位*/
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 
	/* 溢出配置中断*/
	TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
    /* 允许TIM3开始计数 */
	TIM_Cmd(TIM2, DISABLE);
}

//extern u8 Tim2_Flag;
//void delay_us(u16 cnt)
//{
//    /* Set the Autoreload value */
//  	TIM2->ARR = cnt ;
//	/* 溢出配置中断*/
//	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
//    /* 允许TIM2开始计数 */
//	TIM_Cmd(TIM2, ENABLE);
//
//	while(Tim2_Flag==0);
//	Tim2_Flag=0;
//}

u8 cpu_id[12];
/*****************************************************************
*    Function Name(函数名)   :   Get_ID()
*
*    Parameters(参数)        :   
*
*    Description(描述)       :   根据STM32内部全球唯一ID生成可显示的8位字符编码
*
*    Returns(返回值)         :
*
*    Global Variable         
*    全局变量                :
*    File Static Variable    
*    本文件静态变量          :
*    Function Static Variable
*    函数静态变量            :
*----------------------------------------------------------------
*    Revision History(修改历史记录)
*    No.    Date        Revised by        Item        Description
*
*****************************************************************/
void Get_ID(void)
{
    u32 CpuID[3];
    u8  i;
//------------------------------------------------------------
      //获取CPU唯一ID
      CpuID[0]   = *(vu32*)(0x1ffff7e8);
      CpuID[1]   = *(vu32*)(0x1ffff7ec);
      CpuID[2]   = *(vu32*)(0x1ffff7f0);

      cpu_id[0]  = (u8)(CpuID[0] & 0x000000ff);
      cpu_id[1]  = CpuID[0]>>8 & 0x000000ff;
      cpu_id[2]  = (u8)(CpuID[0]>>16 & 0x000000ff);
      cpu_id[3]  = CpuID[0]>>24 & 0x000000ff;

      cpu_id[4]  = CpuID[1] & 0x000000ff;
      cpu_id[5]  = CpuID[1]>>8 & 0x000000ff;
      cpu_id[6]  = CpuID[1]>>16 & 0x000000ff;
      cpu_id[7]  = CpuID[1]>>24 & 0x000000ff;

      cpu_id[8]  = CpuID[2] & 0x000000ff;
      cpu_id[9]  = CpuID[2]>>8 & 0x000000ff;
      cpu_id[10] = CpuID[2]>>16 & 0x000000ff;
      cpu_id[11] = CpuID[2]>>24 & 0x000000ff;
//------------------------------------------------------------
//十六进制的数转变成有效的字符      
	 for(i=0;i<12;i++)
	 	{
		 while(!((cpu_id[i]<58&&cpu_id[i]>47)||(cpu_id[i]<123&&cpu_id[i]>97)))//变到0~9 或者 a~z  
		 cpu_id[i] += 25;
		}
		//u44432u2sq16	 u9r904x37715
//	   cpu_id[11] = 'u';
//	   cpu_id[10] = '9';
//	   cpu_id[9] = 'r';
//	   cpu_id[8] = '9';
//	   cpu_id[7] = '0';
//	   cpu_id[6] = '4';
//	   cpu_id[5] = 'x';
//	   cpu_id[4] = '3';
//	   cpu_id[3] = '7';
//	   cpu_id[2] = '7';
//	   cpu_id[1] = '1';
//	   cpu_id[0] = '5';

//	   cpu_id[11] = '1';
//	   cpu_id[10] = '2';
//	   cpu_id[9] = '3';
//	   cpu_id[8] = '4';
//	   cpu_id[7] = '5';
//	   cpu_id[6] = '6';
//	   cpu_id[5] = '7';
//	   cpu_id[4] = '8';
//	   cpu_id[3] = '9';
//	   cpu_id[2] = 'a';
//	   cpu_id[1] = 'b';
//	   cpu_id[0] = 'c';

	   cpu_id[0] = '1';
	   cpu_id[1] = '2';
	   cpu_id[2] = '3';
	   cpu_id[3] = '4';
	   cpu_id[4] = '5';
	   cpu_id[5] = '6';
	   cpu_id[6] = '7';
	   cpu_id[7] = '8';
	   cpu_id[8] = '9';
	   cpu_id[9] = 'a';
	   cpu_id[10] = 'b';
	   cpu_id[11] = 'c';

//	   cpu_id[11] = '1';
//	   cpu_id[10] = '1';
//	   cpu_id[9] = '1';
//	   cpu_id[8] = '1';
//	   cpu_id[7] = '1';
//	   cpu_id[6] = '1';
//	   cpu_id[5] = '1';
//	   cpu_id[4] = '1';
//	   cpu_id[3] = '1';
//	   cpu_id[2] = '1';
//	   cpu_id[1] = '1';
//	   cpu_id[0] = '1';

//	   cpu_id[11] = 1;
//	   cpu_id[10] = 2;
//	   cpu_id[9] = 3;
//	   cpu_id[8] = 4;
//	   cpu_id[7] = 5;
//	   cpu_id[6] = 6;
//	   cpu_id[5] = 7;
//	   cpu_id[4] = 8;
//	   cpu_id[3] = 9;
//	   cpu_id[2] = 10;
//	   cpu_id[1] = 11;
//	   cpu_id[0] = 12;
//------------------------------------------------------------
//串口打印--调试用
	 for(i=0;i<12;i++)
	 	{
		 IOT_DEBUGF(("%c",cpu_id[i]));
		}
	 IOT_DEBUGF(("\r\n"));
//------------------------------------------------------------
//加上此句用于调试--可让所有的物联网终端ID为0	 	 
	 //for(i=0;i<12;i++)cpu_id[i]=0;	  //调试用的

}

/*****************************************************************
*    Function Name           :   fputc
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
int fputc(int ch, FILE *f)
{
  /* Write a character to the USART */
  USART_SendData(USART3, (u8) ch);

  /* Loop until the end of transmission */
  while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
  {
  }

  return ch;
}

