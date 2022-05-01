
#include <stdbool.h>
#include <stdint.h>

#include <xc.h>
#include "../mcc_generated_files/tmr1.h"

#include "cli.h"
#include "../constants.h"

#include "../Commands/standardCommands.h"

#define IS_NUMBER(c) (c >= '0' && c <= '9')

uint16_t lastExecutionTime = 0;
bool wrote;
void DIAGnostics(CliHandle_t *handle, void *v)
{
    if (handle->LastRead == '?')
    {
        PrintNumber(handle, lastExecutionTime);
    }
}

CommandDefinition_t DIAGnosticsCommand = DEFINE_COMMAND("DIAG", DIAGnostics);
extern CommandDefinition_t StarCommand;

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
        if (c <= '?')
        {
            return c == '?' || c == '#';
        }
        else
        {
            return c == ':' || c == ';';
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

void ProcessCommand(CliHandle_t *handle, CommandDefinition_t *rootCommand)
{
    CommandDefinition_t *forkCommand, *command;

    command = forkCommand = rootCommand;

    bool indexLock = false;
    uint8_t commandIndex;

    while (true)
    {
        if (handle->LastWord[0] == '*')
        {
            forkCommand = StarCommand.Children;
        }

        if (!indexLock)
        {
            // Only parse the commandIndex once per command string.
            commandIndex = 0;
        }

        if (SCPICompare(command->Command, handle->LastWord))
        {
            while (true)
            {
                char c = handle->LastRead;
                //      0x3A
                if (c == ':') // Branch Deeper
                {
                    if (commandIndex > 0)
                        indexLock = true;

                    if (command->Children)
                    {
                        command = forkCommand = command->Children;
                    }
                    else
                    {
                        QueueErrorCode(CLI_ERROR_INVALID_BRANCH);
                    }
                    break;
                }
                else if (IsSCPIPunctuation(c)) // Get a value
                {
                    if (command->Handle)
                    {
                        command->Handle(handle, &commandIndex);

                        // Check for end conditions
                        // 0x00         0x0D        0x0A
                        if (c == '\x00' || c == '\r' || c == '\n')
                        {
                            return;
                        }

                        ReadWord(handle);

                        if (handle->LastWord[0] == ';')
                        {
                            WriteChar(handle, ';');

                            ReadWord(handle);
                            if (handle->LastWord[0] == ':')
                            {
                                ReadWord(handle);
                                command = forkCommand = rootCommand;
                                indexLock = false;
                            }
                            else
                            {
                                command = forkCommand;
                            }

                            break;
                        }
                    }
                    else
                    {
                        QueueErrorCode(CLI_ERROR_INVALID_COMMAND);
                    }
                }
                else if (IS_NUMBER(c)) // Parse a number
                {
                    commandIndex *= 10;
                    commandIndex += c - '0';
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

void WriteString(CliHandle_t *handle, const char *word)
{
    while (*word)
    {
        handle->Write(*word++);
    }

    wrote = true;
}

void WriteChar(CliHandle_t *handle, char c)
{
    handle->Write(c);
    wrote = true;
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
        *c++ = handle->Read();

        if (IsSCPIPunctuation(*c))
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

uint16_t ParseIEEEHeader(CliHandle_t *handle)
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

volatile uint8_t stackPnt;
void ProcessCLI(CliHandle_t *handle, CommandDefinition_t *commands)
{
    while (handle->GetReceivedCount() > 0)
    {
        char c = *handle->ReceivePnt++ = handle->Read();

        if (IsSCPIPunctuation(c))
        {
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

volatile NumberFormat_e NumberFormat;
void SetNumberFormat(NumberFormat_e format)
{
    NumberFormat = format;
}

NumberFormat_e GetNumberFormat(void)
{
    return NumberFormat;
}

void PrintHex24(CliHandle_t *handle, uint24_t input)
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

void PrintInt14(CliHandle_t *handle, uint16_t input)
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
void PrintInt24(CliHandle_t *handle, uint24_t input)
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

void PrintNumber(CliHandle_t *handle, uint24_t input)
{
    switch (NumberFormat)
    {
    case HexFormat:
        PrintHex24(handle, input);
        break;
    case DecimalFormat:

        if (input < 10000)
            PrintInt14(handle, (uint16_t)input);
        else
            PrintInt24(handle, input);

        break;
    default:
        while (true)
            ;
    }
}

void GenerateIEEEHeader(CliHandle_t *handle, uint16_t dataSize)
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

    PrintInt14(handle, dataSize);
}