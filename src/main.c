#include "main.h"

#include "FreeRTOS.h"
#include "cli.h"
#include "stdio.h"
#include "stm32f7xx.h"
#include "stm32f7xx_ll_bus.h"
#include "stm32f7xx_ll_gpio.h"
#include "stm32f7xx_ll_utils.h"
#include "task.h"

static inline void print_info()
{
    printf("ООО \"ЭМ Рус\", Плата:\r\n");
    printf("[Отладочная сборка]\r\n");
}

extern void xPortSysTickHandler(void);

void SVC_Handler(void) __attribute__((naked));
void SVC_Handler(void)
{
    __asm volatile(".extern vPortSVCHandler \n"
                   "b vPortSVCHandler       \n");
}

void PendSV_Handler(void) __attribute__((naked));
void PendSV_Handler(void)
{
    __asm volatile(".extern xPortPendSVHandler \n"
                   "b xPortPendSVHandler       \n");
}

void SysTick_Handler(void)
{
    xPortSysTickHandler();
}

void hb_task(void* pvParameters)
{
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin                 = LL_GPIO_PIN_0;
    GPIO_InitStruct.Mode                = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed               = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType          = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull                = LL_GPIO_PULL_NO;
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
    USART3_Init();
    print_info();
    CLI_motorstand_init();
    xTaskCreate(hb_task, "hb", 128, 0, 0, NULL);

    vTaskStartScheduler();

    while (1)
    {
    }
}
