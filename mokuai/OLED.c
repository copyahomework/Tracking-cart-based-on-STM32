/**
 ****************************************************************************************************
 * @file        OLED.c
 * @author      @#enbor;
 * @date        2024年5月11日 22点26分；
 * @brief       HAL库OLED显示; 
 ****************************************************************************************************
 * @attention
 *
 * 实验平台: STM32F103C8T6开发板
 *
 ****************************************************************************************************
 **/
 
#include "OLED.h"
#include "OLED_Font.h"   
#include "stm32f1xx_hal.h"                  // Device header
/* oled_i2c_hal.c */
#include "i2c.h"          // CubeMX 生成

#define OLED_ADDR 0x78    // 7-bit 0x3C << 1

/* 底层封装：HAL I2C 发送 */
static void OLED_Write(uint8_t ctrl, uint8_t data)
{
    uint8_t buf[2] = {ctrl, data};
    HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDR, buf, 2, HAL_MAX_DELAY);
}

/* 写命令 */
void OLED_WriteCommand(uint8_t cmd)
{
    OLED_Write(0x00, cmd);
}

/* 写数据 */
void OLED_WriteData(uint8_t data)
{
    OLED_Write(0x40, data);
}

/* 设置光标 */
void OLED_SetCursor(uint8_t y, uint8_t x)
{
    OLED_WriteCommand(0xB0 | y);
    OLED_WriteCommand(0x10 | ((x >> 4) & 0x0F));
    OLED_WriteCommand(0x00 | (x & 0x0F));
}

/* 清屏 */
void OLED_Clear(void)
{
    for (uint8_t y = 0; y < 8; y++) {
        OLED_SetCursor(y, 0);
        for (uint16_t x = 0; x < 128; x++)
            OLED_WriteData(0x00);
    }
}

/**
  * @brief  OLED显示一个字符
  * @param  Line 行位置，范围：1~4
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的一个字符，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//设置光标位置在上半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i]);			//显示上半部分内容
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//设置光标位置在下半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);		//显示下半部分内容
	}
}

/**
  * @brief  OLED显示字符串
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(Line, Column + i, String[i]);
	}
}

/**
  * @brief  OLED次方函数
  * @retval 返回值等于X的Y次方
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

/**
  * @brief  OLED显示数字（十进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~4294967295
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED显示数字（十进制，带符号数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-2147483648~2147483647
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar(Line, Column, '+');
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar(Line, Column, '-');
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED显示数字（十六进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~0xFFFFFFFF
  * @param  Length 要显示数字的长度，范围：1~8
  * @retval 无
  */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)							
	{
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		if (SingleNumber < 10)
		{
			OLED_ShowChar(Line, Column + i, SingleNumber + '0');
		}
		else
		{
			OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
}

/**
  * @brief  OLED显示数字（二进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
  * @param  Length 要显示数字的长度，范围：1~16
  * @retval 无
  */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
	}
}

/**
  * @brief  OLED初始化
  * @param  无
  * @retval 无
  */
/* 初始化序列 */
void OLED_Init(void)
{
    HAL_Delay(100);      // 电源稳定

    OLED_WriteCommand(0xAE); // DISPLAYOFF
    OLED_WriteCommand(0xD5); // SETDISPLAYCLOCKDIV
    OLED_WriteCommand(0x80);
    OLED_WriteCommand(0xA8); // SETMULTIPLEX
    OLED_WriteCommand(0x3F);
    OLED_WriteCommand(0xD3); // SETDISPLAYOFFSET
    OLED_WriteCommand(0x00);
    OLED_WriteCommand(0x40); // SETSTARTLINE
    OLED_WriteCommand(0xA1); // SEGREMAP
    OLED_WriteCommand(0xC8); // COMSCANDEC
    OLED_WriteCommand(0xDA); // SETCOMPINS
    OLED_WriteCommand(0x12);
    OLED_WriteCommand(0x81); // SETCONTRAST
    OLED_WriteCommand(0xCF);
    OLED_WriteCommand(0xD9); // SETPRECHARGE
    OLED_WriteCommand(0xF1);
    OLED_WriteCommand(0xDB); // SETVCOMDETECT
    OLED_WriteCommand(0x30);
    OLED_WriteCommand(0xA4); // DISPLAYALLON_RESUME
    OLED_WriteCommand(0xA6); // NORMALDISPLAY
    OLED_WriteCommand(0x8D); // CHARGEPUMP
    OLED_WriteCommand(0x14);
    OLED_WriteCommand(0xAF); // DISPLAYON

    OLED_Clear();
}
