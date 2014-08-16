#ifndef __PWM_H
#define __PWM_H
#include "stm32f4xx.h"
#define motor_set_type 1486
#define motor_set_max 1700
#define motor_set_min 1300

#define servo_set_type 1480
#define servo_set_max 1900
#define servo_set_min 1150

#define yaw_set_type 1380
#define yaw_set_max 1980
#define yaw_set_min 960

#define pitch_set_type 1780
#define pitch_set_max 2070
#define pitch_set_min 1260

void PWM_init(void);
void PWM_servo_set(uint16_t pp);
void PWM_motor_set(uint16_t pp);
void PWM_yaw_set(uint16_t pp);
void PWM_pitch_set(uint16_t pp);

#endif

