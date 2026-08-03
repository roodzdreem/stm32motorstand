#ifndef USART_H
#define USART_H

#include <stddef.h>
#include "FreeRTOS.h"
#include "queue.h"

void USART3_Init(void);
QueueHandle_t USART3_GetRxQueue(void);
void USART3_Write(const char *pcString, size_t xLen);

#endif /* USART_H */