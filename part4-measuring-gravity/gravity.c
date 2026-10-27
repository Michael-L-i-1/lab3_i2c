#include <stdio.h>
#include "gravity.h"
#include "../part3-writing-the-driver/i2c.h"

UART_HandleTypeDef hlpuart1;

// UART to print to the Mac's serial terminal (not the sensor; that's I2C)
void uart_init(void)
{
    // PG7 = TX, PG8 = RX: LPUART1, wired on the board to the ST-Link USB port
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWREx_EnableVddIO2(); // port G needs its own power supply turned on
    __HAL_RCC_GPIOG_CLK_ENABLE();
    GPIO_InitTypeDef gpio = {0};
    gpio.Pin = GPIO_PIN_7 | GPIO_PIN_8;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Alternate = GPIO_AF8_LPUART1;
    HAL_GPIO_Init(GPIOG, &gpio);

    // 115200 baud, 8 data bits, no parity, 1 stop bit
    __HAL_RCC_LPUART1_CLK_ENABLE();
    hlpuart1.Instance = LPUART1;
    hlpuart1.Init.BaudRate = 115200;
    hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
    hlpuart1.Init.StopBits = UART_STOPBITS_1;
    hlpuart1.Init.Parity = UART_PARITY_NONE;
    hlpuart1.Init.Mode = UART_MODE_TX_RX;
    HAL_UART_Init(&hlpuart1);
}

// printf sends each character here, and it goes out over the UART to the Mac
int __io_putchar(int ch)
{
    HAL_UART_Transmit(&hlpuart1, (uint8_t *)&ch, 1, 10);
    return ch;
}

void run_part4(void)
{
    uint8_t ctrl_reg1 = ACCEL_ON;
    uint8_t data[6];

    i2c_init();
    uart_init();

    // enable acceleration measurement
    HAL_I2C_Mem_Write(&hi2c1, ACCEL_ADDR, CTRL_REG1_A, I2C_MEMADD_SIZE_8BIT, &ctrl_reg1, 1, HAL_MAX_DELAY);

    while (1)
    {
        HAL_I2C_Mem_Read(&hi2c1,
                         ACCEL_ADDR,                  // Device address
                         OUT_X_L_A | READ_MASK,       // First output register
                         I2C_MEMADD_SIZE_8BIT,        // 1-byte register address
                         data,                        // Output buffer
                         6,                           // Number of bytes (X, Y, Z)
                         HAL_MAX_DELAY);              // Timeout

        // combine the data bytes for each coordinate (since 2 bytes per)
        int16_t x = (int16_t)(data[1] << 8 | data[0]);
        int16_t y = (int16_t)(data[3] << 8 | data[2]);
        int16_t z = (int16_t)(data[5] << 8 | data[4]);

        // convert raw counts to g
        float gx = x / COUNTS_PER_G;
        float gy = y / COUNTS_PER_G;
        float gz = z / COUNTS_PER_G;

        printf("X: %d (%.2f g)  Y: %d (%.2f g)  Z: %d (%.2f g)\r\n", x, gx, y, gy, z, gz);
        HAL_Delay(200);
    }
}
