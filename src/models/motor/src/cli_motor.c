#include "cli_motor.h"

#include "cli.h"
#include "string.h"

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

    snprintf(pcWriteBuffer,
             xWriteBufferLen,
             "motor: got param %.*s ",
             (int)xParamLen1,
             pcParam);

    if (is_param(pcParam, xParamLen1, "set_pin"))
    {
        const char* pin   = FreeRTOS_CLIGetParameter(pcCommandString, 2, &xParamLen2);
        const char* state = FreeRTOS_CLIGetParameter(pcCommandString, 3, &xParamLen3);

        snprintf(pcWriteBuffer,
                 xWriteBufferLen,
                 "OK: pin %.*s, state %.*s\r\n",
                 (int)xParamLen2,
                 pin,
                 (int)xParamLen3,
                 state);

        char pin_buf[8]   = {0};
        char state_buf[8] = {0};
        memcpy(pin_buf, pin, xParamLen2 < 7 ? xParamLen2 : 7);
        memcpy(state_buf, state, xParamLen3 < 7 ? xParamLen3 : 7);

        int8_t pin_int   = (int8_t)atoi(pin_buf);
        int8_t state_int = (int8_t)atoi(state_buf);

        cli_set_pin(pin_int, state_int);
    }
    return pdFALSE;
}

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