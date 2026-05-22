#ifndef _LED_H
#define _LED_H

#include "stm32f1xx_hal.h"

#define LED7_GPIO					GPIOA
#define LED7_PIN					GPIO_PIN_7

void onLED(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void offLED(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void fanzhuanLED(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void shanshuoLED(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

#endif
