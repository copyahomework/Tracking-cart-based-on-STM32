#include "stm32f1xx_hal.h"
#include "Delay.h"
#include "Key.h"

uint8_t Key_Flag;

/**
  * 函    数：按键初始化
  * 参    数：无
  * 返 回 值：无
  */

/**
  * 函    数：按键获取键码
  * 参    数：无
  * 返 回 值：按下按键的键码值，范围：0~2，返回0代表没有按键按下
  * 注意事项：此函数是阻塞式操作，当按键按住不放时，函数会卡住，直到按键松手
  */

uint8_t Key_getkeyNum(void)//获取按键状态
{
	if (HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13) == 0) return KEY_PRESSED;
	return KEY_UNPRESSED;
}

uint8_t Key_checkFlag(uint8_t Flag){//检查标志位
	if (Key_Flag & Flag){
		if(Flag != KEY_HOLD){
			Key_Flag &= ~Flag;//HOLD在中断自动清零，其他标志位手动清零
		}
		return 1;
	}
		return 0;
}

void Key_Tick(void){
	static uint8_t count ;
	static uint8_t NowKeynum,LastKeynum;
	static uint8_t anjian;
	/*
	按键状态说明：
	anjian = 0：按键按下
	anjian = 1：判断按键是否是长按
	anjian = 2：判断按键是否是双击
	anjian = 3：
	anjian = 4：按键长按
	*/
	
	static uint16_t Time;
	
	if(Time > 0) Time --;
	
	count++;
	if (count >= 20){
		count = 0;
		
		LastKeynum = NowKeynum;
		NowKeynum = Key_getkeyNum();
		
		if(NowKeynum == KEY_PRESSED && LastKeynum == KEY_UNPRESSED){
			//DOWN = 1	按下
			Key_Flag |= KEY_DOWN;
		}
		
		if (NowKeynum == KEY_UNPRESSED && LastKeynum == KEY_PRESSED) {
			//UP = 1 松开
			Key_Flag |= KEY_UP;
		}
		
		switch(anjian){
			case 0:
				if(NowKeynum == KEY_PRESSED){
					//HOLD = 1  按ing
					Key_Flag |= KEY_HOLD;
					Time = KEY_TIME_LONG;
					anjian = 1;
				}
				break;
				
			case 1:
				if(NowKeynum == KEY_UNPRESSED){
					//HOLD = 0
					Key_Flag &= ~KEY_HOLD;
					Time = KEY_TIME_DOUBLE;
					anjian = 2;
				} else if(Time == 0){
					Time = KEY_TIME_REPET;
					//LONG = 1
					Key_Flag |= KEY_LONG;
					anjian = 4;
				}
				break;
				
				
			case 2:
				if(NowKeynum == KEY_PRESSED){
					//DOUBLE = 1
					Key_Flag |= KEY_DOUBLE;
					anjian = 3;
				}else if(Time == 0){
					//SINGLE = 1
					Key_Flag |= KEY_SINGLE;
					anjian = 0;
				}
				break;
				
			case 3:
				if(NowKeynum == KEY_UNPRESSED){
					anjian = 0;
				}
				break;
				
			case 4:
				if(NowKeynum == KEY_UNPRESSED){
					anjian = 0;
				}else if(Time == 0){
					Time = KEY_TIME_REPET;
				}
				break;
				
			default:break;
		}
	}
}
