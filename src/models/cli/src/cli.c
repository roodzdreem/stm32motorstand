#include "cli.h"

#include <stdio.h>

static BaseType_t CLI_motorstand(char*       pcWriteBuffer,
                                 size_t      xWriteBufferLen,
                                 const char* pcCommandString);

const CLI_Command_Definition_t xCLI_motor = {
    .pcCommand                   = "motor",
    .pcHelpString                = "motor - пока какая-нибудь хуйня, потом поправлю\r\n",
    .pxCommandInterpreter        = CLI_motorstand,
    .cExpectedNumberOfParameters = -1};

BaseType_t CLI_motorstand_init()
{
    return (BaseType_t)FreeRTOS_CLIRegisterCommand(&xCLI_motor);
}
static BaseType_t CLI_motorstand(char*       pcWriteBuffer,
                                 size_t      xWriteBufferLen,
                                 const char* pcCommandString)
{
    BaseType_t  xParamLen;
    const char* pcParam1 = FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParamLen);


    snprintf(pcWriteBuffer, xWriteBufferLen, "motor: got param\r\n");
    return pdFALSE;    // pdTRUE, если это не последний кусок вывода (см. ниже)
}
void vCliTaskStart(void) {}