#ifndef __IO_SPI_H__
#define __IO_SPI_H__

#include "stm32f4xx_hal.h"
#include <inttypes.h>

uint8_t hal_Spi2_ReadWriteByte(uint8_t txdata);
void Icm_Spi_ReadWriteNbytes(uint8_t* pBuffer, uint8_t len);

uint8_t icm42688_read_reg(uint8_t reg);
void icm42688_read_regs(uint8_t reg, uint8_t* buf, uint16_t len);
uint8_t icm42688_write_reg(uint8_t reg, uint8_t value);

#endif
