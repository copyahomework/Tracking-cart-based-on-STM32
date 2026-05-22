#ifndef _xunji_H
#define _xunji_H

#include "stm32f1xx_hal.h"                  // Device header

#define THRESHOLD 2000  //灰度传感器阈值

extern uint16_t AD_Value[3];
extern uint16_t huidu1;
extern uint16_t huidu2;
extern uint16_t huidu3;

void xunji(void);
void xunji_Tick(void);
#endif
