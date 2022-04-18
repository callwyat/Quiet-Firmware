
#include "../CLI/cli.h"
#include "../outputs.h"
#include "../constants.h"
#include "outputCommand.h"

#include <stdint.h>

#define PWM_OFFSET 3
#define PWM_CHANNELS 6

const OutputCommand_t pwmCommandSettings = DEFINE_OUTPUT_COMMAND_T(PWM_CHANNELS, PWM_OFFSET, PWM_ERROR_GROUP);

void PWMChannelModeCommand(CliBuffer_t *buffer, void *channel)
{
    OutputChannelModeCommand(buffer, pwmCommandSettings, channel);
}

void PWMChannelValueCommand(CliBuffer_t *buffer, void *channel)
{
    OutputChannelValueCommand(buffer, pwmCommandSettings, channel);
}

CommandDefinition_t pwmChanCommands[] = {
    DEFINE_COMMAND("VALU", PWMChannelValueCommand),
    DEFINE_COMMAND("MODE", PWMChannelModeCommand),
};

CommandDefinition_t pwmCommands[] = {
    DEFINE_COMMAND_W_BRANCH("CH", PWMChannelValueCommand, pwmChanCommands),
};

CommandDefinition_t PWMCommand = DEFINE_BRANCH("PWM", pwmCommands);

