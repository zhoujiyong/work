/*******************************************************************
*
*    FILENAME             :  RTC.C
*
*    COPYRIGHT            :  深圳市讯方通信技术有限公司
*
*    MODULE NAME          :
*
*    CPU                  :  stm32F103ZET6
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
#include "RTC.h"
/*******************************************************************
*    Macro Define Section 
*******************************************************************/

/*******************************************************************
*    Struct Define Section
*******************************************************************/

/*******************************************************************
*    Prototype Declare Section
*******************************************************************/
void RTC2Time(void);
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
*    Function Name           :  Time_Adjust  
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
void Time_Adjust(void)
{
    T_STRUCT time; 
	
	time.year=DEFAULT_TIME_YEAR;
	time.month=DEFAULT_TIME_MONTH;
	time.day=DEFAULT_TIME_DAY;
	time.hour=DEFAULT_TIME_HOUR;
	time.minute=DEFAULT_TIME_MINUTE;
	time.sec=DEFAULT_TIME_SECOND;
	SetRTCTime(&time); 
}


//比较两个字符串指定长度的内容是否相等
//参数:s1,s2要比较的两个字符串;len,比较长度
//返回值:1,相等;0,不相等
u8 str_cmpx(u8*s1,u8*s2,u8 len)
{
	u8 i;
	for(i=0;i<len;i++)if((*s1++)!=*s2++)return 0;
	return 1;	   
}

const u8 *COMPILED_DATE=__DATE__;//获得编译日期
const u8 *COMPILED_TIME=__TIME__;//获得编译时间
const u8 Month_Tab[12][3]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"}; 
//自动设置时间为编译器时间   
void Auto_Time_Adjust(void)
{
	T_STRUCT time;
	u8 temp[3];
	u8 i;
	u8 mon,date;
	u16 year;
	u8 sec,min,hour;
	for(i=0;i<3;i++)temp[i]=COMPILED_DATE[i];   
	for(i=0;i<12;i++)if(str_cmpx((u8*)Month_Tab[i],temp,3))break;	
	mon=i+1;//得到月份
	if(COMPILED_DATE[4]==' ')date=COMPILED_DATE[5]-'0'; 
	else date=10*(COMPILED_DATE[4]-'0')+COMPILED_DATE[5]-'0';  
	//year=1000*(COMPILED_DATE[7]-'0')+100*(COMPILED_DATE[8]-'0')+10*(COMPILED_DATE[9]-'0')+COMPILED_DATE[10]-'0';
	year=10*(COMPILED_DATE[9]-'0')+COMPILED_DATE[10]-'0';	   
	hour=10*(COMPILED_TIME[0]-'0')+COMPILED_TIME[1]-'0';  
	min=10*(COMPILED_TIME[3]-'0')+COMPILED_TIME[4]-'0';  
	sec=10*(COMPILED_TIME[6]-'0')+COMPILED_TIME[7]-'0'; 

	time.year=year;
	time.month=mon;
	time.day=date;
	time.hour=hour;
	time.minute=min;
	time.sec=sec;
	SetRTCTime(&time); 
	//RTC_Set(year,mon,date,hour,min,sec)	;
	//printf("%d-%d-%d  %d:%d:%d\n",year,mon,date,hour,min,sec);
}

/***********************************
**函数名:RTCInit
**功能:RTC设置
**注意事项:要根据是否是第一次设置，才进入下面的RTC设定
			判断是否第一次设置，只需要判断RTC后备寄存器1的值是否为事先写入的0XA5A5，如果不是，则
			RTC是第一次上电，需要初始化RTC，并把实际时间转化为RTC计数值 
			返回FALSE则代表RTC没有被初始化.
************************************/
void RTCInit(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	if (BKP_ReadBackupRegister(BKP_TIME_SET) != 0xA5A5)
	{
		/* Reset Backup Domain */
		BKP_DeInit();

		/* Enable LSE */
		RCC_LSEConfig(RCC_LSE_ON);
		/* Wait till LSE is ready */
		//等待外部晶振震荡 需要等待比较长的时间
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);

		//使用外部晶振32768
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);  

		//允许RTC
		RCC_RTCCLKCmd(ENABLE);
		//等待RTC寄存器同步
		RTC_WaitForSynchro();

		RTC_WaitForLastTask();
		//允许RTC的秒中断(还有闹钟中断和溢出中断可设置)
		RTC_ITConfig(RTC_IT_SEC, ENABLE);

		RTC_WaitForLastTask();
		//32768晶振预分频值是32767
		//RTC_SetPrescaler(32767); /* 预分频值RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
		RTC_SetPrescaler(32776);	//如果需要校准晶振,可修改此分频值
		RTC_WaitForLastTask();

		//写入RTC后备寄存器1 0xa5a5
		BKP_WriteBackupRegister(BKP_TIME_SET, 0xA5A5);  

		//清除标志
		RCC_ClearFlag();

		//Time_Adjust();
		Auto_Time_Adjust();
	}
	//如果RTC已经设置
	else
	{
		//等待RTC与APB同步
		RTC_WaitForSynchro();
		RTC_WaitForLastTask();
		//刚上电的情况，需要把RTC的值转换为时间
		RTC2Time();
		//使能秒中断 
		RTC_ITConfig(RTC_IT_SEC, ENABLE);  
		RTC_WaitForLastTask();
	}
}



/******************************
**函数名:GetRTCTime
**功能:获取实际时间
**注意事项：获取值将放进参数指针内，注意获取前需要关闭RTC中断，则可以避免秒越界的问题
**
*******************************/
static T_STRUCT Real_Time;//实时时间值,只允许本模块调用,外部获取时间一律使用GetTime函数
void GetRTCTime(T_STRUCT* time)
{
	RTC_ITConfig(RTC_IT_SEC, DISABLE);	//关闭秒中断
	RTC_WaitForLastTask();

	time->year=Real_Time.year;
	time->month=Real_Time.month;
	time->day=Real_Time.day;
	time->hour=Real_Time.hour;
	time->minute=Real_Time.minute;
	time->sec=Real_Time.sec;
	time->date=Real_Time.date;

	RTC_WaitForLastTask();
	RTC_ITConfig(RTC_IT_SEC, ENABLE); //打开秒中断
}

/***********************************
**函数名:GetDate
**功能:计算星期
**注意事项:白菜星期算法,这个一般用在GPS获取日期后计算出星期.
************************************/
const u8 TAB_DATE[12]={6,2,2,5,0,3,5,1,4,6,2,4,};
u8 GetDate(T_STRUCT* time)
{
	return( (time->year + time->year/4 - ( (time->month<3)&&(time->year%4==0) ) + TAB_DATE[time->month-1] + time->day )%7);
} 

/*****************
**函数名:RTC2Time
**功能:把RTC内的计数器转换为实际时间  基数为  寄存器内的年月日
**注意事项:由于RTC只能存储秒，所以每次日跳进的时候都把年月日存到掉电寄存器内。
** 而每次时钟开电，则先读取之前的年月日，并检测RTC内的CLK是否大于一天，如大于，则进行必要的运算使寄存器内的日期更新。
** 这些都要在RTC中断前处理掉，然后RTC中断内，计时值永远不会大于0x0001517f（一日的秒总数）
******************/
const u8 Month2Day_Tab[12]={31,28,31,30,31,30,31,31,30,31,30,31} ;
void RTC2Time(void)
{
	u32 count;
	u8 tmp,change=0;

	Real_Time.year=BKP_ReadBackupRegister(BKP_TIME_YEAR);//年值
	Real_Time.month=BKP_ReadBackupRegister(BKP_TIME_MONTH);//月值
	Real_Time.day=BKP_ReadBackupRegister(BKP_TIME_DAY);//日值
	Real_Time.date=BKP_ReadBackupRegister(BKP_TIME_DATE);//星期值

	RTC_ITConfig(RTC_IT_SEC, DISABLE);	//为了避免代码重入引起的问题,这里吧RTC秒中断屏蔽了
	count=RTC_GetCounter();
	//计算新的年月日
	while (count>=0x0001517f)	 //上次关电到本次跨越了一天以上
	{
		change=1;
		count-=0x0001517f;
		//星期自加
		if ((++Real_Time.date)>=8)
			Real_Time.date=1;

		//如果是2月，计算闰年(不需要考虑2099以后的400年一非润）
		if (Real_Time.month==2)
		{
			if (Real_Time.year%4)
				tmp=28;
			else
				tmp=29;
		}
		else
		{
			tmp=Month2Day_Tab[Real_Time.month-1];
		}
		if ((++Real_Time.day)>tmp)
		{
			Real_Time.day=1;

			if ((++Real_Time.month)>12)
			{
				Real_Time.month=1;

				if ((++Real_Time.year)>=100)
				{
					Real_Time.year=0;
				}
			}
		}
	}
	//计算新的时分秒
	Real_Time.hour=count/3600;
	Real_Time.minute=(count%3600)/60;
	Real_Time.sec=(count%3600)%60;

	//如果跨越了一天，则计算后，要存回寄存器内
	if (change)
	{
		RTC_SetCounter(count);
		BKP_WriteBackupRegister(BKP_TIME_DATE,Real_Time.date);
		BKP_WriteBackupRegister(BKP_TIME_DAY,Real_Time.day);
		BKP_WriteBackupRegister(BKP_TIME_MONTH,Real_Time.month);
		BKP_WriteBackupRegister(BKP_TIME_YEAR,Real_Time.year);
	}
	//重新打开RTC中断
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
}


/*********************************
**函数名:SetRTCTime
**功能:设置时间，除了把Real_Time的值改变外,还要把时分秒转换为RTC计数值,年月日存到后备寄存器上
**注意事项:函数内会自动根据年月日计算星期,并且返回到*time上
**********************************/
void SetRTCTime(T_STRUCT* time)
{
	u32 count;
	RTC_ITConfig(RTC_IT_SEC, DISABLE);	//关闭秒中断

	RTC_WaitForLastTask();
	//付时间值到Real_Time上
	Real_Time.year=time->year;
	Real_Time.month=time->month;
	Real_Time.day=time->day;
	Real_Time.hour=time->hour;
	Real_Time.minute=time->minute;
	Real_Time.sec=time->sec;
	//计算星期
	time->date=Real_Time.date=GetDate(time);

	//把新的年月日存到掉电寄存器上

	BKP_WriteBackupRegister(BKP_TIME_DATE,Real_Time.date);
//	RTC_WaitForLastTask();
	BKP_WriteBackupRegister(BKP_TIME_DAY,Real_Time.day);
//	RTC_WaitForLastTask();
	BKP_WriteBackupRegister(BKP_TIME_MONTH,Real_Time.month);
//	RTC_WaitForLastTask();
	BKP_WriteBackupRegister(BKP_TIME_YEAR,Real_Time.year);
//	RTC_WaitForLastTask();

	//计算新的RTC count值
	count=Real_Time.hour*3600+Real_Time.minute*60+Real_Time.sec;
	RTC_WaitForLastTask();
	RTC_SetCounter(count);
	RTC_WaitForLastTask();
	RTC_ITConfig(RTC_IT_SEC, ENABLE); //打开秒中断
}
/**************************************************************
** 函数名:RTCTick
** 功能:RTC的秒跳动
** 注意事项:此函数在RTC秒中断中调用,则可在每个秒中断中刷新当前时间
***************************************************************/
void RTCTick(void)
{
	u8 tmp;
	if ((++Real_Time.sec)>59)
	{
		Real_Time.sec=0;
		if ((++Real_Time.minute)>59)
		{
			Real_Time.minute=0;
			if ((++Real_Time.hour)>23)
			{
				Real_Time.hour=0;
				//星期自加
				if ((++Real_Time.date)>=8)
					Real_Time.date=1;
				//--存储新的星期
				BKP_WriteBackupRegister(BKP_DR5,Real_Time.date);

				//如果是2月，计算闰年(不需要考虑2099以后的400年一非润）
				if (Real_Time.month==2)
				{
					if (Real_Time.year%4)
						tmp=28;
					else
						tmp=29;
				}
				else
				{
					tmp=Month2Day_Tab[Real_Time.month-1];
				}
				if ((++Real_Time.day)>tmp)
				{
					Real_Time.day=1;
					if ((++Real_Time.month)>12)
					{
						Real_Time.month=1;

						if ((++Real_Time.year)>99)
						{

							Real_Time.year=0;
						}
						//--储存新的年
						BKP_WriteBackupRegister(BKP_DR2,Real_Time.year);
					}
					//--储存新的月
					BKP_WriteBackupRegister(BKP_DR3,Real_Time.month);
				}
				//--储存新的日
				BKP_WriteBackupRegister(BKP_DR4,Real_Time.day);
			}
		}
	}
}

void GetTimeStr(u8 *time)
{
	T_STRUCT time_s;

	GetRTCTime(&time_s);
	time[0]= time_s.year/10 + '0';
	time[1]= time_s.year%10 + '0';
	time[2]= time_s.month/10 + '0';
	time[3]= time_s.month%10 + '0';
	time[4]= time_s.day/10 + '0';
	time[5]= time_s.day%10 + '0';
	time[6]= time_s.hour/10 + '0';
	time[7]= time_s.hour%10 + '0';
	time[8]= time_s.minute/10 + '0';
	time[9]= time_s.minute%10 + '0';
	time[10]= time_s.sec/10 + '0';
	time[11]= time_s.sec%10 + '0';
}

u8 StrToUchar(u8 ch, u8 cl)
{
	u8 val;

	val = (ch-'0')*10 + (cl-'0');

	return val;
}

void AdjustTimeStr(u8 *time)
{
	T_STRUCT time_s;

	time_s.year = StrToUchar(time[0], time[1]);
	time_s.month = StrToUchar(time[2], time[3]);
	time_s.day = StrToUchar(time[4], time[5]);
	time_s.hour = StrToUchar(time[6], time[7]);
	time_s.minute = StrToUchar(time[8], time[9]);
	time_s.sec = StrToUchar(time[10], time[11]);
	
	SetRTCTime(&time_s);
}

//void TimeDis(T_STRUCT *ptime)
//{
//	char dis_buf[18];
//
//	dis_buf[0]=ptime->year/10+'0';
//	dis_buf[1]=ptime->year%10+'0';
//	dis_buf[2]='/';
//	dis_buf[3]=ptime->month/10+'0';
//	dis_buf[4]=ptime->month%10+'0';
//	dis_buf[5]='/';
//	dis_buf[6]=ptime->day/10+'0';
//	dis_buf[7]=ptime->day%10+'0';
//	dis_buf[8]=' ';
//
//	dis_buf[9]=ptime->hour/10+'0';
//	dis_buf[10]=ptime->hour%10+'0';
//	dis_buf[11]=':';
//	dis_buf[12]=ptime->minute/10+'0';
//	dis_buf[13]=ptime->minute%10+'0';
//	dis_buf[14]=':';
//	dis_buf[15]=ptime->sec/10+'0';
//	dis_buf[16]=ptime->sec%10+'0';
//	dis_buf[17]='\0';
//
//	//hLM->WrASCII(56, 128, dis_buf);
//	//hLM->dis(56, 128, dis_buf, 0);
//}

void RtcTask(void *parg)
{
	T_STRUCT time_s;
	T_STRUCT *ptime=&time_s;
	parg = parg;
	for(;;)
	{
		GetRTCTime(ptime);
		GetDate(ptime);
		//TimeDis(ptime);
		printf("%d/%d/%d %d:%d:%d date:%d\r\n",ptime->year,ptime->month,ptime->day,ptime->hour,ptime->minute,ptime->sec,ptime->date);
		OSTimeDlyHMSM(0, 0, 1, 0);
	}
}

