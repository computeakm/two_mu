#ifndef __IMU_IO_H 
#define __IMU_IO_H 
#include "main.h"
#include "stdio.h"
#include "spi.h"
#include "tim.h"
#include "i2c.h"

#include "lbsuanfa.h"
#include "tm_stm32_ahrs_imu.h"
#include "kalman_filter.h"


#include "icm42688.h"
#include "IMU.h"
#include "bmp390_task.h"

#include "method.h"


#define used_tim htim2
#define bim088_spi hspi2


#define ACCEL_CS_PORT GPIOA
#define ACCEL_CS_PIN  GPIO_PIN_1

#define GYRO_CS_PORT GPIOA
#define GYRO_CS_PIN  GPIO_PIN_0



#define icm_spi hspi1

#define ICM_PORT_SPIX_CS		 GPIOB
#define ICM_PIN_SPIX_CS	     GPIO_PIN_11




#define I2C_HANDLE	(hi2c1)
#define SPI_HANDLE	(hspi2)
#define UART_HANDLE	(huart1)




#endif
