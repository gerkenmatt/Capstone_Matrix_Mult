
#include "main.h"
#include "stm32f4xx_hal.h"
#include "uart_com.h"
#include "matrix_mult.h"


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

int main(void)
{

  /* USER CODE BEGIN 1 */
    uint8_t tx_sel = 1;
    uint8_t error_count = 0;
    uint8_t data_sent = 2;
    uint8_t data_rec = 4;
    uint8_t hello[] = "Hello World\n";
    HAL_StatusTypeDef uart_status = HAL_BUSY;
    char ch = 'a';
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_USART1_UART_Init();

  /* USER CODE BEGIN 2 */
  //HAL_UART_Transmit(&huart1, 0xFF, 1, 1000); //idle bit transmission (TEST WITH THIS)
  /* USER CODE END 2 */

  /* Infinite loop */
  //print the YAML header
   xprintf("\n\r\n---\n\r\n");
   xprintf("hw: msp430f2619\n\r\n");
   xprintf("test: MM\n\r\n");
   xprintf("mit: none\n\r\n");
   xprintf("printing: %i\n\r\n", robust_printing);
   xprintf("input change rate: %i\n\r\n", change_rate);
   xprintf("Side matrix size: %i\n\r\n", side);
   xprintf("ver: 0.1\n\r\n");
   xprintf("fac: LANSCE Nov 2015\n\r\n");
   xprintf("d:\n\r\n");

   //start test
   matrix_multiply_test();

  iputs("testing\n\r\n");
  iputs("fuckkkkkkk");

  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
      if (tx_sel == 1)
      {
//    	  sendString("hello wurld\n\r\n");
//    	  sendByte(++ch);
//    	  sendString("\n\r\n");
    	  iputs("testing\n\r\n");
    	  xprintf("hollo werld\n\n\r\n");
    	  xprintf("integer example: %i\n\r\n", 55);

      }

      HAL_Delay(500); //1 sec delay

      if (data_rec & 0x01) //check for error
    	  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_SET);
      else
    	  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET);

      HAL_Delay(100);
      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);
      data_sent++;
  }
  /* USER CODE END 3 */

}


/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);

  /*Configure GPIO pins : PG13 PG14 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: xprintf("Wrong parameters value: file %s on line %d\n\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
