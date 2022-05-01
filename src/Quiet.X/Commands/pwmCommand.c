
#include "../CLI/cli.h"
#include "../outputs.h"
#include "../constants.h"
#include "outputCommand.h"

#include <stdint.h>

#define PWM_OFFSET 3
#define PWM_CHANNELS 6

const OutputCommand_t pwmCommandSettings = DEFINE_OUTPUT_COMMAND_T(PWM_CHANNELS, PWM_OFFSET, PWM_ERROR_GROUP);

void PWMChannelModeCommand(CliHandle_t *handle, void *channel)
{
    OutputChannelModeCommand(handle, pwmCommandSettings, channel);
}

void PWMChannelValueCommand(CliHandle_t *handle, void *channel)
{
    OutputChannelValueCommand(handle, pwmCommandSettings, channel);
}

CommandDefinition_t pwmChanCommands[] = {
    DEFINE_COMMAND("VALU", PWMChannelValueCommand),
    DEFINE_COMMAND("MODE", PWMChannelModeCommand),
};

CommandDefinition_t pwmCommands[] = {
    DEFINE_COMMAND_W_BRANCH("CH", PWMChannelValueCommand, pwmChanCommands),
};

CommandDefinition_t PWMCommand = DEFINE_BRANCH("PWM", pwmCommands);
