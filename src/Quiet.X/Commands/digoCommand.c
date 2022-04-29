
#include "../CLI/cli.h"
#include "../constants.h"
#include "../outputs.h"
#include "outputCommand.h"

#define DIGO_OFFSET -1
#define DIGO_CHANNELS 8

const OutputCommand_t digoCommandSettings = DEFINE_OUTPUT_COMMAND_T(DIGO_CHANNELS, DIGO_OFFSET, DIGO_ERROR_GROUP);

void DIGOChannelModeCommand(CliBuffer_t *buffer, void *channel)
{
    OutputChannelModeCommand(buffer, digoCommandSettings, channel);
}

void DIGOChannelValueCommand(CliBuffer_t *buffer, void *channel)
{
    OutputChannelValueCommand(buffer, digoCommandSettings, channel);
}

CommandDefinition_t digoChanCommands[] = {
    DEFINE_COMMAND("VALU", DIGOChannelValueCommand),  
    DEFINE_COMMAND("MODE", DIGOChannelModeCommand),  
};

CommandDefinition_t digoCommands[] = {
    DEFINE_COMMAND_W_BRANCH("CH", DIGOChannelValueCommand, digoChanCommands),
};

void DIGODiscreetCommand(CliBuffer_t *buffer, void* v)
{
    if (*buffer->InputPnt == '?')
    {
        //Progress the pointer past the query
        ++buffer->InputPnt;
        NumberToString(buffer, DOUT);
    }
    else if (*buffer->InputPnt == ' ')
    {
        ++buffer->InputPnt;
        int16_t value = ParseInt(&buffer->InputPnt);
        if (value >= 0 && value < 256)
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
