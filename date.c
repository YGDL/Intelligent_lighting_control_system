#include "stm32f10x.h"
#include "date.h"


/****************************************************************************************************************************************/



//�ж��Ƿ������꺯��
//�·�   1  2  3  4  5  6  7  8  9  10 11 12
//����   31 29 31 30 31 30 31 31 30 31 30 31
//������ 31 28 31 30 31 30 31 31 30 31 30 31
//����:���
//���:������ǲ�������.1,��.0,����

u8 Is_Leap_Year(u16 year)
{
	if(year%4==0)							//�����ܱ�4����
	{
		if(year%100==0)
		{
			if(year%400==0)
				return 1;					//�����00��β,��Ҫ�ܱ�400����
			else
				return 0;
		}
		else
			return 1;
	}
	else
		return 0;
}

//����ʱ��
//�������ʱ��ת��Ϊ����
//��1970��1��1��Ϊ��׼
//1970~2099��Ϊ�Ϸ����
//����ֵ:0,�ɹ�;����:�������.
//�·����ݱ�

const u8 week_table[12]={0,3,3,6,1,4,6,2,5,0,3,5};

//���������ݱ�
//ƽ����·����ڱ�

const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};

void RTC_Set(Date_InitTypeDef* Date)
{	
	u16 t;
	u32 seccount=0;
	
	for(t=1970;t< Date -> year;t++)			//��������ݵ��������
	{
		if(Is_Leap_Year(t))
			seccount+=31622400;				//�����������
		else
			seccount+=31536000;				//ƽ���������
	}
	
	Date -> month-=1;
	for(t=0;t< Date -> month;t++)			//��ǰ���·ݵ����������
	{
		seccount+=(u32)mon_table[t]*86400;	//�·����������
		if(Is_Leap_Year(Date -> year)&&t==1)seccount+=86400;	//����2�·�����һ���������
	}
	
	seccount+=(u32)(Date -> date-1)*86400;	//��ǰ�����ڵ����������
	seccount+=(u32)Date -> hour*3600;		//Сʱ������
	seccount+=(u32)Date -> minute*60;		//����������
	seccount+=Date -> second;				//�������Ӽ���ȥ
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);		//ʹ��PWR��BKP����ʱ��
	PWR_BackupAccessCmd(ENABLE);			//ʹ��RTC�ͺ󱸼Ĵ�������
	RTC_SetCounter(seccount);				//����RTC��������ֵ 
	RTC_WaitForLastTask();					//�ȴ����һ�ζ�RTC�Ĵ�����д�������
}

//*************************************************************************************************************************************//

void RTC_Get(void)
{
	static u16 daycnt=0;
	u32 timecount=0;
	u32 temp=0;
	u16 temp1=0;
	timecount=RTC_GetCounter();
	temp=timecount/86400;					//�õ�����(��������Ӧ��)
	if(daycnt!=temp)						//����һ����
	{
		daycnt=temp;
		temp1=1970;							//��1970�꿪ʼ
		while(temp>=365)
		{
			if(Is_Leap_Year(temp1))			//������
			{
				if(temp>=366)
					temp-=366;				//�����������
				else
				{
					temp1++;
					break;
				}
			}
			else
				temp-=365;					//ƽ��
			temp1++;
		}
		Date.year=temp1;					//�õ����
		temp1=0;
		while(temp>=28)						//������һ����
		{
			if(Is_Leap_Year(Date.year)&&temp1==1)		//�����ǲ�������/2�·�
			{
				if(temp>=29)
					temp-=29;				//�����������
				else
					break;
			}
			else
			{
				if(temp>=mon_table[temp1])
					temp-=mon_table[temp1];	//ƽ��
				else
					break;
			}
			temp1++;
		}
		Date.month=temp1+1;					//�õ��·�
		Date.date=temp+1;					//�õ�����
	}
	temp=timecount%86400;					//�õ�������
	Date.hour=temp/3600;					//Сʱ
	Date.minute=(temp%3600)/60; 			//����
	Date.second=(temp%3600)%60;				//����
	Date.week=RTC_Get_Week(Date.year,Date.month,Date.date);		//��ȡ����
}	 

//������������ڼ�
//��������:���빫�����ڵõ�����(ֻ����1901-2099��)
//������������������� 
//����ֵ�����ں�

u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{
	u16 temp2;
	u8 yearH,yearL;
	yearH=year/100;
	yearL=year%100;							// ���Ϊ21����,�������100
	if (yearH>19)yearL+=100;				// ����������ֻ��1900��֮���
  	temp2=yearL+yearL/4;
	temp2=temp2%7;
	temp2=temp2+day+week_table[month-1]; 
	if (yearL%4==0&&month<3)temp2--;
	return(temp2%7);
}   
