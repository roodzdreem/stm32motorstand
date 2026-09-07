#include "motor.h"

#include "stm32f7xx.h"
#include "stm32f7xx_ll_bus.h"
#include "stm32f7xx_ll_gpio.h"

#define MOTOR_GPIO GPIOF
// D2 D4 D7 D8
#define MOTOR_PIN_MASK (LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15)


void MotorInit(void)
{
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin                 = MOTOR_PIN_MASK;
    GPIO_InitStruct.Mode                = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed               = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType          = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull                = LL_GPIO_PULL_NO;
    LL_GPIO_Init(MOTOR_GPIO, &GPIO_InitStruct);
    LL_GPIO_ResetOutputPin(MOTOR_GPIO, MOTOR_PIN_MASK);
}

void SetPin(int8_t id, int8_t state)
{
    if (id == 1)
    {
        if (state == 1)
            GPIOF->BSRR = LL_GPIO_PIN_15;
        else
            GPIOF->BSRR = (uint32_t)LL_GPIO_PIN_15 << 16;
    } else if (id == 2)
    {
        if (state == 1)
            GPIOF->BSRR = LL_GPIO_PIN_14;
        else
            GPIOF->BSRR = (uint32_t)LL_GPIO_PIN_14 << 16;
    } else if (id == 3)
    {
        if (state == 1)
            GPIOF->BSRR = LL_GPIO_PIN_13;
        else
            GPIOF->BSRR = (uint32_t)LL_GPIO_PIN_13 << 16;
    } else if (id == 4)
    {
        if (state == 1)
            GPIOF->BSRR = LL_GPIO_PIN_12;
        else
            GPIOF->BSRR = (uint32_t)LL_GPIO_PIN_12 << 16;
    }
}