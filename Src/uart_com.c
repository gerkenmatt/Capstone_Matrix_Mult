/*
 * uart_com.c
 *
 *  Created on: Jan 12, 2018
 *      Author: gerke
 */
#include "uart_com.h"
#include "stm32f4xx_hal.h"
#include <stdarg.h>

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;

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

/* USART1 init function */
void MX_USART1_UART_Init(void)
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

void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}


void xtoa(unsigned long x, const unsigned long *dp)
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

void iputh(unsigned n)
{
   static const char hex[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
   iputc(hex[n & 15]);
}

void xprintf(char *format, ...)
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
void sendByte(char c)
{
	int uart_status = HAL_UART_Transmit(&huart1, &c, 1, 1000);
	if (uart_status != HAL_OK) //when it first goes wrong, we get HAL_BUSY
	   HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_SET); //this is where it goes wrong
}

