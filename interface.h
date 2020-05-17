#ifndef INTERFACE_H_
#define INTERFACE_H_

#include "stm32f10x.h"
#include "date.h"

extern Date_InitTypeDef Date;
extern int Mode;



void Interface_1(void);
void Interface_1_Mode0(void);
void Interface_1_Mode1(void);
void Mode_Set(int* Mode);
void Time_Set(Date_InitTypeDef* Date);
void Alarm_Set(void);
void Alarm_Time_Set(u8 i);
void Alarm_Time_Set_(void);


#endif
