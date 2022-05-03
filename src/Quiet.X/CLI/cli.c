
#include <stdbool.h>
#include <stdint.h>

#include <xc.h>
#include "../mcc_generated_files/tmr1.h"

#include "cli.h"
#include "../constants.h"

#include "../Commands/standardCommands.h"

#define IS_NUMBER(c) (c >= '0' && c <= '9')

extern CommandDefinition_t StarCommand;

uint16_t lastExecutionTime = 0;
bool wrote;
void DIAGnostics(CliHandle_t *handle, void *v)
{
    if (handle->LastRead == '?')
    {
        WriteNumber(handle, lastExecutionTime);
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

volatile NumberFormat_e NumberFormat;
void SetNumberFormat(NumberFormat_e format)
{
    NumberFormat = format;
}

NumberFormat_e GetNumberFormat(void)
{
    return NumberFormat;
}

bool IsSCPIPunctuation(char c)
{
    if (c <= ' ')
    {
        if (c <= '\n')
        {
            return c == '\n' || c == '\x00';
        }
        else
        {
            return c == ' ' || c == '\r';
        }
    }
    else
    {
        if (c <= ':')
        {
            return c == ':' || c == '#';
        }
        else
        {
            return c == '?' || c == ';';
        }
    }
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

char inline ReadChar(CliHandle_t *handle)
{
    return handle->LastRead = handle->Read();
}

char *ReadWord(CliHandle_t *handle)
{
    char *c = handle->LastWord;
    uint8_t i;
    for (i = 0; i < CLI_WORD_SIZE; ++i)
    {
        *c = handle->Read();

        if (IsSCPIPunctuation(*c++))
        {
            break;
        }
    }

    if (i >= CLI_WORD_SIZE)
    {
        QueueErrorCode(CLI_INVALID_WORD);
        __asm("pop");
    }

    *c-- = '\x00';
    handle->LastRead = *c;

    return handle->LastWord;
}

bool ReadWordWithNumber(CliHandle_t *handle, uint8_t *output)
{
    char *c = handle->LastWord;
    uint8_t i;
    uint8_t number = 0xFF;
    for (i = 0; i < CLI_WORD_SIZE; ++i)
    {
        *c = handle->Read();

        if (IsSCPIPunctuation(*c))
        {
            break;
        }
        else if (IS_NUMBER(*c))
        {
            number = number == 0xFF ? 0 : number * 10;
            number += (*c - '0');
        }

        ++c;
    }

    if (i >= CLI_WORD_SIZE)
    {
        QueueErrorCode(CLI_INVALID_WORD);
        __asm("pop");
    }

    ++c;
    *c-- = '\x00';
    handle->LastRead = *c;

    if (number != 0xFF)
    {
        *output = number;
        return true;
    }
    else
    {
        return false;
    }
}

bool ReadBool(CliHandle_t *handle)
{
    char *c = ReadWord(handle);
    bool result;

    if (*c == '0')
    {
        result = false;
    }
    else if (*c == '1')
    {
        result = true;
    }
    else if (*c == 'F')
    {
        result = false;
    }
    else if (*c == 'T')
    {
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
uint16_t ReadInt(CliHandle_t *handle)
{
    char *str = ReadWord(handle);

    if (!IS_NUMBER(*str))
    {
        QueueErrorCode(CLI_ERROR_INVALID_NUMBER);
        // Pop past the call to this function
        __asm("pop");
        return 0;
    }

    uint16_t result = 0;

    // Check if hex or int
    if (*str == '0')
    {
        ++str;

        if (*str == 'X' || *str == 'x')
        {
            ++str;
            // Parse as HEX

            while (true)
            {
                char c = *str;

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
                    else if (c >= 'a' && c <= 'z')
                    {
                        c -= 0x20;
                        result = (result << 4) | (c - '7');
                    }
                    else
                    {
                        CheckValidNumberEnd(c);
                        return result;
                    }
                }

                ++str;
            }
        }
        else
        {
            --str;
        }
    }

    while (IS_NUMBER(*str))
    {
        result *= 10;
        result += *str - '0';

        ++str;
    }

    CheckValidNumberEnd(*str);
    return result;
}

uint24_t ReadInt24(CliHandle_t *handle)
{
    uint24_t result = 0;
    char *str = ReadWord(handle);

    while (IS_NUMBER(*str))
    {
        result *= 10;
        result += *str - '0';

        ++str;
    }

    CheckValidNumberEnd(*str);
    return result;
}

uint16_t ReadIEEEHeader(CliHandle_t *handle)
{
    char c = handle->Read();

    if (IS_NUMBER(c))
    {
        uint8_t headerSize = c - '0';

        c = handle->Read();

        uint16_t result = 0;
        for (uint8_t i = 0; i < headerSize; ++i)
        {
            if (IS_NUMBER(c))
            {
                result *= 10;
                result += c - '0';
                c = handle->Read();
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

void WriteChar(CliHandle_t *handle, char c)
{
    handle->Write(c);
    wrote = true;
}

void WriteString(CliHandle_t *handle, const char *word)
{
    while (*word)
    {
        handle->Write(*word++);
    }

    wrote = true;
}

void WriteHex24(CliHandle_t *handle, uint24_t input)
{
    wrote = true;

    handle->Write('0');
    handle->Write('x');

    uint8_t b = (uint8_t)(input >> 16);
    uint8_t nibble = 0x00;

    if (b > 0)
    {
        nibble = (b >> 4);
        handle->Write(nibble + (nibble > 0x09 ? '7' : '0'));

        nibble = (b & 0x0F);
        handle->Write(nibble + (nibble > 0x09 ? '7' : '0'));

        nibble = b;
    }

    b = (uint8_t)(input >> 8);

    if (b > 0 || nibble > 0)
    {
        nibble = (b >> 4);
        handle->Write(nibble + (nibble > 0x09 ? '7' : '0'));

        nibble = (b & 0x0F);
        handle->Write(nibble + (nibble > 0x09 ? '7' : '0'));
    }

    b = (uint8_t)(input);

    nibble = (b >> 4);
    handle->Write(nibble + (nibble > 0x09 ? '7' : '0'));

    nibble = (b & 0x0F);
    handle->Write(nibble + (nibble > 0x09 ? '7' : '0'));
}

const uint16_t decades14[] = {1000, 100, 10, 1};
#define DECADES14_LENGTH sizeof(decades14) / sizeof(decades14[0])

void WriteInt14(CliHandle_t *handle, uint16_t input)
{
    wrote = true;
    if (input == 0)
    {
        handle->Write('0');
    }
    else
    {
        const uint16_t *d = decades14;
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
            handle->Write(c);
        }

        wrote = true;
    }
}

const uint24_t decades24[] = {10000000, 1000000, 100000, 10000, 1000, 100, 10, 1};
#define DECADES24_LENGTH sizeof(decades24) / sizeof(decades24[0])
void WriteInt24(CliHandle_t *handle, uint24_t input)
{
    wrote = true;
    if (input == 0)
    {
        handle->Write('0');
    }
    else
    {
        const uint24_t *d = decades24;
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
            handle->Write(c);
        }
    }
}

void WriteNumber(CliHandle_t *handle, uint24_t input)
{
    switch (NumberFormat)
    {
    case HexFormat:
        WriteHex24(handle, input);
        break;
    case DecimalFormat:

        if (input < 10000)
            WriteInt14(handle, (uint16_t)input);
        else
            WriteInt24(handle, input);

        break;
    default:
        while (true)
            ;
    }
}

void WriteIEEEHeader(CliHandle_t *handle, uint16_t dataSize)
{
    wrote = true;
    handle->Write('#');

    char headerSize = 4;
    const uint16_t *decadePnt = decades14;

    while (dataSize < *decadePnt++)
    {
        --headerSize;
    }

    handle->Write(headerSize + '0');

    WriteInt14(handle, dataSize);
}

void ProcessCommand(CliHandle_t *handle, CommandDefinition_t *rootCommand)
{
    CommandDefinition_t *forkCommand, *command;

    command = forkCommand = rootCommand;
    uint8_t commandIndex;

    while (true)
    {
        if (SCPICompare(command->Command, handle->LastWord))
        {
            while (true)
            {
                char c = handle->LastRead;
                //      0x3A
                if (c == ':') // Branch Deeper
                {
                    if (command->Children)
                    {
                        command = forkCommand = command->Children;
                    }
                    else
                    {
                        QueueErrorCode(CLI_ERROR_INVALID_BRANCH);
                    }

                    ReadWordWithNumber(handle, &commandIndex);

                    break;
                }
                else if (command->Handle)
                {
                    command->Handle(handle, &commandIndex);
                    c = handle->LastRead;

                    if (c == '?')
                    {
                        ReadWordWithNumber(handle, &commandIndex);
                        c = handle->LastRead;
                    }

                    // Check for command chaining
                    if (c == ';')
                    {
                        WriteChar(handle, ';');
                        ReadWordWithNumber(handle, &commandIndex);

                        // Check for chain to root
                        if (handle->LastRead == ':')
                        {
                            // Reset the CommandIndex
                            commandIndex = 0;
                            ReadWord(handle);
                            command = forkCommand = rootCommand;
                        }
                        else
                        {
                            command = forkCommand;
                        }

                        c = handle->LastRead;
                        break;
                    }
                    // Check for end conditions
                    // 0x00         0x0D        0x0A
                    else if (c == '\x00' || c == '\r' || c == '\n')
                    {
                        return;
                    }
                }
                else
                {
                    QueueErrorCode(CLI_ERROR_INVALID_COMMAND);
                }
            }
        }
        else
        {
            ++command;
            if (command->Command[0] == '\x00')
            {
                if (handle->LastWord[0] == '*')
                {
                    command = forkCommand = StarCommand.Children;

                    // Shift the whole word left one.
                    char *shift1 = handle->LastWord;
                    char *shift2 = &handle->LastWord[1];
                    while (*shift2)
                    {
                        *shift1++ = *shift2++;
                    }
                    *shift1 = '\x00';
                }
                else
                {
                    break;
                }
            }
        }
    }
}

void ProcessCLI(CliHandle_t *handle, CommandDefinition_t *commands)
{
    while (handle->GetReceivedCount() > 0)
    {
        char c = *handle->ReceivePnt++ = handle->Read();

        if (IsSCPIPunctuation(c))
        {
            *handle->ReceivePnt = '\x00';
            handle->LastRead = c;
            handle->ReceivePnt = handle->LastWord;
            wrote = false;

            TMR1_WriteTimer(0x0000);
            TMR1_StartTimer();

            ProcessCommand(handle, commands);

            TMR1_StopTimer();

            // If something was written, make sure it is terminated
            if (wrote)
            {
                handle->Write('\r');
                handle->Write('\n');
            }

            lastExecutionTime = TMR1_ReadTimer();
        }
    }
}

CommandDefinition_t DIAGnosticsCommand = DEFINE_COMMAND("DIAG", DIAGnostics);
