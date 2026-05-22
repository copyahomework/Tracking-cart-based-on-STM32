/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "motor.h"
#include "Delay.h"
#include "OLED.h"
#include "Key.h"
#include "LED.h"
#include "xunji.h"
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t data;//串口收发数据
uint8_t setXunji = 0;//循迹模式标志位
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void run(uint8_t num){//串口接收字符后执行相应操作
	 	switch(num){
		case'w':	
			qianjin();
			OLED_ShowString(1,1,"                ");
			OLED_ShowString(1,1,"go");
			break;
		case's':
			shanshuoLED(LED7_GPIO,LED7_PIN);
			houtui();
		  OLED_ShowString(1,1,"                ");
			OLED_ShowString(1,1,"back");
			break;
		case'a':
			zuoqian();	
			OLED_ShowString(1,1,"                ");
			OLED_ShowString(1,1,"left");
			break;
		case'd':
			youqian();	
			OLED_ShowString(1,1,"                ");
			OLED_ShowString(1,1,"right");
			break;
		case't':
			tingche();
			OLED_ShowString(1,1,"                ");
			OLED_ShowString(1,1,"stop");
			break;
		case'x':
			setXunji = !setXunji;
			OLED_ShowString(1,1,"                ");
			OLED_ShowString(1,1,"xunji");
			break;
		default:
			tingche();
			OLED_ShowString(1,1,"                ");
			OLED_ShowString(1,1,"stop");
		break;
	}

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	Key_Tick();
	xunji_Tick();
}

uint8_t rx_buf[1];          // 1 字节缓冲
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
      run(rx_buf[0]);                 // 直接解析
      HAL_UART_Receive_IT(&huart1, rx_buf, 1); // 重新挂接收
    }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim2);
	
	motor_Init();
	OLED_Init();
	
	onLED(LED7_GPIO,LED7_PIN);
	
	HAL_ADCEx_Calibration_Start(&hadc1);//校准adc
	HAL_ADC_Start_DMA(&hadc1,(uint32_t*)AD_Value,3);
	
	OLED_ShowString(1,1,"Ciallo!");
	OLED_ShowString(2,1,"zuo:");
	OLED_ShowString(3,1,"zhong:");
	OLED_ShowString(4,1,"you:");
	
	HAL_UART_Receive_IT(&huart1, rx_buf, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  { 
		//循迹模式
		if(setXunji){
			xunji();
		}
		
		//按下按键中
		//if(Key_checkFlag(KEY_HOLD)){
		//	
		//}
		
		/*按键按下*/
		if(Key_checkFlag(KEY_DOWN)){
			OLED_ShowString(1,1,"                ");
			OLED_ShowString(1,1,"LOADING~~");
		}

		/*按键抬起*/
		if(Key_checkFlag(KEY_UP)){
			
		}
		
		if(Key_checkFlag(KEY_SINGLE)){//单击状态相应
			for(int i = 0 ; i < 3 ; i ++){
				onLED(LED7_GPIO,LED7_PIN);
				Delay_ms(100);
				offLED(LED7_GPIO,LED7_PIN);
				Delay_ms(100);
			}
			tingche();
			OLED_ShowString(1,1,"                ");
			OLED_ShowString(1,1,"TEST FINISHED");
		}
		
		if(Key_checkFlag(KEY_DOUBLE)){//双击状态相应
			houtui();
			OLED_ShowString(1,1,"                ");
			OLED_ShowString(1,1,"BACK TEST");
		}
		
		if(Key_checkFlag(KEY_LONG)){//长按状态相应
			qianjin();
			OLED_ShowString(1,1,"                ");
			OLED_ShowString(1,1,"GO TEST");
		}

		//dma搬运adc数据
		huidu1 = AD_Value[0];
		huidu2 = AD_Value[1];
		huidu3 = AD_Value[2];
		
		OLED_ShowNum(2,8,huidu1,4);
		OLED_ShowNum(3,8,huidu2,4);
		OLED_ShowNum(4,8,huidu3,4);
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
