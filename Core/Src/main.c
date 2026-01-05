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
#include "string.h"
#include "../../Drivers/SYSTEM/delay/delay.h"
#include "../../Drivers/SYSTEM/led/led.h"
#include "../../Drivers/SYSTEM/key/key.h"
#include "../../Drivers/SYSTEM/usart/usart2.h"
#include "../../Drivers/SYSTEM/usart/retarget.h"
#include "../../Drivers/SYSTEM/tpad/tpad.h"
#include "../../Drivers/SYSTEM/fsmc/fsmc_lcd.h"
#include "../../Drivers/SYSTEM/fsmc/pack/util.h"
#include "../../Drivers/SYSTEM/rtc/rtc.h"
#include "../../Drivers/SYSTEM/usmart/usmart.h"
#include "../../Drivers/SYSTEM/pwr/pwrlow.h"
#include "../../Drivers/SYSTEM/pwr/pvd.h"
#include "../../Drivers/SYSTEM/adc/adc1.h"

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
  // led_init();
  usart_init(115200);
  RetargetInit(&g_huart); //初始化printf
  // key_init();
  // tpad_init();
  fsmc_lcd_init();
  // rtc_deinit();
  // usmart_dev.init(72);  //usmart
  // wkup_init();
  // pvd_init();
  // dma2_init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  const uint16_t BUFFER_SIZE = 10 * 6;
  uint16_t buffer[BUFFER_SIZE];
  adc1_init();
  adc_dma_init((uint32_t)&buffer);  //&符号应该是可加可不加，因为数组形参就看做是首地址
  // adc_dma_enable(BUFFER_SIZE);  //
  uint16_t i,j;
  uint16_t adcx;
  uint32_t sum;
  float temp;
    lcd_show_string(30,  50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30,  70, 200, 16, 16, "ADC 6CH DMA TEST", RED);
    lcd_show_string(30,  90, 200, 16, 16, "ATOM@ALIENTEK", RED);

    lcd_show_string(30, 110, 200, 12, 12, "ADC1_CH0_VAL:", BLUE);
    lcd_show_string(30, 122, 200, 12, 12, "ADC1_CH0_VOL:0.000V", BLUE); /* 先在固定位置显示小数点 */

    lcd_show_string(30, 140, 200, 12, 12, "ADC1_CH1_VAL:", BLUE);
    lcd_show_string(30, 152, 200, 12, 12, "ADC1_CH1_VOL:0.000V", BLUE); /* 先在固定位置显示小数点 */

    lcd_show_string(30, 170, 200, 12, 12, "ADC1_CH2_VAL:", BLUE);
    lcd_show_string(30, 182, 200, 12, 12, "ADC1_CH2_VOL:0.000V", BLUE); /* 先在固定位置显示小数点 */

    lcd_show_string(30, 200, 200, 12, 12, "ADC1_CH3_VAL:", BLUE);
    lcd_show_string(30, 212, 200, 12, 12, "ADC1_CH3_VOL:0.000V", BLUE); /* 先在固定位置显示小数点 */

    lcd_show_string(30, 230, 200, 12, 12, "ADC1_CH4_VAL:", BLUE);
    lcd_show_string(30, 242, 200, 12, 12, "ADC1_CH4_VOL:0.000V", BLUE); /* 先在固定位置显示小数点 */

    lcd_show_string(30, 260, 200, 12, 12, "ADC1_CH5_VAL:", BLUE);
    lcd_show_string(30, 272, 200, 12, 12, "ADC1_CH5_VOL:0.000V", BLUE); /* 先在固定位置显示小数点 */

    adc_dma_enable(BUFFER_SIZE);   /* 启动ADC DMA采集 */

    while (1) {
        if (g_adc_dma_flag == 1)
        {
            /* 循环显示通道0~通道5的结果 */
            for(j = 0; j < 6; j++)  /* 遍历6个通道 */
            {
                sum = 0; /* 清零 */
                for (i = 0; i < BUFFER_SIZE / 6; i++)  /* 每个通道采集了10次数据,进行10次累加 */
                {
                    sum += buffer[(6 * i) + j];      /* 相同通道的转换数据累加 */
                }
                adcx = sum / (BUFFER_SIZE / 6);        /* 取平均值 */

                /* 显示结果 */
                lcd_show_xnum(108, 110 + (j * 30), adcx, 4, 12, 0, BLUE);   /* 显示ADC采样后的原始值 */

                temp = (float)adcx * (3.3 / 4096);  /* 获取计算后的带小数的实际电压值，比如3.1111 */
                adcx = temp;                        /* 赋值整数部分给adcx变量，因为adcx为u16整形 */
                lcd_show_xnum(108, 122 + (j * 30), adcx, 1, 12, 0, BLUE);   /* 显示电压值的整数部分，3.1111的话，这里就是显示3 */

                temp -= adcx;                       /* 把已经显示的整数部分去掉，留下小数部分，比如3.1111-3=0.1111 */
                temp *= 1000;                       /* 小数部分乘以1000，例如：0.1111就转换为111.1，相当于保留三位小数。 */
                lcd_show_xnum(120, 122 + (j * 30), temp, 3, 12, 0X80, BLUE);/* 显示小数部分（前面转换为了整形显示），这里显示的就是111. */
            }

            g_adc_dma_flag = 0;                      /* 清除DMA采集完成状态标志 */
            adc_dma_enable(BUFFER_SIZE);       /* 启动下一次ADC DMA采集 */
        }

        delay_ms(100);
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
