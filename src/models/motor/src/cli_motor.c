#include "cli_motor.h"

#include "cli.h"
#include "string.h"

#include <stdlib.h>
#define MOTOR_USAGE                                          \
    "Использование:\r\n"                                     \
    "  motor set_pin <pin (1-4, 5 = все)> <state (0/1)>\r\n" \
    "  motor start\r\n"                                      \
    "  motor stop\r\n"
static BaseType_t CLI_motorstand(char*       pcWriteBuffer,
                                 size_t      xWriteBufferLen,
                                 const char* pcCommandString);

const CLI_Command_Definition_t xCLI_motor = {.pcCommand = "motor",
                                             .pcHelpString =
                                                 "motor - Управление мотором\r\n",
                                             .pxCommandInterpreter = CLI_motorstand,
                                             .cExpectedNumberOfParameters = -1};

BaseType_t CLI_motorstand_init()
{
    return (BaseType_t)FreeRTOS_CLIRegisterCommand(&xCLI_motor);
}
static BaseType_t CLI_motorstand(char*       pcWriteBuffer,
                                 size_t      xWriteBufferLen,
                                 const char* pcCommandString)
{
    BaseType_t  xParamLen1, xParamLen2, xParamLen3;
    const char* pcParam = FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParamLen1);

    pcWriteBuffer[0] = '\0';    // никакого "эха" прошлой команды

    if (is_param(pcParam, xParamLen1, "set_pin"))
    {
        const char* pin   = FreeRTOS_CLIGetParameter(pcCommandString, 2, &xParamLen2);
        const char* state = FreeRTOS_CLIGetParameter(pcCommandString, 3, &xParamLen3);
        if (pin == NULL || state == NULL)
        {
            snprintf(pcWriteBuffer,
                     xWriteBufferLen,
                     "ERR: нужны pin и state\r\n" MOTOR_USAGE);
            return pdFALSE;
        }

        char pin_buf[8]   = {0};
        char state_buf[8] = {0};
        memcpy(pin_buf, pin, xParamLen2 < 7 ? xParamLen2 : 7);
        memcpy(state_buf, state, xParamLen3 < 7 ? xParamLen3 : 7);

        cli_set_pin((int8_t)atoi(pin_buf), (int8_t)atoi(state_buf));
        snprintf(pcWriteBuffer,
                 xWriteBufferLen,
                 "OK: pin %s, state %s\r\n",
                 pin_buf,
                 state_buf);
    } else if (is_param(pcParam, xParamLen1, "start"))
    {
        start_stepping();
        snprintf(pcWriteBuffer, xWriteBufferLen, "OK: мотор запущен\r\n");
    } else if (is_param(pcParam, xParamLen1, "stop"))
    {
        stop_stepping();
        snprintf(pcWriteBuffer, xWriteBufferLen, "OK: мотор остановлен\r\n");
    } else
    {
        snprintf(pcWriteBuffer,
                 xWriteBufferLen,
                 MOTOR_USAGE);    // пусто или неизвестная команда
    }
    return pdFALSE;
}
/* FIXME: Исправить эту ошибку
[PET_PROJECT]

motor set_pin
Неправильные параметры команды
motor set_pin <pin (0-4)> <state (0/1)>

motor set_pin
Неправильные параметры команды
motor set_pin <pin (0-4)> <state (0/1)>

motor set_pin 1 1
OK: pin 1, state 1
motor set_pin
Неправильные параметры команды
motor set_pin <pin (0-4)> <state (0/1)>
OK: pin 1, state 1

*/
void cli_set_pin(int8_t id, int8_t state)
{
    switch (id)
    {
    case 1:
        GPIOF->BSRR = (state == 1) ? IN1_PIN : (uint32_t)IN1_PIN << 16;
        break;
    case 2:
        GPIOF->BSRR = (state == 1) ? LL_GPIO_PIN_14 : (uint32_t)LL_GPIO_PIN_14 << 16;
        break;
    case 3:
        GPIOF->BSRR = (state == 1) ? LL_GPIO_PIN_13 : (uint32_t)LL_GPIO_PIN_13 << 16;
        break;
    case 4:
        GPIOF->BSRR = (state == 1) ? LL_GPIO_PIN_12 : (uint32_t)LL_GPIO_PIN_12 << 16;
        break;
    case 5:
        GPIOF->BSRR = (state == 1) ? MOTOR_PIN_MASK : (uint32_t)MOTOR_PIN_MASK << 16;
        break;

    default:
        break;
    }
}
