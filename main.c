#include "stm32f10x.h"
#include "gpio.h"
#include "oled.h"
#include "rtc.h"
#include "adc.h"
#include "spi.h"
#include "date.h"
#include "key.h"
#include "interface.h"
#include "delay.h"
#include "tim.h"


Date_InitTypeDef Date={0,0,0,2000,1,1,1};//时钟结构体

int Mode = 0;				//模式标志位，初始为自动模式

int main()
{
	OLED_GPIO_Init();
	LED_GPIO_Init();
	KEY_GPIO_Init();
	Detection_GPIO_Init();
	OLED_SPI_Init();
	OLED_Init();
	
	OLED_Clear();
	
	Display_Init();		//调用显示 “初始化中……” 函数
	TIM1_Init();
	Light_Init();
	RTC_Init();
	
	while(1)
	{
		if(KEY_1 == 0)
			Interface_1();
		if(Mode == 1)
		{
			if(KEY_2 == 0)
				LED_REVERSAL;
		}
		ON_OFF_Display(!LED);
		delay_ms(50);
	}
}
