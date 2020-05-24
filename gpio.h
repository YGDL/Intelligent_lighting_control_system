#ifndef GPIO_H_
#define GPIO_H_

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

//OLED屏部分

#define 	OLED_SPI_GPIO_CLK		RCC_APB2Periph_GPIOA
#define		OLED_SPI_CLK			RCC_APB2Periph_SPI1
#define		OLED_SPI_PORT			GPIOA

#define		OLED_SPI_CS_PIN			GPIO_Pin_4
#define		OLED_SPI_RES_PIN		GPIO_Pin_3
#define		OLED_SPI_SCL_PIN		GPIO_Pin_5
#define		OLED_SPI_SDA_PIN		GPIO_Pin_7
#define		OLED_SPI_DC_PIN			GPIO_Pin_2

#define		OLED_SPI_RES_ON			GPIO_SetBits( OLED_SPI_PORT, OLED_SPI_RES_PIN )
#define		OLED_SPI_RES_OFF		GPIO_ResetBits( OLED_SPI_PORT, OLED_SPI_RES_PIN )

#define		OLED_SPI_CS_ON			GPIO_SetBits( OLED_SPI_PORT, OLED_SPI_CS_PIN )
#define		OLED_SPI_CS_OFF			GPIO_ResetBits( OLED_SPI_PORT, OLED_SPI_CS_PIN )

#define		OLED_SPI_WRITE_COM		GPIO_ResetBits( OLED_SPI_PORT, OLED_SPI_DC_PIN )
#define		OLED_SPI_WRITE_DATA		GPIO_SetBits( OLED_SPI_PORT, OLED_SPI_DC_PIN )

//按键部分

#define		KEY_GPIO_CLK			RCC_APB2Periph_GPIOB
#define		KEY_PORT				GPIOB

#define		KEY_1_PIN				GPIO_Pin_15
#define		KEY_2_PIN				GPIO_Pin_4
#define 	KEY_3_PIN				GPIO_Pin_5
#define		KEY_4_PIN				GPIO_Pin_6
#define		KEY_5_PIN				GPIO_Pin_7
#define 	KEY_6_PIN				GPIO_Pin_8

#define 	KEY_1					GPIO_ReadInputDataBit(KEY_PORT, KEY_1_PIN)
#define 	KEY_2					GPIO_ReadInputDataBit(KEY_PORT, KEY_2_PIN)
#define 	KEY_3					GPIO_ReadInputDataBit(KEY_PORT, KEY_3_PIN)
#define 	KEY_4					GPIO_ReadInputDataBit(KEY_PORT, KEY_4_PIN)
#define 	KEY_5					GPIO_ReadInputDataBit(KEY_PORT, KEY_5_PIN)
#define 	KEY_6					GPIO_ReadInputDataBit(KEY_PORT, KEY_6_PIN)

//TEST key检测

#define 	LED_GPIO_CLK			RCC_APB2Periph_GPIOB
#define		LED_PORT				GPIOB
#define		LED_PIN					GPIO_Pin_12

#define		LED_ON					GPIO_ResetBits( LED_PORT, LED_PIN )
#define		LED_OFF					GPIO_SetBits( LED_PORT, LED_PIN )
#define 	LED_REVERSAL			{LED_PORT -> ODR ^= LED_PIN;}
#define		LED						GPIO_ReadInputDataBit(LED_PORT, LED_PIN)

//ADC

#define		Light_GPIO_CLK			RCC_APB2Periph_GPIOA
#define		Light_PORT				GPIOA
#define		Light_PIN				GPIO_Pin_0

//people detection

#define		Detection_GPIO_CLK		RCC_APB2Periph_GPIOB
#define		Detection_PORT			GPIOB
#define		Detection_PIN			GPIO_Pin_1

#define		Detection				GPIO_ReadInputDataBit(Detection_PORT, Detection_PIN)

void OLED_GPIO_Init(void);
void KEY_GPIO_Init(void);
void LED_GPIO_Init(void);
void Detection_GPIO_Init(void);


#endif
