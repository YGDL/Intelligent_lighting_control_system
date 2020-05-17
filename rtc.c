#include "stm32f10x.h"
#include "delay.h"
#include "NVIC.h"
#include "stm32f10x_it.h"
#include "date.h"
#include "oled.h"
#include "interface.h"


void RTC_Init(void)
{	
	u8 temp=0;												//检查是不是第一次配置时钟
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);		//使能PWR和BKP外设时钟
	PWR_BackupAccessCmd(ENABLE);							//使能后备寄存器访问
	
	if (BKP_ReadBackupRegister(BKP_DR10) != 0x5050)			//从指定的后备寄存器中读出数据:读出了与写入的指定数据不相乎
	{
		BKP_DeInit();										//复位备份区域
		
		RCC_LSEConfig(RCC_LSE_ON);							//设置外部低速晶振(LSE),使用外设低速晶振
		
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET && temp<250 )			//检查指定的RCC标志位设置与否,等待低速晶振就绪
		{
			temp++;
			delay_ms(10);
		}
															//	RCC_RTCCLKSource_LSE    RCC_RTCCLKSource_HSE_Div128
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);				//设置RTC时钟(RTCCLK),选择LSE作为RTC时钟
		
		RTC_WaitForLastTask();								//等待最近一次对RTC寄存器的写操作完成
		
		RCC_RTCCLKCmd(ENABLE);								//使能RTC时钟
		
		RTC_WaitForLastTask();								//等待最近一次对RTC寄存器的写操作完成
		
		RTC_WaitForSynchro();								//等待RTC寄存器同步
  		
		RTC_ITConfig(RTC_IT_SEC | RTC_IT_ALR, ENABLE);		//使能RTC秒中断		
		
		RTC_WaitForLastTask();								//等待最近一次对RTC寄存器的写操作完成
		
		RTC_EnterConfigMode();								//允许配置
		
		RTC_SetPrescaler(32767);							//设置RTC预分频的值
		
		RTC_WaitForLastTask();								//等待最近一次对RTC寄存器的写操作完成
		
		RTC_Set(&Date);										//设置时间
		
		
		
		RTC_ExitConfigMode();								//退出配置模式
		
		BKP_WriteBackupRegister(BKP_DR10, 0X5050);			//向指定的后备寄存器中写入用户程序数据
	}
	else													//系统继续计时
	{		
		RTC_WaitForLastTask();								//等待最近一次对RTC寄存器的写操作完成
		
		RTC_ITConfig(RTC_IT_SEC | RTC_IT_ALR, ENABLE);		//使能RTC秒中断
		
		RTC_WaitForLastTask();								//等待最近一次对RTC寄存器的写操作完成
	}
	RTC_NVIC_Config();										//RCT中断分组设置
	
	RTC_Get();												//更新时间
	OLED_Clear();
	Date_Display(&Date, 0);
	Interface_0_Display();
	
	RTC_WaitForLastTask();									//等待最近一次对RTC寄存器的写操作完成
	
	Alarm_Time_Set_();
}
