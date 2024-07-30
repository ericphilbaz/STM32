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
#include "lptim.h"
#include "usart.h"
#include "rtc.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

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
uint8_t rtc_flag=0;

//==================
void pwr_enter_sleep(void)
{

	
SystemClock_Config();
/* 执行 WFI 指令, 进入睡眠模式21ma */
//HAL_SuspendTick();	
//HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
	

//停止mode
//	HAL_SuspendTick(); /* 暂停滴答时钟，防止通过滴答时钟中断唤醒9ma */ 
//	/* 进入停止模式，设置稳压器为低功耗模式，等待中断唤醒 */ 
//	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
	__HAL_RCC_GPIOA_CLK_ENABLE();
	//__HAL_RCC_PWR_IS_CLK_ENABLED();
//待机mode 
	GPIO_InitTypeDef GPIO_InitStruct_A = {0};
	GPIO_InitTypeDef GPIO_InitStruct_B = {0};
  // 配置GPIOA的所有引脚为模拟输入模式
  GPIO_InitStruct_A.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_13|GPIO_PIN_14;
  GPIO_InitStruct_A.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct_A.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct_A.Speed=GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct_A);
	
	GPIO_InitStruct_B.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct_B.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct_B.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct_B.Speed=GPIO_SPEED_FREQ_LOW;
	
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct_B);
	//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_All,GPIO_PIN_RESET);
	
	//HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	//HAL_GPIO_WritePin(GPIOB,GPIO_PIN_All,GPIO_PIN_RESET);
	
	//HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_All,GPIO_PIN_RESET);

	//HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	//HAL_GPIO_WritePin(GPIOD,GPIO_PIN_All,GPIO_PIN_RESET);

	//HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	//HAL_GPIO_WritePin(GPIOE,GPIO_PIN_All,GPIO_PIN_RESET);
	//__HAL_TIM_CLEAR_FLAG(&htim4,TIM_FLAG_UPDATE);

	__HAL_RCC_PWR_CLK_ENABLE();  /* 使能电源时钟 */	
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1); /* 使能 KEY_UP 引脚的唤醒功能5ma */
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU); /* 需要清此标记，否则将保持唤醒状态*/
	//PWR_Config();
	HAL_PWR_EnterSTANDBYMode();  /* 进入待机模式 */
	__HAL_RCC_GPIOA_CLK_DISABLE();
	
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
  MX_LPUART1_UART_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_LPTIM1_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
	HAL_UART_Transmit(&hlpuart1,(uint8_t *)"LP_UART1 START!\n",13,0xff);
	HAL_UART_Transmit(&huart1,(uint8_t *)"UART1 START!\n",13,0xff);
	HAL_UART_Transmit(&huart2,(uint8_t *)"UART2 START!\n",13,0xff);
	
	HAL_UART_Receive_IT(&hlpuart1, (uint8_t *)&lpRxBuffer1, 1);	//使能中断
	HAL_UART_Receive_IT(&huart1, (uint8_t *)&RxBuffer1, 1);	//使能中断
	HAL_UART_Receive_IT(&huart2, (uint8_t *)&RxBuffer2, 1);	//使能中断
	
	uint8_t t=0;
	pwr_enter_sleep();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		HAL_Delay(10);
		//当接收的数据长度大于一个字符时，打印输出
		if(lpusart1_rcv_len>=1)
		{
			HAL_UART_Transmit(&huart1,(uint8_t *)lpusart1_rec_buffer,lpusart1_rcv_len,0xff);	//发送接收到的数据
			memset(lpusart1_rec_buffer,0,lpusart1_rcv_len);																		//清除接收的数据
			lpusart1_rcv_len = 0;																														//清空计数
			t+=1;
		}
		
		if(usart1_rcv_len>=1)
		{
			HAL_UART_Transmit(&hlpuart1,(uint8_t *)usart1_rec_buffer,usart1_rcv_len,0xff);	//发送接收到的数据
			memset(usart1_rec_buffer,0,usart1_rcv_len);																		//清除接收的数据
			usart1_rcv_len = 0;																														//清空计数
		}
		HAL_Delay(10);
		if(usart2_rcv_len>=1)
		{
			HAL_UART_Transmit(&huart1,(uint8_t *)usart2_rec_buffer,usart2_rcv_len,0xff);	//发送接收到的数据
			memset(usart2_rec_buffer,0,usart2_rcv_len);																		//清除接收的数据
			usart2_rcv_len = 0;																														//清空计数
		}
		if(t>3)
		{
			pwr_enter_sleep();
		}
		
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2
                              |RCC_PERIPHCLK_LPUART1|RCC_PERIPHCLK_RTC
                              |RCC_PERIPHCLK_LPTIM1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInit.LptimClockSelection = RCC_LPTIM1CLKSOURCE_PCLK;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
//重写RTC中断函数，用于定时唤醒休眠中的处理器

void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
	rtc_flag = 1;
}
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
