#ifndef DATE_H
#define DATE_H

#include "stm32f10x.h"

//**********************************ʱ����صĽṹ�塢ö��************************************//

typedef enum{
	on = 0x00,
	off = 0x01,
}On_Off_TypeDef;

//ʱ��ṹ��
typedef struct{
	s8  hour;
	s8  minute;
	s8  second;
	//������������
	vu16 year;
	s8  month;
	s8  date;
	s8  week;
}Date_InitTypeDef;

//���ӽṹ��
typedef struct{
//	On_Off_TypeDef	on_off;
//	s8 	week;
	s8	hour;
	s8	minute;
	s8	second;
}Alarm_InitTypeDef;

//********************************************************************************************//


u8 Is_Leap_Year(u16 year);
void RTC_Set(Date_InitTypeDef* Date);
void RTC_Get(void);
u8 RTC_Get_Week(u16 year,u8 month,u8 day);


extern Date_InitTypeDef Date;

















#endif
