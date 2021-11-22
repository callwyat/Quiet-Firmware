
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
        
        const char* word;
        
        switch (mode)
        {
            case OUT_DISCREET:
                word = DISCREETWord;
                break;
                
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

const CommandDefinition_t digoChanCommands[] = {
    DEFINE_COMMAND("VALU", DIGOChannelValueCommand),  
    DEFINE_COMMAND("MODE", DIGOChannelModeCommand),  
};

const CommandDefinition_t digoCommands[] = {
    {
        .Command = "CH",
        .Handle = DIGOChannelValueCommand,
        .Children = digoChanCommands,
        .ChildrenCount = sizeof(digoChanCommands) / sizeof(digoChanCommands[0]),
    }
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

const CommandDefinition_t DIGOCommand = {
    .Command = "DIGO",
    .Handle = DIGODiscreetCommand,
    .Children = digoCommands,
    .ChildrenCount = sizeof(digoCommands) / sizeof(digoCommands[0]),
};
