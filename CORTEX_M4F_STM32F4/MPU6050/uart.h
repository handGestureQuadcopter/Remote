#ifndef _MPU6050_uart_H
#define _MPU6050_uart_H

#define MAX_UART_INPUT 50

#include "misc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"

void uart1_peripheral_init();

//void USART1_IRQHandler();
void command_detect(char *str);
void USART1_puts(char* s);
void USART1_ReadLine();

#endif
