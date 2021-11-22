
#include "../CLI/cli.h"
#include "../outputs.h"
#include <stdint.h>

#define SERVO_OFFSET -1

uint8_t servoChannel;

#define SERVO_CHANNEL (*((uint8_t*)channel) + SERVO_OFFSET)

void SERVChannelModeCommand(CliBuffer_t *buffer, void *channel)
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

void SERVChannelValueCommand(CliBuffer_t *buffer, void *channel)
{
    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        uint16_t value = GetOutputValue(SERVO_CHANNEL);
        NumberToString(buffer, value);
    }
    else if (*buffer->InputPnt == ' ')
    {
        ++buffer->InputPnt;
        int16_t value = ParseInt(&buffer->InputPnt);
        if (value >= 0)
        {
            SetOutputValue(SERVO_CHANNEL, (uint16_t)value);
        }
    }
}

const CommandDefinition_t servChanCommands[] = {
    DEFINE_COMMAND("VALU", SERVChannelValueCommand),
    DEFINE_COMMAND("MODE", SERVChannelModeCommand),
};

const CommandDefinition_t servoCommands[] = {
    {
        .Command = "CH",
        .Handle = SERVChannelValueCommand,
        .Children = servChanCommands,
        .ChildrenCount = sizeof(servChanCommands) / sizeof(servChanCommands[0])
    }
};

const CommandDefinition_t SERVoCommand = DEFINE_BRANCH("SERV", servoCommands);

