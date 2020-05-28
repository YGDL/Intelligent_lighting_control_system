#include "stm32f10x.h"
#include "stm32f10x_it.h"


void TIM1_Init(void)
{
	RCC_PCLK1Config(RCC_HCLK_Div16);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);		//使能TIM1时钟
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;				//TIMx时间基数单位的配置信息
	
	TIM_TimeBaseStructure.TIM_Prescaler = 1000 -1;				//设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_Period  = 7200 - 1;				//设置在下一个更新事件装入活动的自动重装载寄存器周期的值   计数到5000为500ms
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//时钟分频1
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;            
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);				//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);					//使能中断
	
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	
	TIM3_NVIC_Config();
	
	TIM_Cmd(TIM3, ENABLE);										//使能TIMx外设
}
