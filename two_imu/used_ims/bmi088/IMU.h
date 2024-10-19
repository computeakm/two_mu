#ifndef __BMI088_IMU 
#define __BMI088_IMU 

#include "main.h"

#include "Accel.h"
#include "Gyro.h"

void IMU_INIT(SPI_HandleTypeDef* spiHandler);

void IMU_SETUP_FOR_LOGGING(void);
void IMU_ENABLE_ALL(void);
int IMU_READY(void);

#endif
