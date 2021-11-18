

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
#include "../Commands/spiCommand.h"
#include "../Commands/systemCommand.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <xc.h>
#include "../mcc_generated_files/tmr1.h"

#define IS_NUMBER(c) (c >= '0' && c <= '9')

uint16_t lastExecutionTime = 0;
void DIAGnostics(CliBuffer_t *buffer, void* v)
{
    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;

        NumberToString(buffer, lastExecutionTime);
    }
}

CommandDefinition_t DIAGnosticsCommand = DEFINE_COMMAND("DIAG", DIAGnostics);

extern CommandDefinition_t PWMCommand;
extern CommandDefinition_t SERVoCommand;
extern CommandDefinition_t DIGOCommand;
extern CommandDefinition_t ANAOCommand;
extern CommandDefinition_t ANAICommand;
extern CommandDefinition_t DIGICommand;
extern CommandDefinition_t SYSTemCommand;
extern CommandDefinition_t UARTCommand;
extern CommandDefinition_t SPICommand;
extern CommandDefinition_t DIAGnosticsCommand;
extern CommandDefinition_t StarCommand;
            

// Put the commands that have the most branches towards the top
CommandDefinition_t commands[16];

void CliInit(void)
{
    commands[0] = PWMCommand;
    commands[1] = SERVoCommand;
    commands[2] = DIGOCommand;
    commands[3] = ANAOCommand;
    commands[4] = ANAICommand;
    commands[5] = DIGICommand;
    commands[6] = SYSTemCommand;
    commands[7] = UARTCommand;
    commands[8] = SPICommand;
    commands[9] = DIAGnosticsCommand;
    commands[10] = StarCommand;
}

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
    while (**input) // Handles "\x00"
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


void ProcessCommand(CliBuffer_t *buffer)
{
    const CommandDefinition_t* commandList = commands;
    const CommandDefinition_t* command = commandList;

    char *inputEnd = &buffer->InputBuffer[buffer->InputLength];
    
    bool valid = true;
    
    while (true)
    {
        if (SCPICompare(command->Command, buffer->InputPnt))
        {
            const char* commandName = command->Command;
            
            // FF to the end of the command
            while (*commandName++);
            
            // look for numbers or some punctuation
            uint8_t number = 0;
            
            while (true)
            {
                char c = *buffer->InputPnt;
                if (c == ':')           // Branch Deeper
                {
                    if (command->Children)
                    {
                        commandList = command->Children;
                        command = commandList;   
                    }
                    else
                    {
                        valid = false;
                    }
                    break;
                }
                else if (c == '?' || c == ' ' || c == ';'  || c == '\r' || c == '\n')      // Get a value
                {
                    buffer->InputPnt++;
                    if (command->Handle)
                    {
                        command->Handle(buffer, &number);
                        c = *buffer->InputPnt;
                    }
                    else
                    {
                        // Missing Handle
                        while (true);
                    }
                }
                else if (IS_NUMBER(c))      // Parse a number
                {
                    number *= 10;
                    number += c - '0';
                }
                
                // Check for end conditions
                if (c == '\x00' || c == '\r' || c == '\n' || buffer->InputPnt >= inputEnd)
                {
                    return;
                }
                
                if (c == ';')
                {
                    ++buffer->InputPnt;
                    if (*buffer->InputPnt == ':')
                    {
                        ++buffer->InputPnt;
                        command = commands;
                        commandList = commands;
                    }
                    else
                    {
                        --buffer->InputPnt;
                        command = commandList;
                    }
                }
            }
        }
        else
        {
            ++command;
            if (command->Command[0] == '\x00')
            {
                break;
            }
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

void CopyWordToOutBuffer(CliBuffer_t *buffer, const char* word)
{
    while (*word)
    {
        *buffer->OutputPnt++ = *word++;
    }
}

volatile uint8_t stackPnt;
void ProcessCLI(CliBuffer_t *buffer)
{    
    TMR1_WriteTimer(0x0000);
    TMR1_StartTimer();
    stackPnt = STKPTR;
    buffer->InputPnt = buffer->InputBuffer;
    buffer->OutputPnt = buffer->OutputBuffer;
    
    *buffer->OutputPnt = 0x00;
    
    ProcessCommand(buffer);
    
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

void SetLargeDataHandle(CliBuffer_t *buffer, void(*handle)())
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

volatile NumberFormat_e NumberFormat;
void SetNumberFormat(NumberFormat_e format)
{
    NumberFormat = format;
}

NumberFormat_e GetNumberFormat(void)
{
    return NumberFormat;
}
    
void Int24ToHexString(CliBuffer_t *buffer, uint24_t input)
{
    *buffer->OutputPnt++ = '0';
    *buffer->OutputPnt++ = 'x';

    uint8_t b = (uint8_t)(input >> 16);
    uint8_t nibble = 0x00;
    
    if (b > 0)
    {
        nibble = (b >> 4);
        *buffer->OutputPnt++ = nibble + (nibble > 0x09 ? '7' : '0');

        nibble = (b & 0x0F);
        *buffer->OutputPnt++ = nibble + (nibble > 0x09 ? '7' : '0'); 
        
        nibble = b;
    }
    
    b = (uint8_t)(input >> 8);
    
    if (b > 0 || nibble > 0)
    {
        nibble = (b >> 4);
        *buffer->OutputPnt++ = nibble + (nibble > 0x09 ? '7' : '0');

        nibble = (b & 0x0F);
        *buffer->OutputPnt++ = nibble + (nibble > 0x09 ? '7' : '0'); 
    }
    
    b = (uint8_t)(input);
    
    nibble = (b >> 4);
    *buffer->OutputPnt++ = nibble + (nibble > 0x09 ? '7' : '0');

    nibble = (b & 0x0F);
    *buffer->OutputPnt++ = nibble + (nibble > 0x09 ? '7' : '0');
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

void NumberToString(CliBuffer_t *buffer, uint24_t input)
{
    switch (NumberFormat)
    {
        case HexFormat:
            Int24ToHexString(buffer, input);
            break;
        case DecimalFormat:
            
            if (input < 10000)
                Int14ToString(buffer, input);
            else
                Int24ToString(buffer, input);
            
            break;
        default:
            while (true);
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