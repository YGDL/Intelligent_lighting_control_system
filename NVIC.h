#ifndef NVIC_H_
#define NVIC_H_

#include "date.h"

extern Date_InitTypeDef Date;
extern u8 Alarm;

void RTC_IRQHandler(void);

#endif
