#include "stm32f10x.h"
#include "delay.h"
#include "NVIC.h"
#include "stm32f10x_it.h"
#include "date.h"
#include "oled.h"
#include "interface.h"


void RTC_Init(void)
{	
	u8 temp=0;												//����ǲ��ǵ�һ������ʱ��
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);		//ʹ��PWR��BKP����ʱ��
	PWR_BackupAccessCmd(ENABLE);							//ʹ�ܺ󱸼Ĵ�������
	
	if (BKP_ReadBackupRegister(BKP_DR10) != 0x5050)			//��ָ���ĺ󱸼Ĵ����ж�������:��������д���ָ�����ݲ����
	{
		BKP_DeInit();										//��λ��������
		
		RCC_LSEConfig(RCC_LSE_ON);							//�����ⲿ���پ���(LSE),ʹ��������پ���
		
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET && temp<250 )			//���ָ����RCC��־λ�������,�ȴ����پ������
		{
			temp++;
			delay_ms(10);
		}
															//	RCC_RTCCLKSource_LSE    RCC_RTCCLKSource_HSE_Div128
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);				//����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��
		
		RTC_WaitForLastTask();								//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		
		RCC_RTCCLKCmd(ENABLE);								//ʹ��RTCʱ��
		
		RTC_WaitForLastTask();								//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		
		RTC_WaitForSynchro();								//�ȴ�RTC�Ĵ���ͬ��
  		
		RTC_ITConfig(RTC_IT_SEC | RTC_IT_ALR, ENABLE);		//ʹ��RTC���ж�		
		
		RTC_WaitForLastTask();								//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		
		RTC_EnterConfigMode();								//��������
		
		RTC_SetPrescaler(32767);							//����RTCԤ��Ƶ��ֵ
		
		RTC_WaitForLastTask();								//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		
		RTC_Set(&Date);										//����ʱ��
		
		
		
		RTC_ExitConfigMode();								//�˳�����ģʽ
		
		BKP_WriteBackupRegister(BKP_DR10, 0X5050);			//��ָ���ĺ󱸼Ĵ�����д���û���������
	}
	else													//ϵͳ������ʱ
	{		
		RTC_WaitForLastTask();								//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		
		RTC_ITConfig(RTC_IT_SEC | RTC_IT_ALR, ENABLE);		//ʹ��RTC���ж�
		
		RTC_WaitForLastTask();								//�ȴ����һ�ζ�RTC�Ĵ�����д�������
	}
	RTC_NVIC_Config();										//RCT�жϷ�������
	
	RTC_Get();												//����ʱ��
	OLED_Clear();
	Date_Display(&Date, 0);
	Interface_0_Display();
	
	RTC_WaitForLastTask();									//�ȴ����һ�ζ�RTC�Ĵ�����д�������
	
	Alarm_Time_Set_();
}
