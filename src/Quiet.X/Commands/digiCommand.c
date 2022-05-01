
#include "../CLI/cli.h"
#include "../constants.h"

void DIGIDiscreetCommand(CliHandle_t *handle, void *v)
{
    if (handle->LastRead == '?')
    {
        PrintNumber(handle, DIN);
    }
}

void DIGIChannelValueCommand(CliHandle_t *handle, void *v)
{

    if (handle->LastRead == '?')
    {
        uint8_t channel = *((uint8_t *)v);

        if (channel >= 1 && channel <= 8)
        {
            uint8_t value = (DIN & (1 << ((channel)-1))) > 0;
            PrintNumber(handle, value);
        }
        else
        {
            QueueErrorCode(DIGI_ERROR_INVALID_CHANNEL);
        }
    }
}

CommandDefinition_t digiChanCommands[] = {
    DEFINE_COMMAND("VALU", DIGIChannelValueCommand),
};

CommandDefinition_t digiCommands[] = {
    DEFINE_COMMAND_W_BRANCH("CH", DIGIChannelValueCommand, digiChanCommands),
};

CommandDefinition_t DIGICommand = DEFINE_COMMAND_W_BRANCH("DIGI", DIGIDiscreetCommand, digiCommands);