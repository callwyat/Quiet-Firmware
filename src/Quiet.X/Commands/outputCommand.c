
#include "../CLI/cli.h"
#include "../outputs.h"
#include "../constants.h"
#include "outputCommand.h"

#include <stdint.h>

void QueueOutputErrorCode(uint16_t group, uint16_t code)
{
    QueueErrorCode((group << 8) | code);
}

uint8_t ValidateChannel(const OutputCommand_t settings, void *v)
{
    uint8_t channel = (uint8_t)(*((uint8_t *)v));
    if (channel < 1 || channel > settings.MaxChannels)
    {
        QueueOutputErrorCode(settings.ErrorGroup, OUTPUT_ERROR_INVALID_CHANNEL);
        __asm("pop");
    }

    return (uint8_t)(channel + settings.ChannelsOffset);
}

void OutputChannelModeCommand(CliHandle_t *handle, const OutputCommand_t settings, void *v)
{

    if (handle->LastRead == '?')
    {
        ReadChar(handle);
        uint8_t channel = ValidateChannel(settings, v);

        OutputMode_e mode = GetOutputMode(channel);

        const char *word = OutputModeToString(mode);

        WriteString(handle, word);
    }
    else if (handle->LastRead == ' ')
    {
        uint8_t channel = ValidateChannel(settings, v);
        uint8_t error;

        ReadWord(handle);
        if (SCPICompare(PWMWord, handle->LastWord))
        {
            error = SetOutputMode(channel, OUT_PWM);
        }
        else if (SCPICompare(ServoWord, handle->LastWord))
        {
            error = SetOutputMode(channel, OUT_SERVO);
        }
        else if (SCPICompare(DISCREETWord, handle->LastWord))
        {
            error = SetOutputMode(channel, OUT_DISCREET);
        }
        else
        {
            error = OUTPUT_ERROR_INVALID_MODE_SELECTION;
        }

        if (error != OUTPUT_ERROR_NO_ERROR)
        {
            QueueOutputErrorCode(settings.ErrorGroup, error);
        }
    }
}

void OutputChannelValueCommand(CliHandle_t *handle, const OutputCommand_t settings, void *v)
{
    if (handle->LastRead == ' ')
    {
        uint8_t channel = ValidateChannel(settings, v);

        OutputMode_e mode = GetOutputMode(channel);
        if (mode == OUT_DISCREET)
        {
            bool value = ReadBool(handle);
            SetOutputValue(channel, (uint16_t)value);
        }
        else
        {
            uint16_t value = ReadInt(handle);
            if (value >= 0 && value < 1024)
            {
                SetOutputValue(channel, value);
            }
            else
            {
                QueueOutputErrorCode(settings.ErrorGroup, OUTPUT_ERROR_INVALID_VALUE);
            }
        }
    }
    else if (handle->LastRead == '?')
    {
        ReadChar(handle);
        uint8_t channel = ValidateChannel(settings, v);
        uint16_t value = GetOutputValue(channel);
        WriteNumber(handle, value);
    }
}
