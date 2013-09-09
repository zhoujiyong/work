/*******************************************************************
*
*    FILENAME             :  RTC.C
*
*    COPYRIGHT            :  ������Ѷ��ͨ�ż������޹�˾
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


//�Ƚ������ַ���ָ�����ȵ������Ƿ����
//����:s1,s2Ҫ�Ƚϵ������ַ���;len,�Ƚϳ���
//����ֵ:1,���;0,�����
u8 str_cmpx(u8*s1,u8*s2,u8 len)
{
	u8 i;
	for(i=0;i<len;i++)if((*s1++)!=*s2++)return 0;
	return 1;	   
}

const u8 *COMPILED_DATE=__DATE__;//��ñ�������
const u8 *COMPILED_TIME=__TIME__;//��ñ���ʱ��
const u8 Month_Tab[12][3]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"}; 
//�Զ�����ʱ��Ϊ������ʱ��   
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
	mon=i+1;//�õ��·�
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
**������:RTCInit
**����:RTC����
**ע������:Ҫ�����Ƿ��ǵ�һ�����ã��Ž��������RTC�趨
			�ж��Ƿ��һ�����ã�ֻ��Ҫ�ж�RTC�󱸼Ĵ���1��ֵ�Ƿ�Ϊ����д���0XA5A5��������ǣ���
			RTC�ǵ�һ���ϵ磬��Ҫ��ʼ��RTC������ʵ��ʱ��ת��ΪRTC����ֵ 
			����FALSE�����RTCû�б���ʼ��.
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
		//�ȴ��ⲿ������ ��Ҫ�ȴ��Ƚϳ���ʱ��
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);

		//ʹ���ⲿ����32768
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);  

		//����RTC
		RCC_RTCCLKCmd(ENABLE);
		//�ȴ�RTC�Ĵ���ͬ��
		RTC_WaitForSynchro();

		RTC_WaitForLastTask();
		//����RTC�����ж�(���������жϺ�����жϿ�����)
		RTC_ITConfig(RTC_IT_SEC, ENABLE);

		RTC_WaitForLastTask();
		//32768����Ԥ��Ƶֵ��32767
		//RTC_SetPrescaler(32767); /* Ԥ��ƵֵRTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
		RTC_SetPrescaler(32776);	//�����ҪУ׼����,���޸Ĵ˷�Ƶֵ
		RTC_WaitForLastTask();

		//д��RTC�󱸼Ĵ���1 0xa5a5
		BKP_WriteBackupRegister(BKP_TIME_SET, 0xA5A5);  

		//�����־
		RCC_ClearFlag();

		//Time_Adjust();
		Auto_Time_Adjust();
	}
	//���RTC�Ѿ�����
	else
	{
		//�ȴ�RTC��APBͬ��
		RTC_WaitForSynchro();
		RTC_WaitForLastTask();
		//���ϵ���������Ҫ��RTC��ֵת��Ϊʱ��
		RTC2Time();
		//ʹ�����ж� 
		RTC_ITConfig(RTC_IT_SEC, ENABLE);  
		RTC_WaitForLastTask();
	}
}



/******************************
**������:GetRTCTime
**����:��ȡʵ��ʱ��
**ע�������ȡֵ���Ž�����ָ���ڣ�ע���ȡǰ��Ҫ�ر�RTC�жϣ�����Ա�����Խ�������
**
*******************************/
static T_STRUCT Real_Time;//ʵʱʱ��ֵ,ֻ������ģ�����,�ⲿ��ȡʱ��һ��ʹ��GetTime����
void GetRTCTime(T_STRUCT* time)
{
	RTC_ITConfig(RTC_IT_SEC, DISABLE);	//�ر����ж�
	RTC_WaitForLastTask();

	time->year=Real_Time.year;
	time->month=Real_Time.month;
	time->day=Real_Time.day;
	time->hour=Real_Time.hour;
	time->minute=Real_Time.minute;
	time->sec=Real_Time.sec;
	time->date=Real_Time.date;

	RTC_WaitForLastTask();
	RTC_ITConfig(RTC_IT_SEC, ENABLE); //�����ж�
}

/***********************************
**������:GetDate
**����:��������
**ע������:�ײ������㷨,���һ������GPS��ȡ���ں���������.
************************************/
const u8 TAB_DATE[12]={6,2,2,5,0,3,5,1,4,6,2,4,};
u8 GetDate(T_STRUCT* time)
{
	return( (time->year + time->year/4 - ( (time->month<3)&&(time->year%4==0) ) + TAB_DATE[time->month-1] + time->day )%7);
} 

/*****************
**������:RTC2Time
**����:��RTC�ڵļ�����ת��Ϊʵ��ʱ��  ����Ϊ  �Ĵ����ڵ�������
**ע������:����RTCֻ�ܴ洢�룬����ÿ����������ʱ�򶼰������մ浽����Ĵ����ڡ�
** ��ÿ��ʱ�ӿ��磬���ȶ�ȡ֮ǰ�������գ������RTC�ڵ�CLK�Ƿ����һ�죬����ڣ�����б�Ҫ������ʹ�Ĵ����ڵ����ڸ��¡�
** ��Щ��Ҫ��RTC�ж�ǰ��������Ȼ��RTC�ж��ڣ���ʱֵ��Զ�������0x0001517f��һ�յ���������
******************/
const u8 Month2Day_Tab[12]={31,28,31,30,31,30,31,31,30,31,30,31} ;
void RTC2Time(void)
{
	u32 count;
	u8 tmp,change=0;

	Real_Time.year=BKP_ReadBackupRegister(BKP_TIME_YEAR);//��ֵ
	Real_Time.month=BKP_ReadBackupRegister(BKP_TIME_MONTH);//��ֵ
	Real_Time.day=BKP_ReadBackupRegister(BKP_TIME_DAY);//��ֵ
	Real_Time.date=BKP_ReadBackupRegister(BKP_TIME_DATE);//����ֵ

	RTC_ITConfig(RTC_IT_SEC, DISABLE);	//Ϊ�˱�������������������,�����RTC���ж�������
	count=RTC_GetCounter();
	//�����µ�������
	while (count>=0x0001517f)	 //�ϴιص絽���ο�Խ��һ������
	{
		change=1;
		count-=0x0001517f;
		//�����Լ�
		if ((++Real_Time.date)>=8)
			Real_Time.date=1;

		//�����2�£���������(����Ҫ����2099�Ժ��400��һ����
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
	//�����µ�ʱ����
	Real_Time.hour=count/3600;
	Real_Time.minute=(count%3600)/60;
	Real_Time.sec=(count%3600)%60;

	//�����Խ��һ�죬������Ҫ��ؼĴ�����
	if (change)
	{
		RTC_SetCounter(count);
		BKP_WriteBackupRegister(BKP_TIME_DATE,Real_Time.date);
		BKP_WriteBackupRegister(BKP_TIME_DAY,Real_Time.day);
		BKP_WriteBackupRegister(BKP_TIME_MONTH,Real_Time.month);
		BKP_WriteBackupRegister(BKP_TIME_YEAR,Real_Time.year);
	}
	//���´�RTC�ж�
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
}


/*********************************
**������:SetRTCTime
**����:����ʱ�䣬���˰�Real_Time��ֵ�ı���,��Ҫ��ʱ����ת��ΪRTC����ֵ,�����մ浽�󱸼Ĵ�����
**ע������:�����ڻ��Զ����������ռ�������,���ҷ��ص�*time��
**********************************/
void SetRTCTime(T_STRUCT* time)
{
	u32 count;
	RTC_ITConfig(RTC_IT_SEC, DISABLE);	//�ر����ж�

	RTC_WaitForLastTask();
	//��ʱ��ֵ��Real_Time��
	Real_Time.year=time->year;
	Real_Time.month=time->month;
	Real_Time.day=time->day;
	Real_Time.hour=time->hour;
	Real_Time.minute=time->minute;
	Real_Time.sec=time->sec;
	//��������
	time->date=Real_Time.date=GetDate(time);

	//���µ������մ浽����Ĵ�����

	BKP_WriteBackupRegister(BKP_TIME_DATE,Real_Time.date);
//	RTC_WaitForLastTask();
	BKP_WriteBackupRegister(BKP_TIME_DAY,Real_Time.day);
//	RTC_WaitForLastTask();
	BKP_WriteBackupRegister(BKP_TIME_MONTH,Real_Time.month);
//	RTC_WaitForLastTask();
	BKP_WriteBackupRegister(BKP_TIME_YEAR,Real_Time.year);
//	RTC_WaitForLastTask();

	//�����µ�RTC countֵ
	count=Real_Time.hour*3600+Real_Time.minute*60+Real_Time.sec;
	RTC_WaitForLastTask();
	RTC_SetCounter(count);
	RTC_WaitForLastTask();
	RTC_ITConfig(RTC_IT_SEC, ENABLE); //�����ж�
}
/**************************************************************
** ������:RTCTick
** ����:RTC��������
** ע������:�˺�����RTC���ж��е���,�����ÿ�����ж���ˢ�µ�ǰʱ��
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
				//�����Լ�
				if ((++Real_Time.date)>=8)
					Real_Time.date=1;
				//--�洢�µ�����
				BKP_WriteBackupRegister(BKP_DR5,Real_Time.date);

				//�����2�£���������(����Ҫ����2099�Ժ��400��һ����
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
						//--�����µ���
						BKP_WriteBackupRegister(BKP_DR2,Real_Time.year);
					}
					//--�����µ���
					BKP_WriteBackupRegister(BKP_DR3,Real_Time.month);
				}
				//--�����µ���
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
