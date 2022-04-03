
#include "../CLI/cli.h"
#include "../constants.h"
#include "../outputs.h"

#define DIGOUT_OFFSET -1

#define DIGOUT_CHANNEL (uint8_t)(*((uint8_t*)channel) + DIGOUT_OFFSET)

void DIGOChannelModeCommand(CliBuffer_t *buffer, void *channel)
{
    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        
        OutputMode_e mode = GetOutputMode(DIGOUT_CHANNEL);
        
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
            SetOutputMode(DIGOUT_CHANNEL, OUT_PWM);
        }
        else if (SCPICompare(ServoWord, buffer->InputPnt))
        {
            SetOutputMode(DIGOUT_CHANNEL, OUT_SERVO);
        }
        else if (SCPICompare(DISCREETWord, buffer->InputPnt))
        {
            SetOutputMode(DIGOUT_CHANNEL, OUT_DISCREET);
        }
        
        FFTilPunctuation(&buffer->InputPnt);
    }
}

void DIGOChannelValueCommand(CliBuffer_t *buffer, void *channel)
{
    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        uint16_t value = GetOutputValue(DIGOUT_CHANNEL);
        NumberToString(buffer, value);
    }
    else if (*buffer->InputPnt == ' ')
    {
        ++buffer->InputPnt;
        int16_t value = ParseInt(&buffer->InputPnt);
        if (value >= 0)
        {
            SetOutputValue(DIGOUT_CHANNEL, (uint16_t)value);
        }
    }
}

CommandDefinition_t digoChanCommands[] = {
    DEFINE_COMMAND("VALU", DIGOChannelValueCommand),  
    DEFINE_COMMAND("MODE", DIGOChannelModeCommand),  
};

CommandDefinition_t digoCommands[] = {
    DEFINE_COMMAND_W_BRANCH("CH", DIGOChannelValueCommand, digoChanCommands),
};

void DIGODiscreetCommand(CliBuffer_t *buffer, void* v)
{
    if (*buffer->InputPnt == '?')
    {
        //Progress the pointer past the query
        ++buffer->InputPnt;
        NumberToString(buffer, DOUT);
    }
}

CommandDefinition_t DIGOCommand = DEFINE_COMMAND_W_BRANCH("DIGO", DIGODiscreetCommand, digoCommands);
