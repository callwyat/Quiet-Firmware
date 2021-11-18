
#include "../CLI/cli.h"
#include "../outputs.h"

#include <stdint.h>

#define ANAOUT_OFFSET 7

#define ANAOUT_CHANNEL (uint8_t)(*((uint8_t*)channel) + ANAOUT_OFFSET)

void ANAOChannelModeCommand(CliBuffer_t *buffer, void *channel)
{
    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        
        OutputMode_e mode = GetOutputMode(ANAOUT_CHANNEL);
        
        const char* word;
        
        switch (mode)
        {                
            case OUT_PWM:
                word = PWMWord;
                break;
                
            case OUT_SERVO:
                word = ServoWord;
                break;
                
            case OUT_DISCREET:
                // Not a valid option
                break;
        }
        
        CopyWordToOutBuffer(buffer, word);
    }
    else if (*buffer->InputPnt == ' ')
    {
        ++buffer->InputPnt;
        
        // To Upper
        *buffer->InputPnt &= 0xDF;
        
        if (SCPICompare(PWMWord, buffer->InputPnt))
        {
            SetOutputMode(ANAOUT_CHANNEL, OUT_PWM);
        }
        else if (SCPICompare(ServoWord, buffer->InputPnt))
        {
            SetOutputMode(ANAOUT_CHANNEL, OUT_SERVO);
        }
        
        FFTilPunctuation(&buffer->InputPnt);
    }
}

void ANAOChannelValueCommand(CliBuffer_t *buffer, void *channel)
{
    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        uint16_t value = GetOutputValue(ANAOUT_CHANNEL);
        NumberToString(buffer, value);
    }
    else if (*buffer->InputPnt == ' ')
    {
        ++buffer->InputPnt;
        int16_t value = ParseInt(&buffer->InputPnt);
        if (value >= 0)
        {
            SetOutputValue(ANAOUT_CHANNEL, (uint16_t)value);
        }
    }
}

const CommandDefinition_t anaoChanCommands[] = {
  DEFINE_COMMAND("VALU", ANAOChannelValueCommand),
  DEFINE_COMMAND("MODE", ANAOChannelModeCommand),  
};

const CommandDefinition_t anaoCommands[] = {
    {
        .Command = "CH",
        .Handle = ANAOChannelValueCommand,
        .Children = anaoChanCommands,
        .ChildrenCount = sizeof(anaoChanCommands) / sizeof(anaoChanCommands[0])
    },
};

const CommandDefinition_t ANAOCommand = DEFINE_BRANCH("ANAO", anaoCommands);

