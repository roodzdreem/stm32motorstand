#ifndef CLI_TASK_H
#define CLI_TASK_H
#include "FreeRTOS.h"
#include "FreeRTOS_CLI.h"
#include "queue.h"
#include "stdio.h"
#include "task.h"
#include "usart.h"

void       vCliTaskStart(void);
BaseType_t is_param(const char* p, BaseType_t len, const char* kw);

#endif /* CLI_TASK_H */
