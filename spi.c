#include "stm32f10x.h"
#include "gpio.h"

void OLED_SPI_Init(void)
{
	RCC_APB2PeriphClockCmd(OLED_SPI_CLK, ENABLE);
	
	SPI_InitTypeDef SPI_InitStructure;
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	
	SPI_Init(SPI1, &SPI_InitStructure);
	
	SPI_Cmd(SPI1, ENABLE);
}


void OLED_WRITE(u8 data,u8 cmd)
{
	if(cmd==0)								//Ð´ÃüÁî
	{
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
		OLED_SPI_WRITE_COM;
		SPI_I2S_SendData(SPI1, data);
	}
	else if(cmd==1)							//Ð´Êý¾Ý
	{
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
		OLED_SPI_WRITE_DATA;
		SPI_I2S_SendData(SPI1, data);
	}
}
