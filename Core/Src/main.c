/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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

#include <stdlib.h>

#include "gpio.h"
#include "../../Drivers/SYSTEM/delay/delay.h"
#include "../../Drivers/SYSTEM/led/led.h"
#include "../../Drivers/SYSTEM/key/key.h"
#include "../../Drivers/SYSTEM/exti/exti.h"
#include "../../Drivers/SYSTEM/usart/usart2.h"
#include "../../Drivers/SYSTEM/usart/retarget.h"
#include "../../Drivers/SYSTEM/wdg/wdg.h"
#include "../../Drivers/SYSTEM/wdg/wwdg.h"
#include "../../Drivers/SYSTEM/tim/btim.h"
#include "../../Drivers/SYSTEM/tim/gtim_pwm.h"
#include "../../Drivers/SYSTEM/tim/gtim_ic.h"
#include "../../Drivers/SYSTEM/tim/gtim_ecm1.h"
#include "../../Drivers/SYSTEM/tim/atim_rc.h"
#include "../../Drivers/SYSTEM/tim/atim_pwm.h"
#include "../../Drivers/SYSTEM/tim/atim_pwm_survey.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
uint16_t g_timxchy_pwmin_sta=0;    /* PWM输入状态 */
uint32_t g_timxchy_pwmin_hval=0;   /* PWM的高电平脉宽 */
uint32_t g_timxchy_pwmin_cval=0;   /* PWM的周期宽度 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();
  //sys_stm32_clock_init(RCC_PLL_MUL9);     //和SystemClock_Config()一样
  delay_init(72);
  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  // MX_GPIO_Init();
  led_init();
  // key_init();
  // exti_init();
  usart_init(115200);
  RetargetInit(&g_huart); //初始化printf
  // wdg_init(IWDG_PRESCALER_32, 0x0FFF);    //选32分频，装载值4096；超时时间约3.276s
  // wwdg_init(0x7F, 0x5f);
  // btim_init(12000,12000);
  gtim_pwm_init(71,9);  //产生PWM到TIM3_CH2，PB5

  // gtim_ic_init(71, 65535);
  // gtim_ecm1_init(0,65535);
    // atim_rc_init(7199,4999);
  // atim_pwm_chy_init(71,999);
  // atim_pwm_chy_init(1000,65535);
  atim_pwm_survey_init();   //TIM8_CH1（PC6）
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  // atim_timx_cplm_pwm_set(700,100);
  // atim_timx_cplm_pwm_set(45874,0);
  double tpsc = 1.0 / 72;
  while (1){
    if (g_timxchy_pwmin_sta) {
      printf("--begin--\r\n");
      printf("PWM Hight:%d\r\n", g_timxchy_pwmin_hval);   /* CCR2 */
      printf("PWM Cycle:%d\r\n", g_timxchy_pwmin_cval);   /* CCR1 */
      printf("PWM Hight time:%.3fus\r\n", g_timxchy_pwmin_hval * tpsc);            /* 打印高电平脉宽长度 */
      printf("PWM Cycle time:%.3fus\r\n", g_timxchy_pwmin_cval * tpsc);            /* 打印周期时间长度 */
      printf("--end--\r\n");
      atim_timx_pwmin_chy_restart(); /* 重启PWM输入检测 */
    }
      //TIM3->CCR2=5; //设置重装载值，以调整占空比
    delay_ms(2000);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  HAL_StatusTypeDef ret = HAL_ERROR;
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};   //设置其变量初始值都为0；不设初始值的话会是随机数
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;    //要配置的时钟：HSE
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;   //HSE预分频：一分频
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;      //PLL打开
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;  //PLL时钟源：HSE
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;    //PLL倍频系数=9
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)    //最后初始化
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;   //要配置这四个时钟
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;   //sysclk来自PLL
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;    //AHB分频：一分频
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;     //APB1
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;     //APB2

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)   //FLASH延迟周期数：2
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
#ifdef USE_FULL_ASSERT
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
