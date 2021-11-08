
#include "../CLI/cli.h"
#include "../outputs.h"
#include <stdint.h>

#define PWM_OFFSET -1

uint8_t pwmChannel;

#define PWM_CHANNEL ((uint8_t)(pwmChannel + PWM_OFFSET))

void PWMChannelModeCommand(CliBuffer *buffer)
{
    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        
        OutputMode_e mode = GetOutputMode(PWM_CHANNEL);
        
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
                word = DISCREETWord;
                break;
        }
        
        CopyWordToOutBuffer(buffer, word);
    }
    else if (*buffer->InputPnt == ' ')
    {
        ++buffer->InputPnt;
        
        // To Upper
        *buffer->InputPnt &= 0xDF;
        
        if (SCPICompare(ServoWord, buffer->InputPnt))
        {
            SetOutputMode(PWM_CHANNEL, OUT_SERVO);
        }
        
        FFTilPunctuation(&buffer->InputPnt);
    }
}

void PWMChannelValueCommand(CliBuffer *buffer)
{
    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        uint16_t value = GetOutputValue(PWM_CHANNEL);
        IntToString(buffer, value);
    }
    else if (*buffer->InputPnt == ' ')
    {
        ++buffer->InputPnt;
        int16_t value = ParseInt(&buffer->InputPnt);
        if (value >= 0)
        {
            SetOutputValue(PWM_CHANNEL, (uint16_t)value);
        }
    }
}

const CommandDefinition pwmChanCommands[] = {
  DEFINE_COMMAND("MODE", PWMChannelModeCommand),  
  DEFINE_COMMAND("VALU", PWMChannelValueCommand),  
};

const uint8_t pwmChanCommandCount = sizeof(pwmChanCommands) / sizeof(pwmChanCommands[0]);


void PWMSetChannelValue(CliBuffer *buffer, uint8_t channel)
{
    if (channel >= 1 && channel <= 6)
    {
        pwmChannel = channel;
        
        if (*buffer->InputPnt == ':')
        {
            ++buffer->InputPnt;
            ProcessCommand(pwmChanCommands, pwmChanCommandCount, buffer, false);  
        }
        else
        {
            // Default to working with the value
            PWMChannelValueCommand(buffer);
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



