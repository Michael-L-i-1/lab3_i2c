#include "stm32l4xx_hal.h"
#include "../part3-writing-the-driver/i2c.h"
#include "../part4-measuring-gravity/gravity.h"

int main(void)
{
    HAL_Init();

    run_part3();
//    run_part4();

    while (1)
    {
    }
}

// HAL_Delay counts these 1 ms ticks
void SysTick_Handler(void)
{
    HAL_IncTick();
}
