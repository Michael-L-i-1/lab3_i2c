#ifndef LAB3_I2C_H
#define LAB3_I2C_H

#include "stm32l4xx_hal.h"

// LSM303AGR I2C addresses (HAL wants them shifted left by 1)
#define ACCEL_ADDR (0x19 << 1)
#define MAG_ADDR (0x1E << 1)

// LSM303AGR registers
#define WHO_AM_I_A 0x0F // should read 0x33
#define WHO_AM_I_M 0x4F // should read 0x40

extern I2C_HandleTypeDef hi2c1;

void i2c_init(void);
void run_part3(void);

#endif
