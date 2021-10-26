

#include "../CLI/cli.h"
#include <stdint.h>

void GetChannelValue(CliBuffer *buffer, uint8_t channel)
{
    if (channel >= 0 && channel <= 4)
    {
        if (*buffer->InputPnt == '?')
        {
            // Progress the input past this command
            ++buffer->InputPnt;
            
            // Get the value from the analog buffer
            uint16_t value = 0x1 << channel;

            buffer->OutputPnt += IntToString(buffer->OutputPnt, value);
        }
    }
}

const CommandDefinition anaiCommands[] = {
  DEFINE_CHANNEL_COMMAND("CH", GetChannelValue),  
};

const uint8_t anaiCommandCount = sizeof(anaiCommands) / sizeof(anaiCommands[0]);

void AnalogInputs(CliBuffer *buffer)
{
    if (*buffer->InputPnt == ':')
    {
        ++buffer->InputPnt;
        ProcessCommand(anaiCommands, anaiCommandCount, buffer, false);
    }
}