#include "stm32f10x.h"
#include "date.h"
#include "oled.h"
#include "interface.h"
#include "gpio.h"
#include "NVIC.h"

void RTC_IRQHandler(void)
{
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)			//�����ж�
	{
		RTC_Get();										//����ʱ��
		Date_Display(&Date, 0);
	}
	if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)				//�����ж�
	{
		RTC_Get();										//����ʱ��
		
		if(Alarm == 0)
		{
			Alarm = 1;
			RTC_SetAlarm(RTC_GetCounter() - (Date.hour * 3600 + Date.minute * 60 + Date.second) + (BKP_ReadBackupRegister(BKP_DR1) & 0x1f) * 3600 + (BKP_ReadBackupRegister(BKP_DR2) & 0x3f) + ((BKP_ReadBackupRegister(BKP_DR2) >> 6) & 0x3f) * 60);
		}
		else
		{
			Alarm = 0;
			RTC_SetAlarm(RTC_GetCounter() - (Date.hour * 3600 + Date.minute * 60 + Date.second) + (BKP_ReadBackupRegister(BKP_DR3) & 0x1f) * 3600 + (BKP_ReadBackupRegister(BKP_DR4) & 0x3f) + ((BKP_ReadBackupRegister(BKP_DR4) >> 6) & 0x3f) * 60);
		}
	}
	RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);		//�������ж�
	RTC_WaitForLastTask();
}

void TIM3_IRQHandler(void)								//TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)	//���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);		//���TIMx���жϴ�����λ:TIM �ж�Դ 
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
		
		if(Alarm == 1 && ADC_GetConversionValue(ADC1) > 2450 && Detection == 1)
			LED_ON;
		else
			LED_OFF;
	}
}
