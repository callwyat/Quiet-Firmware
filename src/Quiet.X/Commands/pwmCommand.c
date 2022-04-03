
#include "../CLI/cli.h"
#include "../outputs.h"
#include <stdint.h>

#define PWM_OFFSET 3

#define PWM_CHANNEL (uint8_t)(*((uint8_t*)channel) + PWM_OFFSET)

void PWMChannelModeCommand(CliBuffer_t *buffer, void *channel)
{
    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        
        OutputMode_e mode = GetOutputMode(PWM_CHANNEL);
        
        const char* word = OutputModeToString(mode);
        
        CopyWordToOutBuffer(buffer, word);
    }
    else if (*buffer->InputPnt == ' ')
    {
        ++buffer->InputPnt;
        
        // To Upper
        *buffer->InputPnt &= 0xDF;
        
        if (SCPICompare(PWMWord, buffer->InputPnt))
        {
            SetOutputMode(PWM_CHANNEL, OUT_PWM);
        }
        
        FFTilPunctuation(&buffer->InputPnt);
    }
}

void PWMChannelValueCommand(CliBuffer_t *buffer, void *channel)
{
    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        uint16_t value = GetOutputValue(PWM_CHANNEL);
        NumberToString(buffer, value);
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

CommandDefinition_t pwmChanCommands[] = {
    DEFINE_COMMAND("VALU", PWMChannelValueCommand),
    DEFINE_COMMAND("MODE", PWMChannelModeCommand),
};

CommandDefinition_t pwmCommands[] = {
    DEFINE_COMMAND_W_BRANCH("CH", PWMChannelValueCommand, pwmChanCommands),
};

CommandDefinition_t PWMCommand = DEFINE_BRANCH("PWM", pwmCommands);

