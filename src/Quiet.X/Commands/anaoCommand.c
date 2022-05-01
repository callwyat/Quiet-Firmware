
#include "../CLI/cli.h"
#include "../outputs.h"
#include "../constants.h"
#include "outputCommand.h"

#include <stdint.h>

#define ANAO_OFFSET 7
#define ANAO_CHANNELS 2

const OutputCommand_t anaoCommandSettings = DEFINE_OUTPUT_COMMAND_T(ANAO_CHANNELS, ANAO_OFFSET, ANAO_ERROR_GROUP);

void ANAOChannelModeCommand(CliHandle_t *handle, void *channel)
{
    OutputChannelModeCommand(handle, anaoCommandSettings, channel);
}

void ANAOChannelValueCommand(CliHandle_t *handle, void *channel)
{
    OutputChannelValueCommand(handle, anaoCommandSettings, channel);
}

CommandDefinition_t anaoChanCommands[] = {
    DEFINE_COMMAND("VALU", ANAOChannelValueCommand),
    DEFINE_COMMAND("MODE", ANAOChannelModeCommand),
};

CommandDefinition_t anaoCommands[] = {
    DEFINE_COMMAND_W_BRANCH("CH", ANAOChannelValueCommand, anaoChanCommands),
};

CommandDefinition_t ANAOCommand = DEFINE_BRANCH("ANAO", anaoCommands);
