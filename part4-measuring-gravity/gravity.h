#ifndef LAB3_GRAVITY_H
#define LAB3_GRAVITY_H

// LSM303AGR accelerometer registers
#define CTRL_REG1_A 0x20
#define OUT_X_L_A 0x28
#define READ_MASK 0x80 // OR with a register address to read several registers in a row

#define ACCEL_ON 0x57 // CTRL_REG1_A: 100 Hz, normal mode, X/Y/Z enabled
#define COUNTS_PER_G 16384.0f // 16 bits and twos complement --> 65536. Default is +-2g, so per g is 16384

void run_part4(void);

#endif
