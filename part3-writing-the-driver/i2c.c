#include "i2c.h"

I2C_HandleTypeDef hi2c1;

// check these in the debugger
uint8_t who_am_i_a = 0;
uint8_t who_am_i_m = 0;

void i2c_init(void)
{
    // PB8 = SCL, PB9 = SDA
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef gpio = {0};
    gpio.Pin = GPIO_PIN_8 | GPIO_PIN_9;  // for SPA and SPC in data sheet
    gpio.Mode = GPIO_MODE_AF_OD;
    gpio.Pull = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &gpio);

    // I2C1 at 100 kHz
    __HAL_RCC_I2C1_CLK_ENABLE();
    hi2c1.Instance = I2C1;  // associated with PC8 and PC9
    hi2c1.Init.Timing = 0x00420F13;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    HAL_I2C_Init(&hi2c1);
}

void run_part3(void)
{
    i2c_init();

    while (1)
    {
        HAL_I2C_Mem_Read(&hi2c1,
                         (0x19 << 1),           // Device address
                         0x0F,                  // WHO_AM_I_A register
                         I2C_MEMADD_SIZE_8BIT,  // 1-byte register address
                         &who_am_i_a,           // Output buffer
                         1,                     // Number of bytes
                         HAL_MAX_DELAY);        // Timeout

        HAL_I2C_Mem_Read(&hi2c1,
                         (0x1E << 1),           // Device address
                         0x4F,                  // WHO_AM_I_M register
                         I2C_MEMADD_SIZE_8BIT,  // 1-byte register address
                         &who_am_i_m,           // Output buffer
                         1,                     // Number of bytes
                         HAL_MAX_DELAY);        // Timeout

        HAL_Delay(100);
    }
}
