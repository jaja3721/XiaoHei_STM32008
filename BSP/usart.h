#ifndef USART_H
#define USART_H
#include "stm32f4xx.h"
void usart_init(void);

void usart1_init(void);
void USART2_IRQHandler(void);

#endif
