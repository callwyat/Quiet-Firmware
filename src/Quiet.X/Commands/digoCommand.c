
#include "../CLI/cli.h"
#include "../constants.h"
#include "../outputs.h"

#define DIGOUT_OFFSET -1

uint8_t digoChannel;

#define DIGOUT_CHANNEL ((uint8_t)(digoChannel + DIGOUT_OFFSET))

void DIGOChannelModeCommand(CliBuffer_t *buffer)
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

void DIGOChannelValueCommand(CliBuffer_t *buffer)
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

const CommandDefinition digoChanCommands[] = {
  DEFINE_COMMAND("MODE", DIGOChannelModeCommand),  
  DEFINE_COMMAND("VALU", DIGOChannelValueCommand),  
};

const uint8_t digoChanCommandCount = sizeof(digoChanCommands) / sizeof(digoChanCommands[0]);

void DIGOChannelCommand(CliBuffer_t *buffer, uint8_t channel)
{
    if (channel > 0 && channel < 9)
    {
        digoChannel = channel;
        
        if (*buffer->InputPnt == ':')
        {
            ++buffer->InputPnt;
            ProcessCommand(digoChanCommands, digoChanCommandCount, buffer, false);
        }
        else
        {
            // Default to working with the value
            DIGOChannelValueCommand(buffer);
        }
    }
}

const CommandDefinition digoCommands[] = {
  DEFINE_CHANNEL_COMMAND("CH", DIGOChannelCommand),  
};

const uint8_t digoCommandCount = sizeof(digoCommands) / sizeof(digoCommands[0]);


void DIGOCommand(CliBuffer_t *buffer)
{
    if (*buffer->InputPnt == '?')
    {
        //Progress the pointer past the query
        ++buffer->InputPnt;
        NumberToString(buffer, DOUT);
    }
    else if (*buffer->InputPnt == ' ')
    {
        //Progress the pointer past the query
        ++buffer->InputPnt;
        
        int16_t value = ParseInt(&buffer->InputPnt);
        
        if (value >= 0)
        {
            SetDiscreetOutputs((uint8_t)value);
        }
        else
        {
            // TODO: Record an error?
        }
    }
    else if (*buffer->InputPnt == ':')
    {
        ++buffer->InputPnt;
        ProcessCommand(digoCommands, digoCommandCount, buffer, false);
    }
}
