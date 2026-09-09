#ifndef MOTOR_H
#define MOTOR_H

#include "FreeRTOS.h"
#include "stdint.h"
#include "stm32f7xx.h"
#include "stm32f7xx_ll_bus.h"
#include "stm32f7xx_ll_gpio.h"
#include "task.h"

#define MOTOR_GPIO     GPIOF
#define IN1_PIN        LL_GPIO_PIN_15    //D2
#define IN2_PIN        LL_GPIO_PIN_14    //D4
#define IN3_PIN        LL_GPIO_PIN_13    //D7
#define IN4_PIN        LL_GPIO_PIN_12    //D8
#define MOTOR_PIN_MASK (LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15)

typedef struct
{
    uint8_t  phase;
    int8_t   dir;
    uint32_t step_delay_ms;
} Motor;

void motor_init(void);
void cli_set_pin(int8_t id, int8_t state);


#endif /* MOTOR_H */