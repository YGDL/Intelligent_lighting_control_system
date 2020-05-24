#ifndef OLED_H_
#define OLED_H_

#include "stm32f10x.h"
#include "date.h"


#define OLED_COM	0
#define OLED_DATA	1

extern int Mode;

void OLED_Init(void);
void OLED_Display_Off(void);
void OLED_Display_On(void);
void OLED_SetPos(u8 x, u8 y);
void OLED_Clear(void);
void OLED_Display(u8 x, u8 y,u8 c,u8 size,u8 underline);
void Display_Init(void);
void Date_Display(Date_InitTypeDef* Date, u8 i);
void Display_Arrows(s8 x);
void Interface_1_Display(s8 Mode);
void Interface_2_Mode_Set_Display(void);
void Interface_2_Time_Set_Display(void);
void Interface_0_Display(void);
void Interface_2_Alarm_Set_Display(void);
void Interface_2_Alarm_Set_ON_OFF_Display(Alarm_InitTypeDef* Alarm_InitTypeDefStructer, u8 i);
void ON_OFF_Display(u8 i);
void Interface_2_Alarm_Set(void);




#endif
