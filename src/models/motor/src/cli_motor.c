#include "cli_motor.h"

#include "cli.h"

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

    if (param_is(pcParam, xParamLen1, "set_pin"))
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

        // atoi() тоже небезопасен на не-null-terminated строке — нужна копия
        char pin_buf[8]   = {0};
        char state_buf[8] = {0};
        memcpy(pin_buf, pin, xParamLen2 < 7 ? xParamLen2 : 7);
        memcpy(state_buf, state, xParamLen3 < 7 ? xParamLen3 : 7);

        int8_t pin_int   = (int8_t)atoi(pin_buf);
        int8_t state_int = (int8_t)atoi(state_buf);

        SetPin(pin_int, state_int);
    }
    return pdFALSE;
}