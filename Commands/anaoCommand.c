
#include "../CLI/cli.h"
#include "../outputs.h"

#include <stdint.h>

#define ANAOUT_OFFSET -1

uint8_t anaoChannel;

void ANAOChannelModeCommand(CliBuffer *buffer)
{
    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        
        OutputMode_e mode = GetOutputMode(anaoChannel + ANAOUT_OFFSET);
        
        const char* word;
        
        switch (mode)
        {                
            case OUT_PWM:
                word = PWMWord;
                break;
                
            case OUT_SERVO:
                word = ServoWord;
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
            SetOutputMode(anaoChannel + ANAOUT_OFFSET, OUT_PWM);
        }
        else if (SCPICompare(ServoWord, buffer->InputPnt))
        {
            SetOutputMode(anaoChannel + ANAOUT_OFFSET, OUT_SERVO);
        }
        
        buffer->InputPnt += CountTillCommandEnd(buffer->InputPnt);
    }
}

void ANAOChannelValueCommand(CliBuffer *buffer)
{
    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        
        uint8_t value = GetDiscreetOutput(anaoChannel + ANAOUT_OFFSET);        
        *buffer->OutputPnt++ = '0' + value;
    }
    else if (*buffer->InputPnt == ' ')
    {
        ++buffer->InputPnt;
        
        SetDiscreetOutput(anaoChannel + ANAOUT_OFFSET, buffer->InputPnt == '1');
        
        ++buffer->InputPnt;
    }
}

const CommandDefinition anaoChanCommands[] = {
  DEFINE_COMMAND("MODE", ANAOChannelModeCommand),  
  DEFINE_COMMAND("VALU", ANAOChannelValueCommand),  
};

const uint8_t anaoChanCommandCount = sizeof(anaoChanCommands) / sizeof(anaoChanCommands[0]);


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
            
            SetOutputValue(channel + ANAOUT_OFFSET, value);
        }
        else if (*buffer->InputPnt == '?')
        {
            ++buffer->InputPnt;
            
            buffer->OutputPnt += IntToString(buffer->OutputPnt, 
                    GetOutputValue(channel + ANAOUT_OFFSET));
        }
        else if (*buffer->InputPnt == ':')
        {
            ++buffer->InputPnt;
            ProcessCommand(anaoChanCommands, anaoChanCommandCount, buffer, false);  
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


