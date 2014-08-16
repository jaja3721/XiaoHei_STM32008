
#ifndef IMU_ADIS16xxx_H
#define IMU_ADIS16xxx_H

#include "stm32f4xx.h"
extern volatile float roll,pitch,yaw;

void IMU_init(void);
void IMU_BE10(void);

#endif

