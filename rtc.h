#ifndef RTC_H
#define RTC_H

#include "stm32f10x.h"
#include "date.h"


extern Date_InitTypeDef Date;


void RTC_Init(void);

#endif
