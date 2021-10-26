
#include "../CLI/cli.h"
#include <stdint.h>

void AnaoSetChannelValue(CliBuffer *buffer, uint8_t channel)
{
    if (channel >= 1 && channel <= 2)
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

const CommandDefinition anaoCommands[] = {
  DEFINE_CHANNEL_COMMAND("CH", AnaoSetChannelValue),  
};

const uint8_t anaoCommandCount = sizeof(anaoCommands) / sizeof(anaoCommands[0]);

void AnalogOutputs(CliBuffer *buffer)
{
    if (*buffer->InputPnt == ':')
    {
        ++buffer->InputPnt;
        ProcessCommand(anaoCommands, anaoCommandCount, buffer, false);
    }
}


