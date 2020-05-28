#include "stm32f10x.h"
#include "stm32f10x_it.h"


void TIM1_Init(void)
{
	RCC_PCLK1Config(RCC_HCLK_Div16);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);		//ʹ��TIM1ʱ��
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;				//TIMxʱ�������λ��������Ϣ
	
	TIM_TimeBaseStructure.TIM_Prescaler = 1000 -1;				//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_Period  = 7200 - 1;				//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ   ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//ʱ�ӷ�Ƶ1
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;            
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);				//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);					//ʹ���ж�
	
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	
	TIM3_NVIC_Config();
	
	TIM_Cmd(TIM3, ENABLE);										//ʹ��TIMx����
}
