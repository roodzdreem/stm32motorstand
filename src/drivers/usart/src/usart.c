#include "usart.h"

#include "stm32f7xx.h"
#include "stm32f7xx_ll_bus.h"
#include "stm32f7xx_ll_gpio.h"
#include "stm32f7xx_ll_usart.h"

static QueueHandle_t xRxQueue;

void USART3_Init(void)
{
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin                 = LL_GPIO_PIN_8 | LL_GPIO_PIN_9;
    GPIO_InitStruct.Mode                = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed               = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType          = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull                = LL_GPIO_PULL_UP;
    GPIO_InitStruct.Alternate           = LL_GPIO_AF_7;
    LL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    LL_USART_InitTypeDef USART_InitStruct = {0};
    USART_InitStruct.BaudRate             = 115200;
    USART_InitStruct.DataWidth            = LL_USART_DATAWIDTH_8B;
    USART_InitStruct.StopBits             = LL_USART_STOPBITS_1;
    USART_InitStruct.Parity               = LL_USART_PARITY_NONE;
    USART_InitStruct.TransferDirection    = LL_USART_DIRECTION_TX_RX;
    USART_InitStruct.HardwareFlowControl  = LL_USART_HWCONTROL_NONE;
    USART_InitStruct.OverSampling         = LL_USART_OVERSAMPLING_16;
    LL_USART_Init(USART3, &USART_InitStruct);
    LL_USART_ConfigAsyncMode(USART3);
    LL_USART_Enable(USART3);

    xRxQueue = xQueueCreate(64, sizeof(uint8_t));

    LL_USART_EnableIT_RXNE(USART3);
    NVIC_SetPriority(USART3_IRQn, 5);
    NVIC_EnableIRQ(USART3_IRQn);
}

QueueHandle_t USART3_GetRxQueue(void)
{
    return xRxQueue;
}

void USART3_Write(const char* pcString, size_t xLen)
{
    for (size_t i = 0; i < xLen; i++)
    {
        while (!LL_USART_IsActiveFlag_TXE(USART3))
        {
        }
        LL_USART_TransmitData8(USART3, (uint8_t)pcString[i]);
    }
    while (!LL_USART_IsActiveFlag_TC(USART3))
    {
    }
}

void USART3_IRQHandler(void)
{
    if (LL_USART_IsActiveFlag_ORE(USART3))
    {
        LL_USART_ClearFlag_ORE(USART3);
    }

    if (LL_USART_IsActiveFlag_RXNE(USART3))
    {
        uint8_t    cByte                    = LL_USART_ReceiveData8(USART3);
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xQueueSendFromISR(xRxQueue, &cByte, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

int __io_putchar(int ch)
{
    char symbol = (char)ch;
    USART3_Write(&symbol, 1);
    return ch;
}