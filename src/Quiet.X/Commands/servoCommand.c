
#include "../CLI/cli.h"
#include <stdint.h>

void ServoSetChannelValue(CliBuffer *buffer, uint8_t channel)
{
    if (channel >= 1 && channel <= 10)
    {
        if (*buffer->InputPnt == ' ')
        {
            // Get ready to parse this number
            ++buffer->InputPnt;
            
            // Get the value from the analog buffer
            int16_t value = ParseInt(&buffer->InputPnt);

            buffer->OutputPnt += IntToString(buffer->OutputPnt, (uint16_t)value);
        }
    }
}

const CommandDefinition servoCommands[] = {
  DEFINE_CHANNEL_COMMAND("CH", ServoSetChannelValue),  
};

const uint8_t servoCommandCount = sizeof(servoCommands) / sizeof(servoCommands[0]);

void ServoOutputs(CliBuffer *buffer)
{
    if (*buffer->InputPnt == ':')
    {
        ++buffer->InputPnt;
        ProcessCommand(servoCommands, servoCommandCount, buffer, false);
    }
}




