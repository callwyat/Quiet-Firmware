
#include "../CLI/cli.h"
#include "../constants.h"
#include "../outputs.h"

#define DIGOUT_OFFSET 1

uint8_t digoChannel;

void DIGOChannelModeCommand(CliBuffer *buffer)
{
    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        
        OutputMode_e mode = GetOutputMode(digoChannel + DIGOUT_OFFSET);
        
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
            SetOutputMode(digoChannel + DIGOUT_OFFSET, OUT_PWM);
        }
        else if (SCPICompare(ServoWord, buffer->InputPnt))
        {
            SetOutputMode(digoChannel + DIGOUT_OFFSET, OUT_SERVO);
        }
        else if (SCPICompare(DISCREETWord, buffer->InputPnt))
        {
            SetOutputMode(digoChannel + DIGOUT_OFFSET, OUT_DISCREET);
        }
        
        buffer->InputPnt += CountTillCommandEnd(buffer->InputPnt);
    }
}

void DIGOChannelValueCommand(CliBuffer *buffer)
{
    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        
        uint8_t value = GetDiscreetOutput(digoChannel + DIGOUT_OFFSET);        
        *buffer->OutputPnt++ = '0' + value;
    }
    else if (*buffer->InputPnt == ' ')
    {
        ++buffer->InputPnt;
        
        SetDiscreetOutput(digoChannel + DIGOUT_OFFSET, buffer->InputPnt == '1');
        
        ++buffer->InputPnt;
    }
}

const CommandDefinition digoChanCommands[] = {
  DEFINE_COMMAND("MODE", DIGOChannelModeCommand),  
  DEFINE_COMMAND("VALU", DIGOChannelValueCommand),  
};

const uint8_t digoChanCommandCount = sizeof(digoChanCommands) / sizeof(digoChanCommands[0]);

void DIGOChannelCommand(CliBuffer *buffer, uint8_t channel)
{
    if (channel > 0 && channel < 9)
    {
        digoChannel = channel;
        
        if (*buffer->InputPnt == ':')
        {
            ++buffer->InputPnt;
            ProcessCommand(digoChanCommands, digoChanCommandCount, buffer, false);
        }
    }
}

const CommandDefinition digoCommands[] = {
  DEFINE_CHANNEL_COMMAND("CH", DIGOChannelCommand),  
};

const uint8_t digoCommandCount = sizeof(digoCommands) / sizeof(digoCommands[0]);


void DigitalOutputs(CliBuffer *buffer)
{
    if (IS_QUERY(buffer))
    {
        //Progress the pointer past the query
        ++buffer->InputPnt;

        ByteToHexString(buffer->OutputPnt, DOUT);
        buffer->OutputPnt += 4;
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
