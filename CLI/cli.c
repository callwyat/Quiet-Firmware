

#include "cli.h"
#include "../constants.h"

#include "../Commands/standardCommands.h"
#include "../Commands/digiCommand.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <xc.h>

bool SCPICompare(const char *reference, char *input)
{
    // Match upto the first 4 chars, or until reference is null
    for (int i = 0; i < 4; ++i)
    {        
        if (*reference++ != *input++)
        {
            return false;
        }
        else if (*reference == 0x00)
        {
            break;
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
        CliBuffer *buffer, bool isRoot)
{
    for (int i = 0; i < commandsLength; ++i)
    {
        CommandDefinition command = commands[i];
        if (SCPICompare(command.Command, buffer->InputPnt))
        {
            buffer->InputPnt += CountTillCommandEnd(buffer->InputPnt);
            command.Handle(buffer);
            
            // Check for more commands at this level
            if (*buffer->InputPnt == ';')
            {
                ++buffer->InputPnt;
                *buffer->OutputPnt++ = ',';
                i = -1;
            }
            
            // Check if returning to root
            if (*buffer->InputPnt == ':' && isRoot)
            {
                ++buffer->InputPnt;
                continue;
            }
            
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
    buffer->OutputPnt = buffer->OutputBuffer;
    
    *buffer->OutputPnt = 0x00;
    
    ProcessCommand(commands, CommandCount, buffer, true);
    
    // If something was placed in the output buffer, make sure it is terminated
    if (buffer->OutputBuffer[0] != 0x00)
    {
        *buffer->OutputPnt++ = '\n';
        *buffer->OutputPnt++ = '\r';
        *buffer->OutputPnt++ = '\x00';
    }
}
