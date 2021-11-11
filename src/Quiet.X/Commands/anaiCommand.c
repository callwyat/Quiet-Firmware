
#include "../analogInputs.h"

#include "../CLI/cli.h"
#include <stdint.h>

void PrintAnalogInput(CliBuffer_t *buffer, uint8_t channel)
{
    if (channel >= 1 && channel <= 4)
    {
        if (*buffer->InputPnt == '?')
        {
            // Progress the input past this command
            ++buffer->InputPnt;
            
            // Get the value from the analog buffer
            uint16_t value = GetADCValue(channel - 1);
            Int14ToString(buffer, value);
        }
    }
}

const CommandDefinition anaiCommands[] = {
  DEFINE_CHANNEL_COMMAND("CH", PrintAnalogInput),  
};

const uint8_t anaiCommandCount = sizeof(anaiCommands) / sizeof(anaiCommands[0]);

void AnalogInputs(CliBuffer_t *buffer)
{
    if (*buffer->InputPnt == ':')
    {
        ++buffer->InputPnt;
        ProcessCommand(anaiCommands, anaiCommandCount, buffer, false);
    }
}