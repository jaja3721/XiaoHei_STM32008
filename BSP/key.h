#ifndef KEY_H
#define KEY_H

#include "stm32f4xx.h"
void KEY_init(void);
void EXTI9_5_IRQHandler(void);
void EXTI15_10_IRQHandler(void);
uint8_t get_mode(void);
uint8_t get_PV(void);
#endif
