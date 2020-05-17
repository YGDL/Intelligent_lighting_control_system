#include "stm32f10x.h"
#include "adc.h"
#include "gpio.h"
#include "delay.h"
#include "stm32f10x_it.h"

void Light_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	//ʹ��ADC1ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div2);						//��Ƶ2
	
	ADC_InitTypeDef ADC_InitStructure;
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;							//��ɨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;						//������ת������ֹƵ������
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;		//��ֹ����ת����ʹ���������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;					//�Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;									//1��ת���ڹ��������У�Ҳ����ֻת����������1
	
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1,ADC_SampleTime_1Cycles5);	//ͨ��ת������
	
	ADC_NVIC_Config();
	
	ADC_Cmd(ADC1, ENABLE);									//ʹ��ADC
	
	ADC_ResetCalibration(ADC1);								//����ָ����ADC��У׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1));				//��ȡADC����У׼�Ĵ�����״̬
	ADC_StartCalibration(ADC1);								//��ʼָ��ADC��У׼״̬
	while(ADC_GetCalibrationStatus(ADC1));					//��ȡָ��ADC��У׼����
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);					//ʹ�ܻ���ʧ��ָ����ADC�����ת����������
}
