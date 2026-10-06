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

#define PHASE_PINS(b)                                                \
    ((((b) & 0x1u) ? IN1_PIN : 0u) | (((b) & 0x2u) ? IN2_PIN : 0u) | \
     (((b) & 0x4u) ? IN3_PIN : 0u) | (((b) & 0x8u) ? IN4_PIN : 0u))

// Готовое значение для BSRR: включить нужные, выключить остальные
#define PHASE_BSRR(b) \
    (PHASE_PINS(b) | ((uint32_t)(MOTOR_PIN_MASK & ~PHASE_PINS(b)) << 16))

typedef enum
{
    MODE_OFF,
    MODE_FULL,
    MODE_HALF,
    MODE_COUNT,
} motor_mode_e;

typedef enum
{
    MOTOR_DIR_CW  = 1,
    MOTOR_DIR_CCW = -1,
} motor_direction_e;

typedef struct
{
    uint8_t           step_idx;
    motor_direction_e direction;
    uint32_t          step_delay_ms;
    motor_direction_e motor_mode;
    uint8_t           is_stepping;
} motor_instance_t;


static const uint32_t half_step[8] = {
    PHASE_BSRR(0b0001),
    PHASE_BSRR(0b0011),
    PHASE_BSRR(0b0010),
    PHASE_BSRR(0b0110),
    PHASE_BSRR(0b0100),
    PHASE_BSRR(0b1100),
    PHASE_BSRR(0b1000),
    PHASE_BSRR(0b1001),
};

static const uint32_t full_step[4] = {
    PHASE_BSRR(0b0001),
    PHASE_BSRR(0b0010),
    PHASE_BSRR(0b0100),
    PHASE_BSRR(0b1000),
};

void motor_init(void);
void cli_set_pin(int8_t id, int8_t state);
void do_step(motor_instance_t* motor);
void start_stepping(void);
void stop_stepping(void);


#endif /* MOTOR_H */