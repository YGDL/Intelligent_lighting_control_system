#include "stm32f10x.h"

void SystemTick_Init()
{
	SysTick_Config(SystemCoreClock / 1000);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}
