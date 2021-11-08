

#include "cli.h"
#include "../constants.h"

#include "../Commands/standardCommands.h"
#include "../Commands/digiCommand.h"
#include "../Commands/digoCommand.h"
#include "../Commands/anaiCommand.h"
#include "../Commands/anaoCommand.h"
#include "../Commands/pwmCommand.h"
#include "../Commands/servoCommand.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <xc.h>

#define IS_NUMBER(c) (c >= '0' && c <= '9')

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

void FFTilPunctuation(char **input)
{
    while (**input)
    {
        switch (**input)
        {
            case ':':
            case ' ':
            case '?':
            case ';':
                return;
        }

        ++(*input);
    }
}

void ProcessCommand(const CommandDefinition commands[], uint8_t commandsLength, 
        CliBuffer *buffer, bool isRoot)
{
    for (int i = 0; i < commandsLength; ++i)
    {
        const CommandDefinition* command = &commands[i];
        if (SCPICompare(command->Command, buffer->InputPnt))
        {
            if (command->Handle)
            {
                FFTilPunctuation(&buffer->InputPnt);
                command->Handle(buffer);
            }
            else if (command->ChannelHandle)
            {
                // Scan until a number is found
                while (*buffer->InputPnt < '0' || *buffer->InputPnt > '9')
                {
                    // Check if it punctuation showed up
                    switch (*buffer->InputPnt)
                    {
                        case ':':
                        case ' ':
                        case '?':
                        case ';':
                            return;
                    }
                    
                    ++buffer->InputPnt;
                }
                
                // Parse the number
                uint8_t channel = 0;
                
                do
                {
                    channel *= 10;
                    channel += *buffer->InputPnt++ - '0';
                } while (IS_NUMBER(*buffer->InputPnt)); 
                
                command->ChannelHandle(buffer, channel);
            }
            else
            {
                // Developer error, command is missing handle
                while (true);
            }
            
            // Check for more commands at this level
            if (*buffer->InputPnt == ';')
            {
                ++buffer->InputPnt;
                
                // Check if returning to root
                if (*buffer->InputPnt == ':')
                {
                    if (isRoot)
                    {
                        // Prep to run the next command
                        ++buffer->InputPnt;
                    }
                    else
                    {
                        // Back up to the ';'
                        --buffer->InputPnt;
                        return;
                    }
                }
                
                *buffer->OutputPnt++ = ',';
                i = -1;
                continue;
            }
            
            return;
        }
    }
}

void ByteToHexString(CliBuffer *buffer, uint8_t b)
{
    *buffer->OutputPnt++ = '0';
    *buffer->OutputPnt++ = 'x';

    uint8_t upperNibble = (b >> 4) & 0x0F;
    *buffer->OutputPnt++ = upperNibble + (upperNibble > 0x09 ? '7' : '0');

    uint8_t lowerNibble = (b & 0x0F);
    *buffer->OutputPnt++ = lowerNibble + (lowerNibble > 0x09 ? '7' : '0');
}

const uint16_t decades[] = { 10000, 1000, 100, 10, 1 };
void IntToString(CliBuffer *buffer, uint16_t input)
{    
    if (input == 0)
    {
        *buffer->OutputPnt++ = '0';
        return;
    }
    else
    {
        const uint16_t* d = decades;
        // Figure out when to start
        while (*d > input)
        {
            ++d;
        }

        while (d < &decades[5])
        {
            char c = '0';

            while (input >= *d)
            {
                input -= *d;
                ++c;
            }

            ++d;
            *buffer->OutputPnt++ = c;
        }
    }
}

/**
 * Converts as many chars as possible to numbers
 * @param str
 * @return -1 if no numbers were found;
 */
int16_t ParseInt(char** str)
{
    if (!IS_NUMBER(**str))
        return -1;
    
    uint16_t result = 0;
    
    // Check if hex or int
    if (**str == '0')
    {
        ++(*str);
        
        if (**str == 'x' || **str == 'X')
        {
            ++(*str);
            // Parse as HEX
            
            while (true)
            {
                char c = **str;
                
                if (IS_NUMBER(c))
                {
                    result = (result << 4) | (c - '0');
                }
                else
                {
                    // ToUpper whatever we have here
                    char c = c & 0x20;
                    
                    if (c >= 'A' && c <= 'F')
                    {
                        result = (result << 4) | (c - '7');
                    }
                    else
                    {
                        break;
                    }
                }
                
                ++(*str);
            }            
        }
        else
        {
            --(*str);
        }
    }
    else
    {
        while (IS_NUMBER(**str))
        {
            result *= 10;
            result += **str - '0';
            
            ++(*str);
        }
    }
    
    return (int16_t)result;
}

void CopyWordToOutBuffer(CliBuffer *buffer, const char* word)
{
    while (*word)
    {
        *buffer->OutputPnt++ = *word++;
    }
}

// Put the commands that have the most branches towards the top
const CommandDefinition commands[] = {
    DEFINE_COMMAND("PWMO", PWMOutputs),
    DEFINE_COMMAND("SERV", ServoOutputs),
    DEFINE_COMMAND("DIGO", DigitalOutputs),
    DEFINE_COMMAND("ANAO", AnalogOutputs),
    DEFINE_COMMAND("ANAI", AnalogInputs),
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
        *buffer->OutputPnt++ = '\r';
        *buffer->OutputPnt++ = '\n';
        *buffer->OutputPnt = '\x00';
    }
}