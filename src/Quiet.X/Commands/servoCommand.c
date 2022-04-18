
#include "../CLI/cli.h"
#include "../constants.h"
#include "../outputs.h"
#include "outputCommand.h"
#include <stdint.h>

#define SERVO_OFFSET -1
#define SERVO_CHANNELS 10

const OutputCommand_t servCommandSettings = DEFINE_OUTPUT_COMMAND_T(SERVO_CHANNELS, SERVO_OFFSET, SERV_ERROR_GROUP);

void SERVChannelModeCommand(CliBuffer_t *buffer, void *channel)
{
    OutputChannelModeCommand(buffer, servCommandSettings, channel);
}

void SERVChannelValueCommand(CliBuffer_t *buffer, void *channel)
{
    OutputChannelValueCommand(buffer, servCommandSettings, channel);
}

CommandDefinition_t servChanCommands[] = {
    DEFINE_COMMAND("VALU", SERVChannelValueCommand),
    DEFINE_COMMAND("MODE", SERVChannelModeCommand),
};

CommandDefinition_t servoCommands[] = {
    DEFINE_COMMAND_W_BRANCH("CH", SERVChannelValueCommand, servChanCommands),
};

CommandDefinition_t SERVoCommand = DEFINE_BRANCH("SERV", servoCommands);

