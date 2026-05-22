#ifndef __KEY_H
#define __KEY_H

/*

| uint8_t Key_Flag |      
|REPEAT | LONG | DOUBLE | SINGLE | UP | DOWN | HOLD |
| 位 | 名称   | 释义     | 功能描述                                                                 |
|----|--------|----------|--------------------------------------------------------------------------|
| 0  | HOLD   | 按住不放 | 按键按住不放时置1，按键松开时置0                                         |
| 1  | DOWN   | 按下时刻 | 按键按下的时刻置1                                                        |
| 2  | UP     | 松开时刻 | 按键松开的时刻置1                                                        |
| 3  | SINGLE | 单击     | 按键按下松开后，没有再次按下，超过双击时间阈值的时刻置1                 |
| 4  | DOUBLE | 双击     | 按键按下松开后，在双击时间阈值内再次按下的时刻置1                       |
| 5  | LONG   | 长按     | 按键按住不放，超过长按时间阈值的时刻置1                                 |
| 6  | REPEAT | 重复     | 按键长按后，每隔重复时间阈值置一次1，直到按键松开                       |

### 说明：

- **HOLD、DOWN、UP**：在任何时刻，只要检测到对应的事件，就会置标志位。
- **SINGLE、DOUBLE、LONG/REPEAT**：三者互斥，一次完整的按键流程，只会置其中一类标志位。
- **HOLD** 自动置1和清0，其余标志位在检测到指定事件的时刻置1，读后清0。
*/

#define KEY_PRESSED					1
#define KEY_UNPRESSED				0

#define KEY_HOLD						0X01
#define KEY_DOWN						0X02
#define KEY_UP							0X04
#define KEY_SINGLE					0X08
#define KEY_DOUBLE					0X10
#define KEY_LONG						0X20
#define KEY_REPET						0X40

#define KEY_TIME_DOUBLE						2000
#define KEY_TIME_LONG							3000
#define KEY_TIME_REPET						1000

void Key_Init(void);
uint8_t Key_checkFlag(uint8_t Flag);
void Key_Tick(void);

#endif
