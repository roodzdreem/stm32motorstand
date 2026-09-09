#include "cli.h"

#include "usart.h"

#include <stdio.h>
#include <string.h>

#define BACKSPACE 0x08
#define DEL       0x07F


void vCliTask(void* pvParameters);


void vCliTaskStart(void)
{
    xTaskCreate(vCliTask, "cli", 512, NULL, 1, NULL);
}


void vCliTask(void* pvParameters)
{
    size_t        xIndex = 0;
    char          output_buf[128];
    uint8_t       cRxByte   = 0;
    QueueHandle_t input_buf = USART3_GetRxQueue();
    static char   cOutputBuffer[configCOMMAND_INT_MAX_OUTPUT_SIZE];
    BaseType_t    xMore;

    for (;;)
    {
        if (xQueueReceive(input_buf, &cRxByte, portMAX_DELAY))
        {
            if (cRxByte == '\n' || cRxByte == '\r')
            {
                USART3_Write("\n\r", 2);
                if (xIndex > 0)
                {
                    output_buf[xIndex] = '\0';
                    xIndex             = 0;
                    do
                    {
                        xMore = FreeRTOS_CLIProcessCommand(output_buf,
                                                           cOutputBuffer,
                                                           sizeof(cOutputBuffer));

                        USART3_Write(cOutputBuffer, strlen(cOutputBuffer));
                    } while (xMore != pdFALSE);
                }

            } else if (cRxByte == BACKSPACE || cRxByte == DEL)
            {
                if (xIndex > 0)
                {
                    USART3_Write("\b \b", 3);
                    xIndex--;
                }
            } else

            {
                USART3_Write((char*)&cRxByte, sizeof(uint8_t));
                output_buf[xIndex++] = cRxByte;
            }
        } else
        {
        }
    }
}

BaseType_t is_param(const char* p, BaseType_t len, const char* kw)
{
    return p != NULL && (size_t)len == strlen(kw) && strncmp(p, kw, (size_t)len) == 0;
}