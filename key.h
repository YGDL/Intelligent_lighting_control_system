#ifndef KEY_H_

#define KEY_H_

#include "stm32f10x.h"
#include "date.h"
#include "oled.h"





extern Date_InitTypeDef Date;


void KEY_Detection(Date_InitTypeDef* Date);







#endif
