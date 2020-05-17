#include "stm32f10x.h"
#include "gpio.h"
#include "interface.h"
#include "oled.h"
#include "delay.h"
#include "date.h"

/******************************************************ĳȫ�ֱ���****************************************************************************/

u8 Alarm = 0;								//����״̬��0������Ϊ�ص�״̬��1������Ϊ����״̬


/********************************************************����1******************************************************************************/

void Interface_1(void)
{
	RTC_ITConfig(RTC_IT_SEC, DISABLE);		//�ر����жϣ������ж�
	OLED_Clear();
	
	if(Mode == 0)
		Interface_1_Mode0();
	else
		Interface_1_Mode1();
	
	OLED_Clear();							//��ʼ�����ͼ

	Interface_0_Display();					//����Date�ṹ�壬����ʱ����ʾ
	
	RTC_Get();								//����ʱ��
	
	RTC_ITConfig(RTC_IT_SEC, ENABLE);		//�����ж�
	
	Interface_0_Display();					//ͨ���ж�Modeֵ���Ƿ���Ҫ�������жϣ��Լ�ADC�ж�
											//ǿ��ģʽ�£���Mode=1ʱ����
	
}

void Interface_1_Mode0(void)				//�Զ�ģʽ
{
	s8 x = 0;

	Interface_1_Display(Mode);				//д����1��ʾ
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
	Interface_1_Display(Mode);				//д����1��ʾ
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

/********************************************************����2******************************************************************************/

////////////////////////////////////////////////////////////////////ģʽ����/////////////////////////////////////////////////////////////////

void Mode_Set(int* Mode)
{
	s8 Mode_1;
	
	Mode_1 = *Mode;
	
	OLED_Clear();							//д����2��ʾ
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
		RTC_ITConfig(RTC_IT_ALR, ENABLE);				//ʹ�������ж�
		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);		//ʹ��TIM3
		
		Alarm_Time_Set_();								//��������
		if(Alarm == 0)
			LED_ON;
		else
			LED_OFF;
	}
	else
	{
		RTC_ITConfig(RTC_IT_ALR, DISABLE);				//ʧ�������ж�
		TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);		//ʧ��TIM3
	}
	
	OLED_Clear();										//д����1��ʾ
	
	Interface_1_Display(*Mode);
}

////////////////////////////////////////////////////////////////ʱ������////////////////////////////////////////////////////////////////////

void Time_Set(Date_InitTypeDef* Date)
{
	Date_InitTypeDef* Date_1;
	Date_1 = Date;
	u8 x=1;
	OLED_Clear();
	Date_Display(Date_1, x);							//д����2��ʾ
	Interface_2_Time_Set_Display();
	while(1)
	{
		delay_ms(50);
		if(KEY_1 == 0)
		{	
			Date = Date_1;								//����ԭ�ṹ��
			RTC_Set(Date);
			break;
		}
		if(KEY_2 == 0)
			break;										//��������
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
					if(Is_Leap_Year(Date_1 -> year) == 1 && Date_1 -> month == 2) //������
						if(Date_1 -> date > 29)
							Date_1 -> date = 1;
					if(Is_Leap_Year(Date_1 -> year) == 0 && Date_1 -> month == 2) //��������
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
					if(Is_Leap_Year(Date_1 -> year) == 1 && Date_1 -> month == 2) //������
						if(Date_1 -> date > 29)
							Date_1 -> date = 1;
					if(Is_Leap_Year(Date_1 -> year) == 0 && Date_1 -> month == 2) //��������
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
					if(Is_Leap_Year(Date_1 -> year) == 1 && Date_1 -> month == 2) //������
						if(Date_1 -> date < 1)
							Date_1 -> date = 29;
					if(Is_Leap_Year(Date_1 -> year) == 0 && Date_1 -> month == 2) //��������
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
					if(Is_Leap_Year(Date_1 -> year) == 1 && Date_1 -> month == 2) //������
						if(Date_1 -> date < 1)
							Date_1 -> date = 29;
					if(Is_Leap_Year(Date_1 -> year) == 0 && Date_1 -> month == 2) //��������
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
	
	OLED_Clear();								//д����1��ʾ
	Interface_1_Display(Mode);
	delay_ms(50);
}

/////////////////////////////////////////////////////////////��������////////////////////////////////////////////////////////////////////////

void Alarm_Set(void)							//��������
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
	
	Interface_1_Display(Mode);					//д����1��ʾ
	delay_ms(50);
}



void Alarm_Time_Set(u8 i)						//����ʱ������
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
			break;								//��������
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

void Alarm_Time_Set_(void)				//���ʱ��˳����ж�
{
	u32 Now_Time;						//���ڵ�ʱ��
	u32 Alarm_0;						//��һ�����ӵ�ʱ��
	u32 Alarm_1;						//�ڶ������ӵ�ʱ��
	u32 Backup_Data_1;					//����1������
	u32 Backup_Data_2;					//����2������
	u32 Time_Count=RTC_GetCounter();	//RTC�м���������
	RTC_Get();
	
	Now_Time = Date.hour * 3600 + Date.minute * 60 + Date.second;		//��������ʱ
	Backup_Data_1 = BKP_ReadBackupRegister(BKP_DR1) << 16 | BKP_ReadBackupRegister(BKP_DR2);
	Alarm_0 = (Backup_Data_1 & 0x3f) + ((Backup_Data_1 >> 6) & 0x3f) * 60 + ((Backup_Data_1 >> 16) & 0x1f) * 3600;		//����1ֵ
	Backup_Data_2 = BKP_ReadBackupRegister(BKP_DR3) << 16 | BKP_ReadBackupRegister(BKP_DR4);
	Alarm_1 = (Backup_Data_2 & 0x3f) + ((Backup_Data_2 >> 6) & 0x3f) * 60 + ((Backup_Data_2 >> 16) & 0x1f) * 3600;		//����2ֵ
	
	if(Now_Time < Alarm_1 || Now_Time > Alarm_0)
	{
		Alarm = 0;						//�����ص�״̬
		if((Backup_Data_1 >> 31) == 1)
		{
			RTC_WaitForLastTask();		//�ȴ����һ�ζ�RTC�Ĵ�����д�������
			RTC_SetAlarm(Time_Count - Now_Time + Alarm_1);				//�������ӵ�ֵ
			RTC_WaitForLastTask();		//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		}
			
	}
	else
	{
		Alarm = 1;						//��������״̬
		if((Backup_Data_2 >> 31) == 1)
		{
			RTC_WaitForLastTask();		//�ȴ����һ�ζ�RTC�Ĵ�����д�������
			RTC_SetAlarm(Time_Count - Now_Time + Alarm_0);
			RTC_WaitForLastTask();		//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		}
	}
}
