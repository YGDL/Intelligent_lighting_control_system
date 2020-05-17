#include "stm32f10x.h"
#include "gpio.h"

void OLED_GPIO_Init(void)
{
	RCC_APB2PeriphClockCmd(OLED_SPI_GPIO_CLK, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = OLED_SPI_CS_PIN | OLED_SPI_RES_PIN | OLED_SPI_DC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(OLED_SPI_PORT, &GPIO_InitStructure);			//设置CS/RES/DC引脚为通用推挽模式
	
	GPIO_InitStructure.GPIO_Pin = OLED_SPI_SCL_PIN | OLED_SPI_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	
	GPIO_Init(OLED_SPI_PORT, &GPIO_InitStructure);			//设置SCL/SDA引脚为强推挽模式
}

void KEY_GPIO_Init(void)
{	
	RCC_APB2PeriphClockCmd(KEY_GPIO_CLK, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = KEY_1_PIN | KEY_2_PIN | KEY_3_PIN | KEY_4_PIN | KEY_5_PIN | KEY_6_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	
	GPIO_Init(KEY_PORT, &GPIO_InitStructure);				//设置6个按键为上拉输入
}

void LED_GPIO_Init(void)
{
	RCC_APB2PeriphClockCmd(KEY_GPIO_CLK, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = LED_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(LED_PORT, &GPIO_InitStructure);				//设置LED引脚为通用推挽模式
}

void ADC_GPIO_Init(void)
{
	RCC_APB2PeriphClockCmd(KEY_GPIO_CLK, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = Light_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	
	GPIO_Init(Light_PORT, &GPIO_InitStructure);				//ADC引脚为模拟输入
}

void Detection_GPIO_Init(void)
{
	RCC_APB2PeriphClockCmd(Detection_GPIO_CLK, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = Detection_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	
	GPIO_Init(Detection_PORT, &GPIO_InitStructure);			//人体检测引脚为上拉输入
}
