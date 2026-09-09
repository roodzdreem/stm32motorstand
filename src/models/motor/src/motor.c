#include "motor.h"

#include <stdio.h>

// полушаговая последовательность, бит0 = IN1 (PF12) ... бит3 = IN4 (PF15)
static const uint8_t half_step[8] =
    {0b0001, 0b0011, 0b0010, 0b0110, 0b0100, 0b1100, 0b1000, 0b1001};
static const uint8_t full_step[4] = {0b0001, 0b0010, 0b0100, 0b1000};

void        motor_task(void* pvParameters);
static void motor_write(uint8_t phase);

void motor_init(void)
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

    // if (pdPASS != xTaskCreate(motor_task, "motor", 512, NULL, 1, NULL))
    // {
    //     while (1)
    //     {
    //         vTaskDelay(1);
    //     }
    // }
}

void motor_task(void* pvParameters)
{
    uint8_t idx = 0;
    for (;;)
    {
        vTaskDelay(pdMS_TO_TICKS(3));
        motor_write(half_step[(idx++) % 8]);
    }
}

static void motor_write(uint8_t phase)
{
    uint32_t set = 0;
    if (phase & 0x1u)
        set |= IN1_PIN;
    if (phase & 0x2u)
        set |= IN2_PIN;
    if (phase & 0x4u)
        set |= IN3_PIN;
    if (phase & 0x8u)
        set |= IN4_PIN;

    uint32_t clr = MOTOR_PIN_MASK & ~set;    // всё, что не set — выключить
    MOTOR_GPIO->BSRR =
        set | (clr << 16);    // set в младших 16, reset в старших — одной записью
}
