/*
 * uart_com.h
 *
 *  Created on: Jan 12, 2018
 *      Author: gerke
 */

#ifndef UART_COM_H_
#define UART_COM_H_


void MX_USART1_UART_Init(void);
void sendString(char * c);
void iputc(char b);
int iputs(const char * s);
void xtoa(unsigned long x, const unsigned long *dp);
void xprintf(char *format, ...);
void iputh(unsigned n);

void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)


#endif /* UART_COM_H_ */
