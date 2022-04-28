
#include <stdbool.h>
#include <stdint.h>

#include <xc.h>
#include "../mcc_generated_files/tmr1.h"

#include "cli.h"
#include "../constants.h"

#include "../Commands/standardCommands.h"

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
extern CommandDefinition_t StarCommand;

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
            case ':':   // 0x3B
            case ' ':   // 0x20
            case '?':   // 0x3F
            case ';':   // 0x3A
            case '\r':  // 0x0D
            case '\n':  // 0x0A
                return;
        }

        ++(*input);
    }
}


void ProcessCommand(CliBuffer_t *buffer, CommandDefinition_t* commands)
{
    CommandDefinition_t* commandList;
    CommandDefinition_t* command;

    commandList = commands;

    if (*buffer->InputPnt == '*')
    {
        ++buffer->InputPnt;
        commandList = StarCommand.Children;
    }
    
    command = commandList;
    
    char *inputEnd = &buffer->InputBuffer[buffer->InputLength];
    
    bool lockNumber = false;
    uint8_t number;
    
    while (true)
    {
        if (!lockNumber)
        {
            // Only parse the number once per command string.
            number = 0;
        }
        
        const char* commandName = command->Command;
        
        if (SCPICompare(commandName, buffer->InputPnt))
        {
            // FF to the end of the command
            while (*commandName++) ++buffer->InputPnt;
            
            while (true)
            {
                char c = *buffer->InputPnt++;
                //      0x3A
                if (c == ':')           // Branch Deeper
                {
                    if (number > 0)
                        lockNumber = true;
                    
                    if (command->Children)
                    {
                        command = commandList = command->Children;
                    }
                    else
                    {
                        QueueErrorCode(CLI_ERROR_INVALID_BRANCH);
                    }
                    break;
                }
                //           0x3F        0x20        0x3B         0x0D        0x0A
                else if (c == '?' || c == ' ' || c == ';' || c == '\r' || c == '\n')      // Get a value
                {
                    --buffer->InputPnt;
                    if (command->Handle)
                    {
                        char* inputPnt = buffer->InputPnt;
                        command->Handle(buffer, &number);

                        // If the pointer didn't move, the command was no processed
                        if (inputPnt == buffer->InputPnt)
                        {
                            QueueErrorCode(CLI_ERROR_INVALID_COMMAND);
                        }

                        FFTilPunctuation(&buffer->InputPnt);
                        c = *buffer->InputPnt;
                    }
                    else
                    {
                        QueueErrorCode(CLI_ERROR_INVALID_COMMAND);
                    }
                }
                else if (IS_NUMBER(c))      // Parse a number
                {
                    number *= 10;
                    number += c - '0';
                    continue;
                }
                
                // Check for end conditions
                        // 0x00         0x0D        0x0A
                if (c == '\x00' || c == '\r' || c == '\n' || buffer->InputPnt >= inputEnd)
                {
                    return;
                }
                
                        // 0x3B
                if (c == ';')
                {
                    *buffer->OutputPnt++ = ';';
                    ++buffer->InputPnt;
                    
                    if (*buffer->InputPnt == ':')
                    {
                        ++buffer->InputPnt;
                        command = commands;
                        commandList = commands;
                        
                        lockNumber = false;
                    }
                    else
                    {
                        command = commandList;
                    }
                    
                    break;
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

void CheckValidNumberEnd(char c)
{
    if (c == ';' || c == '\r' || c == '\n' || c == '\x00')
    {
        return;
    }
    else
    {
        __asm("pop");
        __asm("pop");
        QueueErrorCode(CLI_ERROR_INVALID_NUMBER);
        return;
    }
}

bool ParseBool(char** str)
{
    char c = **str;
    bool result;

    if (c == '0')
    {
        ++str;
        result = false;
    }
    else if (c == '1')
    {
        ++str;
        result = true;
    }
    else if (c == 'F')
    {
        FFTilPunctuation(str);
        result = false;
    }
    else if (c == 'T')
    {
        FFTilPunctuation(str);
        result = true;
    }
    else
    {
        QueueErrorCode(CLI_ERROR_INVALID_BOOLEAN);
        __asm("pop");
    }

    return result;
}

/**
 * Converts as many chars as possible to numbers
 * @param str
 * @return The number found
 */
int16_t ParseInt(char** str)
{
    if (!IS_NUMBER(**str))
    {
        QueueErrorCode(CLI_ERROR_INVALID_NUMBER);
        // Pop past the call to this function
        __asm("pop");
        return 0;
    }
    
    uint16_t result = 0;
    
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
                        CheckValidNumberEnd(c);
                        return (int16_t)result;
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
    
    CheckValidNumberEnd(**str);
    return (int16_t)result;
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

    CheckValidNumberEnd(**str);
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
                QueueErrorCode(CLI_ERROR_INVALID_IEEE_HEADER);
                __asm("pop");
                return 0;
            }
        }
        
        return result; 
    }
    else
    {
        QueueErrorCode(CLI_ERROR_INVALID_IEEE_HEADER);
        __asm("pop");
        return 0;
    }
}

#define CLI_ERROR_BUFFER_SIZE 16
uint16_t cliErrorBuffer[CLI_ERROR_BUFFER_SIZE];
uint16_t *cliErrorInPnt = cliErrorBuffer;
uint16_t *cliErrorOutPnt = cliErrorBuffer;
unsigned cliErrorBufferOverflow = false;

void QueueErrorCode(uint16_t error)
{
    if (!cliErrorBufferOverflow)
    {
        *cliErrorInPnt++ = error;

        // Handle pointer rollover
        if (cliErrorInPnt >= &cliErrorBuffer[CLI_ERROR_BUFFER_SIZE])
        {
            cliErrorInPnt = cliErrorBuffer;
        }

        if (cliErrorInPnt == cliErrorOutPnt)
        {
            cliErrorBufferOverflow = true;
        }
    }
}

uint16_t DequeueErrorCode(void)
{
    if (cliErrorOutPnt == cliErrorInPnt)
    {
        if (cliErrorBufferOverflow)
        {
            cliErrorBufferOverflow = false;
            return ERROR_CODE_ERROR_BUFFER_OVERFLOW;
        }

        return ERROR_CODE_NO_ERROR;
    }
    else
    {
        uint16_t result = *cliErrorOutPnt;
        *cliErrorOutPnt++ = ERROR_CODE_NO_ERROR;

        // Handle pointer rollover
        if (cliErrorOutPnt >= &cliErrorBuffer[CLI_ERROR_BUFFER_SIZE])
        {
            cliErrorOutPnt = cliErrorBuffer;
        }

        return result;
    }
}

void ClearAllErrors(void)
{
    cliErrorOutPnt = cliErrorInPnt = cliErrorBuffer;
    cliErrorBufferOverflow = false;
    *cliErrorInPnt = ERROR_CODE_NO_ERROR;
}

void CopyWordToOutBuffer(CliBuffer_t *buffer, const char* word)
{
    while (*word)
    {
        *buffer->OutputPnt++ = *word++;
    }
}

volatile uint8_t stackPnt;
void ProcessCLI(CliBuffer_t *buffer, CommandDefinition_t* commands)
{    
    TMR1_WriteTimer(0x0000);
    TMR1_StartTimer();
    stackPnt = STKPTR;
    buffer->InputPnt = buffer->InputBuffer;
    buffer->OutputPnt = buffer->OutputBuffer;
    
    *buffer->OutputPnt = 0x00;
    
    ProcessCommand(buffer, commands);
    
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
uint24_t *TheStackPnt;

void SetLargeDataHandle(CliBuffer_t *buffer, void(*handle)(CliBuffer_t *buffer, void *v))
{    
    buffer->DataHandle = handle;
    
    TheStackPnt = TheStack;
            
    uint8_t intConSto = INTCON;     // Disable interrupts
    INTCON = 0x00;
    
    // Pop the stack until we get back to Process CLI
    while (STKPTR > stackPnt)
    {
        *TheStackPnt++ = TOS;
        __asm("pop");
    }
    
    *TheStackPnt++ = TOS;
    
    INTCON |= intConSto;            // Restore Interrupts
}

void ClearLargeDataHandle(CliBuffer_t *buffer)
{
    buffer->DataHandle = 0x0000;
    
    uint8_t intConSto = INTCON;     // Disable interrupts
    INTCON = 0x00;
    
    // Pop the stack until we get back to Process CLI
    while (STKPTR > stackPnt)
    {
        __asm("pop");
    }
    
    // Restore the stack
    while (TheStackPnt >= TheStack)
    {
        ++STKPTR;
        TOSU = (uint8_t)(*TheStackPnt >> 16);
        TOSH = (uint8_t)(*TheStackPnt >> 8);
        TOSL = (uint8_t)(*TheStackPnt--);
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
                Int14ToString(buffer, (uint16_t)input);
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