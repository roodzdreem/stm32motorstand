#include "main.h"
#include "stdio.h"
#include "stm32f7xx.h"
#include "stm32f7xx_ll_bus.h"
#include "stm32f7xx_ll_gpio.h"
#include "stm32f7xx_ll_utils.h"
#include "FreeRTOS.h"
#include "task.h"

static inline void print_info()
{
    printf("ООО \"ЭМ Рус\", Плата:\r\n");
    printf("[Отладочная сборка]\r\n");
}

void hb_task(void *pvParameters)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin        = LL_GPIO_PIN_0;
    GPIO_InitStruct.Mode       = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed      = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull       = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    for (;;)
    {
        LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_0);
        vTaskDelay(450);
        LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_0);
        vTaskDelay(450);
    }
}


int main(void)
{
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
    // BaseType_t res = xTaskCreate(hb_task, "hb", 128, 0, 1, NULL);
    // if (res != pdPASS)
    // {
        LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Pin        = LL_GPIO_PIN_0;
        GPIO_InitStruct.Mode       = LL_GPIO_MODE_OUTPUT;
        GPIO_InitStruct.Speed      = LL_GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
        GPIO_InitStruct.Pull       = LL_GPIO_PULL_NO;
        LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_0);

    // }
    // vTaskStartScheduler();


    while (1)
    {
    }
}
