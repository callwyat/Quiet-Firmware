
#include "../CLI/cli.h"
#include "../outputs.h"
#include "../constants.h"
#include "outputCommand.h"

#include <stdint.h>

void QueueOutputErrorCode(uint16_t group, uint16_t code)
{
    QueueErrorCode((group << 8) | OUTPUT_ERROR_INVALID_CHANNEL);
}

uint8_t ValidateChannel(const OutputCommand_t settings, void *v)
{
    uint8_t channel = (uint8_t)(*((uint8_t*)v));
    if (channel < 1 || channel > settings.MaxChannels)
    {
        QueueOutputErrorCode(settings.ErrorGroup, OUTPUT_ERROR_INVALID_CHANNEL);
        __asm("pop");
    }

    return (uint8_t)(channel + settings.ChannelsOffset);
}

void OutputChannelModeCommand(CliBuffer_t *buffer, const OutputCommand_t settings, void *v)
{

    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        uint8_t channel = ValidateChannel(settings, v);
        
        OutputMode_e mode = GetOutputMode(channel);
        
        const char* word = OutputModeToString(mode);
        
        CopyWordToOutBuffer(buffer, word);
    }
    else if (*buffer->InputPnt == ' ')
    {
        ++buffer->InputPnt;
        
        uint8_t channel = ValidateChannel(settings, v);
        uint8_t error;

        if (SCPICompare(PWMWord, buffer->InputPnt))
        {
            error = SetOutputMode(channel, OUT_PWM);
        }
        else if (SCPICompare(ServoWord, buffer->InputPnt))
        {
            error = SetOutputMode(channel, OUT_SERVO);
        }
        else if (SCPICompare(DISCREETWord, buffer->InputPnt))
        {
            error = SetOutputMode(channel, OUT_DISCREET);
        }
        else
        {
            error = OUTPUT_ERROR_INVALID_MODE_SELECTION;
        }
        
        FFTilPunctuation(&buffer->InputPnt);

        if (error != OUTPUT_ERROR_NO_ERROR)
        {
            QueueOutputErrorCode(settings.ErrorGroup, error);
        }
    }
}

void OutputChannelValueCommand(CliBuffer_t *buffer, const OutputCommand_t settings, void *v)
{
    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        uint8_t channel = ValidateChannel(settings, v);
        uint16_t value = GetOutputValue(channel);
        NumberToString(buffer, value);
    }
    else if (*buffer->InputPnt == ' ')
    {
        ++buffer->InputPnt;
        uint8_t channel = ValidateChannel(settings, v);

        int16_t value = ParseInt(&buffer->InputPnt);
        if (value >= 0 && value < 1024)
        {
            SetOutputValue(channel, (uint16_t)value);
        }
        else
        {
            QueueOutputErrorCode(settings.ErrorGroup, OUTPUT_ERROR_INVALID_VALUE);
        }
    }
}

