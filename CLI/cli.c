

#include "cli.h"
#include "../constants.h"

#include "../Commands/standardCommands.h"
#include "../Commands/digiCommand.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <xc.h>

bool CompareStrings(const char *a, char *b, int length)
{
    for (int i = 0; i < length; ++i)
    {
        if (*a++ != *b++)
        {
            return false;
        }
    }
    
    return true;
}

int CountTillCommandEnd(char *input)
{
    char *c = input;
    
    while (*c != '\x00' && *c != ':' && *c != '?' && *c != ' ')
    {
        ++c;
    }
    
    return c - input;
}

void ProcessCommand(CommandDefinition commands[], uint8_t commandsLength, 
        CliBuffer *buffer)
{
    for (int i = 0; i < commandsLength; ++i)
    {
        if (CompareStrings(commands[i].Command, buffer->InputPnt, 4))
        {
            buffer->InputPnt += CountTillCommandEnd(buffer->InputPnt);
            commands[i].Handle(buffer);
            return;
        }
    }
}

void ByteToHexString(char* str, uint8_t b)
{
    *str++ = '0';
    *str++ = 'x';
    
    uint8_t upperNibble = (b >> 4) & 0x0F;
    *str++ = upperNibble + (upperNibble > 0x09 ? '7' : '0');
    
    uint8_t lowerNibble = (b & 0x0F);
    *str++ = lowerNibble + (lowerNibble > 0x09 ? '7' : '0');
    
    *str = 0x00;
}

CommandDefinition commands[] = {
    DEFINE_COMMAND("DIGI", DigitalInputs),
    DEFINE_COMMAND("*IDN", Identify),
};

const uint8_t CommandCount = sizeof(commands) / sizeof(commands[0]);

void ProcessCLI(CliBuffer *buffer)
{
    // Upper Case the input
    char *pnt = buffer->InputBuffer;
    
    while (*pnt != 0x00)
    {
        if (*pnt >= 'a' && *pnt <= 'z')
        {
            *pnt += 0x20;
        }
        
        ++pnt;
    }
    
    buffer->InputPnt = buffer->InputBuffer;
    
    buffer->OutputBuffer[0] = 0x00;
    
    ProcessCommand(commands, CommandCount, buffer);
    
    pnt = buffer->OutputBuffer;
    
    // If something was placed in the output buffer, make sure it is terminated
    if (*pnt != 0x00)
    {
        while (*pnt != 0x00) ++pnt;

        *pnt++ = '\n';
        *pnt++ = '\r';
        *pnt++ = '\x00';
    }
}
