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
#include "gpio.h"
#include "../../Drivers/SYSTEM/delay/delay.h"
#include "../../Drivers/SYSTEM/led/led.h"
#include "../../Drivers/SYSTEM/key/key.h"
#include "../../Drivers/SYSTEM/usart/usart2.h"
#include "../../Drivers/SYSTEM/usart/retarget.h"
#include "../../Drivers/SYSTEM/tpad/tpad.h"
#include "../../Drivers/SYSTEM/fsmc/fsmc_lcd.h"
#include "../../Drivers/SYSTEM/fsmc/pack/util.h"

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
  //sys_stm32_clock_init(RCC_PLL_MUL9);     //和SystemClock_Config()一样
  delay_init(72);
  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  // MX_GPIO_Init();
  led_init();
  usart_init(115200);
  RetargetInit(&g_huart); //初始化printf
  // key_init();
  // tpad_init();
  fsmc_lcd_init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  lcd_draw_point(0,0,RED);
  lcd_draw_point(0,1,RED);
  lcd_draw_point(100,100,CYAN);

  printf("color is : %#x\r\n",lcd_read_point(100,100));
  uint8_t i = 0;
  while (1) {
    /* USER CODE END WHILE */
    switch (i) {
      case 0:
        lcd_clear(WHITE);
        break;

      case 1:
        lcd_clear(BLACK);
        break;

      case 2:
        lcd_clear(BLUE);
        break;

      case 3:
        lcd_clear(RED);
        break;

      case 4:
        lcd_clear(MAGENTA);
        break;

      case 5:
        lcd_clear(GREEN);
        break;
    }

    // lcd_show_string(10, 40, 240, 32, 32, "STM32", RED);
    // lcd_show_string(10, 80, 240, 24, 24, "TFTLCD TEST", RED);
    // lcd_show_string(10, 110, 240, 16, 16, "ATOM@ALIENTEK", RED);
    // lcd_show_string(10, 130, 240, 16, 16, (char *)lcd_id, RED); /* 显示LCD ID */
    i++;
    if (i == 6) i = 0;

    delay_ms(500);
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
