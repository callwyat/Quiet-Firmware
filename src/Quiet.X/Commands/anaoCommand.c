
#include "../CLI/cli.h"
#include "../outputs.h"

#include <stdint.h>

#define ANAOUT_OFFSET -1

uint8_t anaoChannel;

#define ANAOUT_CHANNEL ((uint8_t)(anaoChannel + ANAOUT_OFFSET))

void ANAOChannelModeCommand(CliBuffer_t *buffer)
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

void ANAOChannelValueCommand(CliBuffer_t *buffer)
{
    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        uint16_t value = GetOutputValue(ANAOUT_CHANNEL);
        IntToString(buffer, value);
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

const CommandDefinition anaoChanCommands[] = {
  DEFINE_COMMAND("VALU", ANAOChannelValueCommand),
  DEFINE_COMMAND("MODE", ANAOChannelModeCommand),  
};

const uint8_t anaoChanCommandCount = sizeof(anaoChanCommands) / sizeof(anaoChanCommands[0]);


void ANAOChannelCommand(CliBuffer_t *buffer, uint8_t channel)
{
    if (channel >= 1 && channel <= 2)
    {
        anaoChannel = channel;
        
        if (*buffer->InputPnt == ':')
        {
            ++buffer->InputPnt;
            ProcessCommand(anaoChanCommands, anaoChanCommandCount, buffer, false);  
        }
        else
        {
            // Default to working with the value
            ANAOChannelValueCommand(buffer);
        }
    }
}

const CommandDefinition anaoCommands[] = {
  DEFINE_CHANNEL_COMMAND("CH", ANAOChannelCommand),  
};

const uint8_t anaoCommandCount = sizeof(anaoCommands) / sizeof(anaoCommands[0]);

void AnalogOutputs(CliBuffer_t *buffer)
{
    if (*buffer->InputPnt == ':')
    {
        ++buffer->InputPnt;
        ProcessCommand(anaoCommands, anaoCommandCount, buffer, false);
    }
}


