
#include "../CLI/cli.h"
#include "../outputs.h"
#include <stdint.h>

#define SERVO_OFFSET -1

uint8_t servoChannel;

#define SERVO_CHANNEL ((uint8_t)(servoChannel + SERVO_OFFSET))

void SERVChannelModeCommand(CliBuffer *buffer)
{
    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        
        OutputMode_e mode = GetOutputMode(SERVO_CHANNEL);
        
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
            SetOutputMode(SERVO_CHANNEL, OUT_SERVO);
        }
        
        FFTilPunctuation(&buffer->InputPnt);
    }
}

void SERVChannelValueCommand(CliBuffer *buffer)
{
    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        uint16_t value = GetOutputValue(SERVO_CHANNEL);
        IntToString(buffer, value);
    }
    else if (*buffer->InputPnt == ' ')
    {
        ++buffer->InputPnt;
        int16_t value = ParseInt(&buffer->InputPnt);
        if (value > 0)
        {
            SetOutputValue(SERVO_CHANNEL, (uint16_t)value);
        }
    }
}

const CommandDefinition servChanCommands[] = {
  DEFINE_COMMAND("MODE", SERVChannelModeCommand),  
  DEFINE_COMMAND("VALU", SERVChannelValueCommand),  
};

const uint8_t servChanCommandCount = sizeof(servChanCommands) / sizeof(servChanCommands[0]);


void SERVChannelCommand(CliBuffer *buffer, uint8_t channel)
{
    if (channel >= 1 && channel <= 2)
    {
        servoChannel = channel;
        
        if (*buffer->InputPnt == ':')
        {
            ++buffer->InputPnt;
            ProcessCommand(servChanCommands, servChanCommandCount, buffer, false);  
        }
        else
        {
            // Default to working with the value
            SERVChannelValueCommand(buffer);
        }
    }
}

const CommandDefinition servoCommands[] = {
  DEFINE_CHANNEL_COMMAND("CH", SERVChannelCommand),  
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




