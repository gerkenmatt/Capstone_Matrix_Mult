/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
//#include <stdlib.h>
//#include <string.h>
#include <stdarg.h>

/* Defines -------- put into header -----------------------------------------*/
#define     side                12
#define     robust_printing     1
#define     change_rate         500

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;

/*matrix mult variables*/
int first_matrix[side][side];
int second_matrix[side][side];
unsigned long results_matrix[side][side];
unsigned long golden_matrix[side][side];

unsigned long int ind = 0;
int local_errors = 0;
int in_block = 0;
int seed_value = -1;


/*for tiny xprintf*/
static const unsigned long dv[] = {
//  4294967296      // 32 bit unsigned max
   1000000000,     // +0
    100000000,     // +1
     10000000,     // +2
      1000000,     // +3
       100000,     // +4
//       65535      // 16 bit unsigned max
        10000,     // +5
         1000,     // +6
          100,     // +7
           10,     // +8
            1,     // +9
};


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void sendByte(char c);
static void sendString(char * c);
void iputc(char b);
int iputs(const char * s);
static void xtoa(unsigned long x, const unsigned long *dp);
static void xprintf(char *format, ...);
static void iputh(unsigned n);



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

/************put into own source file*/
static void xtoa(unsigned long x, const unsigned long *dp)
{
   char c;
   unsigned long d;
   if(x)
   {
       while(x < *dp) ++dp;
       do {
           d = *dp++;
           c = '0';
           while(x >= d) ++c, x -= d;
           iputc(c);
       } while(!(d & 1));
   }
   else
      iputc('0');
}

static void iputh(unsigned n)
{
   static const char hex[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
   iputc(hex[n & 15]);
}

static void xprintf(char *format, ...)
{
   char c;
   int i;
   long n;

   va_list a;
   va_start(a, format);
//   iputs("format: ");
//   iputs(format);
   while(c = *format++) {
       if(c == '%') {
           switch(c = *format++) {
               case 's':                       // String
                   iputs(va_arg(a, char*));
                   break;
               case 'c':                       // Char
                   iputc(va_arg(a, char));
                   break;
               case 'i':                       // 16 bit Integer
               case 'u':                       // 16 bit Unsigned
                   i = va_arg(a, int);
                   if(c == 'i' && i < 0) i = -i, iputc('-');
                   xtoa((unsigned)i, dv + 5);
                   break;
               case 'l':                       // 32 bit Long
               case 'n':                       // 32 bit uNsigned loNg
                   n = va_arg(a, long);
                   if(c == 'l' &&  n < 0) n = -n, iputc('-');
                   xtoa((unsigned long)n, dv);
                   break;
               case 'x':                       // 16 bit heXadecimal
                   i = va_arg(a, int);
                   iputh(i >> 12);
                   iputh(i >> 8);
                   iputh(i >> 4);
                   iputh(i);
                   break;
               case 0: return;
               default: goto bad_fmt;
           }
       } else
    bad_fmt:    iputc(c);
   }
   va_end(a);
}

void init_matrices() {
	int i = 0;
	int j = 0;

	//seed the random number generator
	//the method is designed to reset SEUs in the matrices, using the current seed value
	//that way each test starts error free
	if (seed_value == -1)
	{
		srand(ind);
		seed_value = ind;
	}
	else
	{
		srand(seed_value);
	}

	//fill the matrices
	for ( i = 0; i < side; i++ )
	{
		for (j = 0; j < side; j++)
		{
			first_matrix[i][j] = rand();
			second_matrix[i][j] = rand();
		}
	}
}

void matrix_multiply(int f_matrix[][side], int s_matrix[][side], unsigned long r_matrix[][side])
{
	int i = 0;
	int j = 0;
	int k = 0;
	unsigned long sum = 0;

	//MM
	for ( i = 0 ; i < side ; i++ )
	{
		for ( j = 0 ; j < side ; j++ )
		{
			for ( k = 0 ; k < side ; k++ )
			{
				sum = sum + f_matrix[i][k]*s_matrix[k][j];
			}
			r_matrix[i][j] = sum;
			sum = 0;
		}
	}
}

int checker(unsigned long golden_matrix[][side], unsigned long results_matrix[][side])
{
	int first_error = 0;
	int num_of_errors = 0;
	int i = 0;
	int j = 0;

	for(i=0; i<side; i++)
	{
		for (j = 0; j < side; j++)
		{
			if (golden_matrix[i][j] != results_matrix[i][j])
			{
				//checker found an error, print results to screen
				if (!first_error)
				{
					if (!in_block && robust_printing)
					{
						xprintf(" - i: %n\n\r\n", ind);
						xprintf("   E: {%i_%i: [%x, %x],", i, j, golden_matrix[i][j], results_matrix[i][j]);
						first_error = 1;
						in_block = 1;
					}
					else if (in_block && robust_printing)
					{
						xprintf("   E: {%i_%i: [%x, %x],", i, j, golden_matrix[i][j], results_matrix[i][j]);
						first_error = 1;
					}
				}
				else
				{
					if (robust_printing)
						xprintf("%i_%i: [%x, %x],", i, j, golden_matrix[i][j], results_matrix[i][j]);
				}
				num_of_errors++;
			}
		}
	}

	if (first_error)
	{
		xprintf("}\n\r\n");
		first_error = 0;
	}

	if (!robust_printing && (num_of_errors > 0))
	{
		if (!in_block)
		{
			xprintf(" - i: %n\n\r\n", ind);
			xprintf("   E: %i\n\r\n", num_of_errors);
			in_block = 1;
		}
		else
		{
			xprintf("   E: %i\n\r\n", num_of_errors);
		}
	}

	return num_of_errors;
}

void matrix_multiply_test()
{

	//initialize variables
	int total_errors = 0;

	init_matrices();
	//setup golden values
	matrix_multiply(first_matrix, second_matrix, golden_matrix);

	while (1)
	{
		matrix_multiply(first_matrix, second_matrix, results_matrix);
		local_errors = checker(golden_matrix, results_matrix);

		//if there is an error, fix the input matrics
		//golden is recomputed so that the code doesn't
		//have to figure out if the error was in the results
		//or golden matrix
		if (local_errors > 0)
		{
			init_matrices();
			matrix_multiply(first_matrix, second_matrix, golden_matrix);
		}

		//acking to see if alive, as well as changing input values
		if (ind % change_rate == 0)
		{
			xprintf("# %n, %i\n\r\n", ind, total_errors);
			seed_value = -1;
			init_matrices();
			//have to recompute the golden
			matrix_multiply(first_matrix, second_matrix, golden_matrix);
		}

		//reset vars and such
		ind++;
		total_errors += local_errors;
		local_errors = 0;
		in_block = 0;
	}

}
int iputs(const char * s)
{
	char c;
	while (c = *s++)
	{
		sendByte(c);
	}
	return 1;
}

void iputc(char b)
{
	sendByte(b);
}

static void sendString(char * str)
{
	char c;
	while (c = *str++)
	{
		sendByte(c);
	}
}



static void sendByte(char c)
{
	int uart_status = HAL_UART_Transmit(&huart1, &c, 1, 1000);
	if (uart_status != HAL_OK) //when it first goes wrong, we get HAL_BUSY
	   HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_SET); //this is where it goes wrong
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

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

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

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
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

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
