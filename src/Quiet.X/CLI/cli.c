

#include "cli.h"
#include "../constants.h"

#include "../Commands/standardCommands.h"
#include "../Commands/digiCommand.h"
#include "../Commands/digoCommand.h"
#include "../Commands/anaiCommand.h"
#include "../Commands/anaoCommand.h"
#include "../Commands/pwmCommand.h"
#include "../Commands/servoCommand.h"
#include "../Commands/uartCommand.h"
#include "../Commands/systemCommand.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <xc.h>
#include "../mcc_generated_files/tmr1.h"

#define IS_NUMBER(c) (c >= '0' && c <= '9')

bool SCPICompare(const char *reference, char *input)
{
    // Match upto the first 4 chars, or until reference is null
    for (int i = 0; i < 4; ++i)
    {        
        // To Upper
        if (*input >= 'a' && *input <= 'z')
        {
            *input -= 0x20;
        }
        
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
        CliBuffer_t *buffer, bool isRoot)
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

void ByteToHexString(CliBuffer_t *buffer, uint8_t b)
{
    *buffer->OutputPnt++ = '0';
    *buffer->OutputPnt++ = 'x';

    uint8_t upperNibble = (b >> 4) & 0x0F;
    *buffer->OutputPnt++ = upperNibble + (upperNibble > 0x09 ? '7' : '0');

    uint8_t lowerNibble = (b & 0x0F);
    *buffer->OutputPnt++ = lowerNibble + (lowerNibble > 0x09 ? '7' : '0');
}

const uint16_t decades14[] = { 1000, 100, 10, 1 };
#define DECADES14_LENGTH sizeof(decades14) / sizeof(decades14[0])

void Int14ToString(CliBuffer_t *buffer, uint16_t input)
{    
    if (input == 0)
    {
        *buffer->OutputPnt++ = '0';
        return;
    }
    else
    {
        const uint16_t* d = decades14;
        // Figure out when to start
        while (*d > input)
        {
            ++d;
        }

        while (d < &decades14[DECADES14_LENGTH])
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

const uint24_t decades24[] = { 10000000, 1000000, 100000, 10000, 1000, 100, 10, 1 };
#define DECADES24_LENGTH sizeof(decades24) / sizeof(decades24[0])
void Int24ToString(CliBuffer_t *buffer, uint24_t input)
{    
    if (input == 0)
    {
        *buffer->OutputPnt++ = '0';
        return;
    }
    else
    {
        const uint24_t* d = decades24;
        // Figure out when to start
        while (*d > input)
        {
            ++d;
        }

        while (d < &decades24[DECADES24_LENGTH])
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
    
    int16_t result = 0;
    
    // Check if hex or int
    if (**str == '0')
    {
        ++(*str);
        
        if (**str == 'X' || **str == 'x')
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
                    if (c >= 'A' && c <= 'F')
                    {
                        result = (result << 4) | (c - '7');
                    }
                    else if (c >= 'a' && c <= 'z'){
                        c -= 0x20;
                        result = (result << 4) | (c - '7');
                    }
                    else
                    {
                        return result;
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
    
    
    while (IS_NUMBER(**str))
    {
        result *= 10;
        result += **str - '0';

        ++(*str);
    }
    
    return result;
}

uint24_t ParseInt24(char** str)
{
    uint24_t result = 0;
    
    while (IS_NUMBER(**str))
    {
        result *= 10;
        result += **str - '0';

        ++(*str);
    }
    
    return result;
}

uint16_t ParseIEEEHeader(CliBuffer_t *buffer)
{
    if (IS_NUMBER(*buffer->InputPnt))
    {
        uint8_t headerSize = *buffer->InputPnt - '0';
        
        ++buffer->InputPnt;
        
        uint16_t result = 0;
        for (uint8_t i = 0; i < headerSize; ++i)
        {
            if (IS_NUMBER(*buffer->InputPnt))
            {
                result *= 10;
                result += *buffer->InputPnt++ - '0';
            }
            else
            {
                return 0;
            }
        }
        
        return result; 
    }
    else
    {
        return 0;
    }
}

void GenerateIEEEHeader(CliBuffer_t *buffer, uint16_t dataSize)
{
    *buffer->OutputPnt++ = '#';
    
    char *c = buffer->OutputPnt++;
    Int14ToString(buffer, dataSize);
    
    uint8_t dataHeaderSize = (uint8_t)(buffer->OutputPnt - c) - 1;
    
    *c = dataHeaderSize + '0';
}

void CopyWordToOutBuffer(CliBuffer_t *buffer, const char* word)
{
    while (*word)
    {
        *buffer->OutputPnt++ = *word++;
    }
}

uint16_t lastExecutionTime = 0;
void DIAGnosticsCommand(CliBuffer_t *buffer)
{
    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;

        Int14ToString(buffer, lastExecutionTime);
    }
}

// Put the commands that have the most branches towards the top
const CommandDefinition commands[] = {
    DEFINE_COMMAND("PWM", PWMCommand),
    DEFINE_COMMAND("SERV", SERVoCommand),
    DEFINE_COMMAND("DIGO", DIGOCommand),
    DEFINE_COMMAND("ANAO", ANAOCommand),
    DEFINE_COMMAND("ANAI", ANAICommand),
    DEFINE_COMMAND("DIGI", DIGICommand),
    DEFINE_COMMAND("SYST", SYSTemCommand),
    DEFINE_COMMAND("UART", UARTCommand),
    DEFINE_COMMAND("DIAG", DIAGnosticsCommand),
    DEFINE_COMMAND("*", StarCommand),
};

const uint8_t CommandCount = sizeof(commands) / sizeof(commands[0]);

volatile uint8_t stackPnt;
void ProcessCLI(CliBuffer_t *buffer)
{    
    TMR1_WriteTimer(0x0000);
    TMR1_StartTimer();
    stackPnt = STKPTR;
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

    TMR1_StopTimer();
    lastExecutionTime = TMR1_ReadTimer();
}

uint24_t TheStack[32];
uint24_t *TheStackPnt = TheStack;

void SetLargeDataHandle(CliBuffer_t *buffer, CommandHandle handle)
{    
    buffer->DataHandle = handle;
    
    uint8_t intConSto = INTCON;     // Disable interrupts
    INTCON = 0x00;
    
    // Pop the stack until we get back to Process CLI
    while (STKPTR > stackPnt)
    {
        *TheStackPnt++ = TOS;
        __asm("pop");
    }
    
    INTCON |= intConSto;            // Restore Interrupts
}

void ClearLargeDataHandle(CliBuffer_t *buffer)
{
    buffer->DataHandle = 0x0000;
    
    uint8_t intConSto = INTCON;     // Disable interrupts
    INTCON = 0x00;
    
    // Pop the stack until we get back to Process CLI
    while (TheStackPnt != TheStack)
    {
        TOS = *TheStackPnt--;
        __asm("push");
    }
    
    INTCON |= intConSto;            // Restore Interrupts
}
