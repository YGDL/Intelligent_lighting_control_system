#include "stm32f10x.h"
#include "date.h"
#include "oled.h"
#include "gpio.h"
#include "delay.h"

void KEY_Detection(Date_InitTypeDef* Date)
{
	Date_InitTypeDef* Date_1;
	Date_1 = Date;
	u8 x=1;
	OLED_Clear();
	Date_Display(Date_1, x);
	while(1)
	{
		delay_ms(30);
		if(KEY_1 == 0)
		{	
			LED_OFF;
			Date = Date_1;			//更新原结构体
			OLED_Clear();
			Date_Display(Date, 0);
			break;
		}
		if(KEY_2 == 0)
			break;					//放弃设置
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
	OLED_Clear();
	delay_ms(30);
}

			
