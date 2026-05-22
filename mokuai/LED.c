#include "stm32f1xx_hal.h"

void onLED(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
	HAL_GPIO_WritePin(GPIOx,GPIO_Pin,GPIO_PIN_SET);
}

void offLED(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
	HAL_GPIO_WritePin(GPIOx,GPIO_Pin,GPIO_PIN_RESET);
}

void fanzhuanLED(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
	HAL_GPIO_TogglePin(GPIOx,GPIO_Pin);
}

void shanshuoLED(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
	static uint8_t count ;
	count++;
	if (count >= 20){
		count = 0;
		fanzhuanLED(GPIOx,GPIO_Pin);
	}
}
