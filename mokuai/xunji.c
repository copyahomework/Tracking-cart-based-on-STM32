#include "xunji.h"
#include "motor.h"
#include "OLED.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>

uint16_t AD_Value[3];

uint16_t huidu1;
uint16_t huidu2;
uint16_t huidu3;
static uint8_t last_state = 0;

void xunji(void) {
    uint8_t left  = (huidu1 < THRESHOLD) ? 1 : 0;
    uint8_t mid   = (huidu2 < THRESHOLD) ? 1 : 0;
    uint8_t right = (huidu3 < THRESHOLD) ? 1 : 0;

    uint8_t state = (left << 2) | (mid << 1) | right;

    switch (state) {
        case 0x02: // 010: only mid
            qianjin();
            last_state = 0;
            break;

        case 0x04: // 100: only left
            zuoqian();
            last_state = 2;
            break;

        case 0x01: // 001: only right
            youqian();
            last_state = 1;
            break;

        case 0x03: // 110: right + mid
            youqian();
            last_state = 1;
            break;
				
				case 0x06: // 110: left + mid
            zuoqian();
            last_state = 2;
            break;
	
				case 0x07: // 111
            qianjin();
            last_state = 0;
            break;
				
        case 0x00: // 000: all lost
            if (last_state == 1) {
							youqian();
						}else if (last_state == 2) 
						{
							zuoqian();
						}
            else qianjin();
            break;

        default:
            qianjin();
            break;
    }
}

void xunji_Tick(void) {//定时向串口发送灰度传感器数据
    static uint16_t count = 0;

    count++;
    if (count >= 5000 ) {
			count = 0;
      
			char buf[64];
			int len = snprintf(buf, sizeof(buf),"zuo:%d zhong:%d you:%d\r\n",huidu1, huidu2, huidu3);
        HAL_UART_Transmit(&huart1, (uint8_t *)buf, len, HAL_MAX_DELAY);
    }
}
