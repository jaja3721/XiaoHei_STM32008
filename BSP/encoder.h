#ifndef ENCODER_H
#define ENCODER_H
#include "stm32f4xx.h"

extern  volatile float velocity;//ʵ���ٶ�

void Encoder_init(void);
int16_t get_speed(void);
#endif
