#include "stm32f10x.h"
#include "adc.h"
#include "gpio.h"
#include "delay.h"
#include "stm32f10x_it.h"

void Light_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	//使能ADC1时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div2);						//分频2
	
	ADC_InitTypeDef ADC_InitStructure;
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;							//非扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;						//非连续转换，防止频繁进入
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;		//禁止触发转换，使用软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;					//右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;									//1个转换在规则序列中，也就是只转换规则序列1
	
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1,ADC_SampleTime_1Cycles5);	//通道转换配置
	
	ADC_NVIC_Config();
	
	ADC_Cmd(ADC1, ENABLE);									//使能ADC
	
	ADC_ResetCalibration(ADC1);								//重置指定的ADC的校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC1));				//获取ADC重置校准寄存器的状态
	ADC_StartCalibration(ADC1);								//开始指定ADC的校准状态
	while(ADC_GetCalibrationStatus(ADC1));					//获取指定ADC的校准程序
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);					//使能或者失能指定的ADC的软件转换启动功能
}
