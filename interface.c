#include "stm32f10x.h"
#include "gpio.h"
#include "interface.h"
#include "oled.h"
#include "delay.h"
#include "date.h"

/******************************************************某全局变量****************************************************************************/

u8 Alarm = 0;								//闹钟状态，0：闹钟为关灯状态；1：闹钟为开灯状态


/********************************************************界面1******************************************************************************/

void Interface_1(void)
{
	RTC_ITConfig(RTC_IT_SEC, DISABLE);		//关闭秒中断，闹钟中断
	OLED_Clear();
	
	if(Mode == 0)
		Interface_1_Mode0();
	else
		Interface_1_Mode1();
	
	OLED_Clear();							//初始界面绘图

	Interface_0_Display();					//更新Date结构体，更新时间显示
	
	RTC_Get();								//更新时间
	
	RTC_ITConfig(RTC_IT_SEC, ENABLE);		//打开秒中断
	
	Interface_0_Display();					//通过判断Mode值看是否需要打开闹钟中断，以及ADC中断
											//强制模式下，即Mode=1时不打开
	
}

void Interface_1_Mode0(void)				//自动模式
{
	s8 x = 0;

	Interface_1_Display(Mode);				//写界面1显示
	Display_Arrows(x);
	
	while(1)
	{
		delay_ms(50);
		if(KEY_1 == 0)
		{
			switch(x)
			{
				case 0:
					Mode_Set(&Mode);
					Display_Arrows(x);
					break;
				case 1:
					Time_Set(&Date);
					Display_Arrows(x);
					break;
				case 2:
					Alarm_Set();
					Display_Arrows(x);
					break;
			}
		}
		if(KEY_2 == 0)
			break;
		if(Mode == 0)
		{
			if(KEY_3 == 0)
			{
				x -= 1;
				if(x < 0)
					x = 2;
				OLED_Clear();
				Display_Arrows(x);
				Interface_1_Display(Mode);
			}
			if(KEY_6 == 0)
			{
				x += 1;
				if(x > 2)
					x = 0;
				OLED_Clear();
				Display_Arrows(x);
				Interface_1_Display(Mode);
			}
		}
	}
	delay_ms(50);
}

void Interface_1_Mode1()
{
	Interface_1_Display(Mode);				//写界面1显示
	Display_Arrows(0);
	
	while(1)
	{
		delay_ms(50);
		if(KEY_1 == 0)
			Mode_Set(&Mode);
			Display_Arrows(0);
		if(KEY_2 == 0)
			break;
	}
	delay_ms(50);	
}

/********************************************************界面2******************************************************************************/

////////////////////////////////////////////////////////////////////模式设置/////////////////////////////////////////////////////////////////

void Mode_Set(int* Mode)
{
	s8 Mode_1;
	
	Mode_1 = *Mode;
	
	OLED_Clear();							//写界面2显示
	Interface_2_Mode_Set_Display();
	Display_Arrows(Mode_1);
	
	while(1)
	{
		delay_ms(50);
		if(KEY_1 == 0)
		{
			*Mode = Mode_1;
			break;
		}
		if(KEY_2 == 0)
			break;
		if(KEY_3 == 0)
		{
			Mode_1 -= 1;
			if(Mode_1 < 0)
				Mode_1 = 1;
			OLED_Clear();
			Display_Arrows(Mode_1);
			Interface_2_Mode_Set_Display();
		}
		if(KEY_6 == 0)
		{
			Mode_1 += 1;
			if(Mode_1 > 1)
				Mode_1 = 0;
			OLED_Clear();
			Display_Arrows(Mode_1);
			Interface_2_Mode_Set_Display();
		}
		
	}
	
	delay_ms(50);
	
	if(Mode_1 == 0)
	{
		RTC_ITConfig(RTC_IT_ALR, ENABLE);				//使能闹钟中断
		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);		//使能TIM3
		
		Alarm_Time_Set_();								//更新闹钟
		if(Alarm == 0)
			LED_ON;
		else
			LED_OFF;
	}
	else
	{
		RTC_ITConfig(RTC_IT_ALR, DISABLE);				//失能闹钟中断
		TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);		//失能TIM3
	}
	
	OLED_Clear();										//写界面1显示
	
	Interface_1_Display(*Mode);
}

////////////////////////////////////////////////////////////////时间设置////////////////////////////////////////////////////////////////////

void Time_Set(Date_InitTypeDef* Date)
{
	Date_InitTypeDef* Date_1;
	Date_1 = Date;
	u8 x=1;
	OLED_Clear();
	Date_Display(Date_1, x);							//写界面2显示
	Interface_2_Time_Set_Display();
	while(1)
	{
		delay_ms(50);
		if(KEY_1 == 0)
		{	
			Date = Date_1;								//更新原结构体
			RTC_Set(Date);
			break;
		}
		if(KEY_2 == 0)
			break;										//放弃设置
		if(KEY_3 == 0)
		{
			switch(x)
			{
				case 1:
					Date_1 -> year += 1000;
					if(Date_1 -> year > 2099)
						Date_1 -> year = 2099;
					break;
					
					
				case 2:
					Date_1 -> year += 100;
					if(Date_1 -> year > 2099)
						Date_1 -> year = 2099;
					break;
					
					
				case 3:
					Date_1 -> year += 10;
					if(Date_1 -> year > 2099)
						Date_1 -> year = 2099;
					break;
					
					
				case 4:
					Date_1 -> year += 1;
					if(Date_1 -> year > 2099)
						Date_1 -> year = 2099;
					break;
					
					
				case 5:
					Date_1 -> month += 10;
					if(Date_1 -> month > 12)
						Date_1 -> month = 1;
					break;
					
					
				case 6:
					Date_1 -> month += 1;
					if(Date_1 -> month > 12)
						Date_1 -> month = 1;
					break;
					
					
				case 7:
					Date_1 -> date += 10;
					if(Is_Leap_Year(Date_1 -> year) == 1 && Date_1 -> month == 2) //是闰年
						if(Date_1 -> date > 29)
							Date_1 -> date = 1;
					if(Is_Leap_Year(Date_1 -> year) == 0 && Date_1 -> month == 2) //不是闰年
						if(Date_1 -> date > 28)
							Date_1 -> date = 1;
					if(Date_1 -> month == 1 || Date_1 -> month == 3 || Date_1 -> month == 5 || Date_1 -> month == 7 || Date_1 -> month == 8 || Date_1 -> month == 10 || Date_1 -> month == 12)
						if(Date_1 -> date > 31)
							Date_1 -> date = 1;
					if(Date_1 -> month == 4 || Date_1 -> month == 6 || Date_1 -> month == 9 || Date_1 -> month == 11)
						if(Date_1 -> date > 30)
							Date_1 -> date = 1;
					break;
						
						
				case 8:
					Date_1 -> date += 1;
					if(Is_Leap_Year(Date_1 -> year) == 1 && Date_1 -> month == 2) //是闰年
						if(Date_1 -> date > 29)
							Date_1 -> date = 1;
					if(Is_Leap_Year(Date_1 -> year) == 0 && Date_1 -> month == 2) //不是闰年
						if(Date_1 -> date > 28)
							Date_1 -> date = 1;
					if(Date_1 -> month == 1 || Date_1 -> month == 3 || Date_1 -> month == 5 || Date_1 -> month == 7 || Date_1 -> month == 8 || Date_1 -> month == 10 || Date_1 -> month == 12)
						if(Date_1 -> date > 31)
							Date_1 -> date = 1;
					if(Date_1 -> month == 4 || Date_1 -> month == 6 || Date_1 -> month == 9 || Date_1 -> month == 11)
						if(Date_1 -> date > 30)
							Date_1 -> date = 1;
					break;
						
						
				case 9:
					Date_1 -> hour += 10;
					if(Date_1 -> hour > 23)
						Date_1 -> hour = 0;
					break;
					
					
				case 10:
					Date_1 -> hour += 1;
					if(Date_1 -> hour > 23)
						Date_1 -> hour = 0;
					break;
					
					
				case 11:
					Date_1 -> minute += 10;
					if(Date_1 -> minute > 59)
						Date_1 -> minute = 0;
					break;
					
					
				case 12:
					Date_1 -> minute += 1;
					if(Date_1 -> minute > 59)
						Date_1 -> minute = 0;
					break;
					
					
				case 13:
					Date_1 -> second += 10;
					if(Date_1 -> second > 59)
						Date_1 -> second = 0;
					break;
					
					
				case 14:
					Date_1 -> second +=1;
					if(Date_1 -> second > 59)
						Date_1 -> second = 0;
					break;
			}
			Date_Display(Date_1, x);
		}
		if(KEY_4 == 0)
		{
			x--;
			if(x < 1)
				x = 14;
			Date_Display(Date_1, x);
		}
		if(KEY_5 == 0)
		{
			x++;
			if(x > 14)
				x = 1;
			Date_Display(Date_1, x);
		}
		if(KEY_6 == 0)
		{
			switch(x)
			{
				case 1:
					Date_1 -> year -= 1000;
					if(Date_1 -> year < 1970)
						Date_1 -> year = 1970;
					break;
					
					
				case 2:
					Date_1 -> year -= 100;
					if(Date_1 -> year < 1970)
						Date_1 -> year = 1970;
					break;
					
					
				case 3:
					Date_1 -> year -= 10;
					if(Date_1 -> year < 1970)
						Date_1 -> year = 1970;
					break;
					
					
				case 4:
					Date_1 -> year -= 1;
					if(Date_1 -> year < 1970)
						Date_1 -> year = 1970;
					break;
					
					
				case 5:
					Date_1 -> month -= 10;
					if(Date_1 -> month < 1)
						Date_1 -> month = 12;
					break;
					
					
				case 6:
					Date_1 -> month -= 1;
					if(Date_1 -> month < 1)
						Date_1 -> month = 12;
					break;
					
					
				case 7:
					Date_1 -> date -= 10;
					if(Is_Leap_Year(Date_1 -> year) == 1 && Date_1 -> month == 2) //是闰年
						if(Date_1 -> date < 1)
							Date_1 -> date = 29;
					if(Is_Leap_Year(Date_1 -> year) == 0 && Date_1 -> month == 2) //不是闰年
						if(Date_1 -> date < 1)
							Date_1 -> date = 28;
					if(Date_1 -> month == 1 || Date_1 -> month == 3 || Date_1 -> month == 5 || Date_1 -> month == 7 || Date_1 -> month == 8 || Date_1 -> month == 10 || Date_1 -> month == 12)
						if(Date_1 -> date < 1)
							Date_1 -> date = 31;
					if(Date_1 -> month == 4 || Date_1 -> month == 6 || Date_1 -> month == 9 || Date_1 -> month == 11)
						if(Date_1 -> date < 1)
							Date_1 -> date = 30;
					break;
						
						
				case 8:
					Date_1 -> date -= 1;
					if(Is_Leap_Year(Date_1 -> year) == 1 && Date_1 -> month == 2) //是闰年
						if(Date_1 -> date < 1)
							Date_1 -> date = 29;
					if(Is_Leap_Year(Date_1 -> year) == 0 && Date_1 -> month == 2) //不是闰年
						if(Date_1 -> date < 1)
							Date_1 -> date = 28;
					if(Date_1 -> month == 1 || Date_1 -> month == 3 || Date_1 -> month == 5 || Date_1 -> month == 7 || Date_1 -> month == 8 || Date_1 -> month == 10 || Date_1 -> month == 12)
						if(Date_1 -> date < 1)
							Date_1 -> date = 31;
					if(Date_1 -> month == 4 || Date_1 -> month == 6 || Date_1 -> month == 9 || Date_1 -> month == 11)
						if(Date_1 -> date < 1)
							Date_1 -> date = 30;
					break;
						
						
				case 9:
					Date_1 -> hour -= 10;
					if(Date_1 -> hour < 0)
						Date_1 -> hour = 23;
					break;
					
					
				case 10:
					Date_1 -> hour -= 1;
					if(Date_1 -> hour < 0)
						Date_1 -> hour = 23;
					break;
					
					
				case 11:
					Date_1 -> minute -= 10;
					if(Date_1 -> minute < 0)
						Date_1 -> minute = 59;
					break;
					
					
				case 12:
					Date_1 -> minute -= 1;
					if(Date_1 -> minute < 0)
						Date_1 -> minute = 59;
					break;
					
					
				case 13:
					Date_1 -> second -= 10;
					if(Date_1 -> second < 0)
						Date_1 -> second = 59;
					break;
					
					
				case 14:
					Date_1 -> second -= 1;
					if(Date_1 -> second < 0)
						Date_1 -> second = 59;
					break;
			}
			Date_Display(Date_1, x);
		}
	}
	
	Alarm_Time_Set_();
	
	OLED_Clear();								//写界面1显示
	Interface_1_Display(Mode);
	delay_ms(50);
}

/////////////////////////////////////////////////////////////闹钟设置////////////////////////////////////////////////////////////////////////

void Alarm_Set(void)							//闹钟设置
{
	s8 x = 0;
	
	RTC_ITConfig(RTC_IT_ALR, DISABLE);
	
	OLED_Clear();
	Interface_2_Alarm_Set_Display();
	Display_Arrows(x);
	
	while(1)
	{
		delay_ms(50);
		if(KEY_1 == 0)
		{
			Alarm_Time_Set(x);
			
			OLED_Clear();
			Interface_2_Alarm_Set_Display();
			Display_Arrows(x);
		}
		if(KEY_2 == 0)
			break;
		if(KEY_3 == 0)
		{
			x -= 1;
			if(x < 0)
				x = 1;
			OLED_Clear();
			Interface_2_Alarm_Set_Display();
			Display_Arrows(x);
		}
		if(KEY_6 == 0)
		{
			x += 1;
			if(x > 1)
				x = 0;
			OLED_Clear();
			Interface_2_Alarm_Set_Display();
			Display_Arrows(x);
		}
	}
	
	Alarm_Time_Set_();
	RTC_ITConfig(RTC_IT_ALR, ENABLE);
	
	OLED_Clear();
	
	Interface_1_Display(Mode);					//写界面1显示
	delay_ms(50);
}



void Alarm_Time_Set(u8 i)						//闹钟时间设置
{
	Alarm_InitTypeDef Alarm_InitTypeDefStructer = {0, 0, 0};
	s8 x = 1;
	u16 y;
	if(i == 0)
	{
		y = BKP_ReadBackupRegister(BKP_DR1);
		Alarm_InitTypeDefStructer.hour = y & 0x1f;
		y = BKP_ReadBackupRegister(BKP_DR2);
		Alarm_InitTypeDefStructer.minute = y >> 6 & 0x3f;
		Alarm_InitTypeDefStructer.second = y & 0x3f;
	}
	else
	{
		y = BKP_ReadBackupRegister(BKP_DR3);
		Alarm_InitTypeDefStructer.hour = y & 0x1f;
		y = BKP_ReadBackupRegister(BKP_DR4);
		Alarm_InitTypeDefStructer.minute = y >> 6 & 0x3f;
		Alarm_InitTypeDefStructer.second = y & 0x3f;
	}
	OLED_Clear();
	Interface_2_Alarm_Set_ON_OFF_Display(&Alarm_InitTypeDefStructer, x); 
	while(1)
	{
		delay_ms(50);
		if(KEY_1 == 0)
		{	
			if(i == 0)
			{
				BKP_WriteBackupRegister(BKP_DR1,1 << 15 | i << 14 | Alarm_InitTypeDefStructer.hour);
				BKP_WriteBackupRegister(BKP_DR2,((Alarm_InitTypeDefStructer.minute << 6) | Alarm_InitTypeDefStructer.second));
			}
			else
			{
				BKP_WriteBackupRegister(BKP_DR3,1 << 15 | i << 14 | Alarm_InitTypeDefStructer.hour);
				BKP_WriteBackupRegister(BKP_DR4,((Alarm_InitTypeDefStructer.minute << 6) | Alarm_InitTypeDefStructer.second));
			}
			break;
		}
		if(KEY_2 == 0)
			break;								//放弃设置
		if(KEY_3 == 0)
		{
			switch(x)
			{
				case 1:
					Alarm_InitTypeDefStructer.hour += 10;
					if(Alarm_InitTypeDefStructer.hour > 23)
						Alarm_InitTypeDefStructer.hour = 0;
					break;
					
					
				case 2:
					Alarm_InitTypeDefStructer.hour += 1;
					if(Alarm_InitTypeDefStructer.hour > 23)
						Alarm_InitTypeDefStructer.hour = 0;
					break;
					
					
				case 3:
					Alarm_InitTypeDefStructer.minute += 10;
					if(Alarm_InitTypeDefStructer.minute > 59)
						Alarm_InitTypeDefStructer.minute = 0;
					break;
					
					
				case 4:
					Alarm_InitTypeDefStructer.minute += 1;
					if(Alarm_InitTypeDefStructer.minute > 59)
						Alarm_InitTypeDefStructer.minute = 0;
					break;
					
					
				case 5:
					Alarm_InitTypeDefStructer.second += 10;
					if(Alarm_InitTypeDefStructer.second > 59)
						Alarm_InitTypeDefStructer.second = 0;
					break;
					
					
				case 6:
					Alarm_InitTypeDefStructer.second +=1;
					if(Alarm_InitTypeDefStructer.second > 59)
						Alarm_InitTypeDefStructer.second = 0;
					break;
			}
			Interface_2_Alarm_Set_ON_OFF_Display(&Alarm_InitTypeDefStructer, x);
		}
		if(KEY_4 == 0)
		{
			x--;
			if(x < 1)
				x = 6;
			Interface_2_Alarm_Set_ON_OFF_Display(&Alarm_InitTypeDefStructer, x);
		}
		if(KEY_5 == 0)
		{
			x++;
			if(x > 6)
				x = 1;
			Interface_2_Alarm_Set_ON_OFF_Display(&Alarm_InitTypeDefStructer, x);
		}
		if(KEY_6 == 0)
		{
			switch(x)
			{
				case 1:
					Alarm_InitTypeDefStructer.hour -= 10;
					if(Alarm_InitTypeDefStructer.hour < 0)
						Alarm_InitTypeDefStructer.hour = 23;
					break;
					
					
				case 2:
					Alarm_InitTypeDefStructer.hour -= 1;
					if(Alarm_InitTypeDefStructer.hour < 0)
						Alarm_InitTypeDefStructer.hour = 23;
					break;
					
					
				case 3:
					Alarm_InitTypeDefStructer.minute -= 10;
					if(Alarm_InitTypeDefStructer.minute < 0)
						Alarm_InitTypeDefStructer.minute = 59;
					break;
					
					
				case 4:
					Alarm_InitTypeDefStructer.minute -= 1;
					if(Alarm_InitTypeDefStructer.minute < 0)
						Alarm_InitTypeDefStructer.minute = 59;
					break;
					
					
				case 5:
					Alarm_InitTypeDefStructer.second -= 10;
					if(Alarm_InitTypeDefStructer.second < 0)
						Alarm_InitTypeDefStructer.second = 59;
					break;
					
					
				case 6:
					Alarm_InitTypeDefStructer.second -=1;
					if(Alarm_InitTypeDefStructer.second < 0)
						Alarm_InitTypeDefStructer.second = 59;
					break;
			}
			Interface_2_Alarm_Set_ON_OFF_Display(&Alarm_InitTypeDefStructer, x);
		}
	}
}

void Alarm_Time_Set_(void)				//完成时间顺序的判定
{
	u32 Now_Time;						//现在的时间
	u32 Alarm_0;						//第一个闹钟的时间
	u32 Alarm_1;						//第二个闹钟的时间
	u32 Backup_Data_1;					//闹钟1的数据
	u32 Backup_Data_2;					//闹钟2的数据
	u32 Time_Count=RTC_GetCounter();	//RTC中计数器数据
	RTC_Get();
	
	Now_Time = Date.hour * 3600 + Date.minute * 60 + Date.second;		//计算现在时
	Backup_Data_1 = BKP_ReadBackupRegister(BKP_DR1) << 16 | BKP_ReadBackupRegister(BKP_DR2);
	Alarm_0 = (Backup_Data_1 & 0x3f) + ((Backup_Data_1 >> 6) & 0x3f) * 60 + ((Backup_Data_1 >> 16) & 0x1f) * 3600;		//闹钟1值
	Backup_Data_2 = BKP_ReadBackupRegister(BKP_DR3) << 16 | BKP_ReadBackupRegister(BKP_DR4);
	Alarm_1 = (Backup_Data_2 & 0x3f) + ((Backup_Data_2 >> 6) & 0x3f) * 60 + ((Backup_Data_2 >> 16) & 0x1f) * 3600;		//闹钟2值
	
	if(Now_Time < Alarm_1 || Now_Time > Alarm_0)
	{
		Alarm = 0;						//表明关灯状态
		if((Backup_Data_1 >> 31) == 1)
		{
			RTC_WaitForLastTask();		//等待最近一次对RTC寄存器的写操作完成
			RTC_SetAlarm(Time_Count - Now_Time + Alarm_1);				//设置闹钟的值
			RTC_WaitForLastTask();		//等待最近一次对RTC寄存器的写操作完成
		}
			
	}
	else
	{
		Alarm = 1;						//表明开灯状态
		if((Backup_Data_2 >> 31) == 1)
		{
			RTC_WaitForLastTask();		//等待最近一次对RTC寄存器的写操作完成
			RTC_SetAlarm(Time_Count - Now_Time + Alarm_0);
			RTC_WaitForLastTask();		//等待最近一次对RTC寄存器的写操作完成
		}
	}
}
