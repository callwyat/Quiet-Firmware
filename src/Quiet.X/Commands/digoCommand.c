
#include "../CLI/cli.h"
#include "../constants.h"
#include "../outputs.h"
#include "outputCommand.h"

#define DIGO_OFFSET -1
#define DIGO_CHANNELS 8

const OutputCommand_t digoCommandSettings = DEFINE_OUTPUT_COMMAND_T(DIGO_CHANNELS, DIGO_OFFSET, DIGO_ERROR_GROUP);

void DIGOChannelModeCommand(CliHandle_t *handle, void *channel)
{
    OutputChannelModeCommand(handle, digoCommandSettings, channel);
}

void DIGOChannelValueCommand(CliHandle_t *handle, void *channel)
{
    OutputChannelValueCommand(handle, digoCommandSettings, channel);
}

CommandDefinition_t digoChanCommands[] = {
    DEFINE_COMMAND("VALU", DIGOChannelValueCommand),
    DEFINE_COMMAND("MODE", DIGOChannelModeCommand),
};

CommandDefinition_t digoCommands[] = {
    DEFINE_COMMAND_W_BRANCH("CH", DIGOChannelValueCommand, digoChanCommands),
};

void DIGODiscreetCommand(CliHandle_t *handle, void *v)
{
    if (handle->LastRead == '?')
    {
        ReadChar(handle);
        WriteNumber(handle, DOUT);
    }
    else if (handle->LastRead == ' ')
    {
        uint16_t value = ReadInt(handle);
        if (value < 256)
        {
            DOUT = (uint8_t)value;
        }
        else
        {
            QueueErrorCode(DIGO_ERROR_GROUP << 8 | OUTPUT_ERROR_INVALID_VALUE);
        }
    }
}

CommandDefinition_t DIGOCommand = DEFINE_COMMAND_W_BRANCH("DIGO", DIGODiscreetCommand, digoCommands);
