#ifndef DATE_H
#define DATE_H

#include "stm32f10x.h"

//**********************************时间相关的结构体、枚举************************************//

typedef enum{
	on = 0x00,
	off = 0x01,
}On_Off_TypeDef;

//时间结构体
typedef struct{
	s8  hour;
	s8  minute;
	s8  second;
	//公历日月年周
	vu16 year;
	s8  month;
	s8  date;
	s8  week;
}Date_InitTypeDef;

//闹钟结构体
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
