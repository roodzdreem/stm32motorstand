#ifndef CLI_TASK_H
#define CLI_TASK_H
#include "FreeRTOS.h"
#include "FreeRTOS_CLI.h"
#include "queue.h"
#include "task.h"
#include "usart.h"
void       vCliTaskStart(void);
BaseType_t CLI_motorstand_init(void);

#endif /* CLI_TASK_H */
