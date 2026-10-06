#include "motor.h"

#include <stdio.h>

// static uint8_t step_idx = 0;
#define ARRAY_LEN(a) (sizeof(a) / sizeof((a)[0]))

static motor_instance_t motor_instance = {
    .step_idx      = 0,
    .direction     = MOTOR_DIR_CW,
    .step_delay_ms = 3,
    .motor_mode    = MODE_HALF,
};

static void motor_task(void* pvParameters);
void        do_step(motor_instance_t* motor);
static void init_motor_gpio(void);

void motor_init(void)
{
    init_motor_gpio();

    if (pdPASS != xTaskCreate(motor_task, "motor", 512, NULL, 1, NULL))
    {
        configASSERT(0);
    }
}

static void motor_task(void* pvParameters)
{
    for (;;)
    {
        do_step(&motor_instance);    // & 7 == % 8 для степени двойки
        vTaskDelay(pdMS_TO_TICKS(motor_instance.step_delay_ms));
    }
}

void do_step(motor_instance_t* motor)
{
    motor->step_idx  = (motor->step_idx + motor->direction) & (ARRAY_LEN(half_step) - 1u);
    MOTOR_GPIO->BSRR = half_step[motor_instance.step_idx];
}

static void init_motor_gpio(void)
{
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin                 = MOTOR_PIN_MASK;
    GPIO_InitStruct.Mode                = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed               = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType          = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull                = LL_GPIO_PULL_NO;
    if (LL_GPIO_Init(MOTOR_GPIO, &GPIO_InitStruct) != SUCCESS)
    {
        printf("Motor GPIO init error\r\n");
    }
    LL_GPIO_ResetOutputPin(MOTOR_GPIO, MOTOR_PIN_MASK);
}