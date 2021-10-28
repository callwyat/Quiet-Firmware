
#include "../CLI/cli.h"
#include <stdint.h>

void PWMSetChannelValue(CliBuffer *buffer, uint8_t channel)
{
    if (channel >= 1 && channel <= 6)
    {
        if (*buffer->InputPnt == ' ')
        {
            // Get ready to parse this number
            ++buffer->InputPnt;
            
            // Get the value from the analog buffer
            int16_t value = ParseInt(&buffer->InputPnt);

            buffer->OutputPnt += IntToString(buffer->OutputPnt, value);
        }
    }
}

const CommandDefinition pwmCommands[] = {
  DEFINE_CHANNEL_COMMAND("CH", PWMSetChannelValue),  
};

const uint8_t pwmCommandCount = sizeof(pwmCommands) / sizeof(pwmCommands[0]);

void PWMOutputs(CliBuffer *buffer)
{
    if (*buffer->InputPnt == ':')
    {
        ++buffer->InputPnt;
        ProcessCommand(pwmCommands, pwmCommandCount, buffer, false);
    }
}



