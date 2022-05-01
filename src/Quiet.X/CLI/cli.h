/*
 * File:   i.h
 * Author: callwyat
 *
 * Created on October 17, 2021, 2:41 PM
 */

#ifndef CLI_H
#define CLI_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

#define CLI_WORD_SIZE 24
    typedef enum
    {
        HexFormat,
        DecimalFormat,
    } NumberFormat_e;

    typedef union CliHandle_t
    {
        struct
        {
            char LastRead;
            char LastWord[CLI_WORD_SIZE];
            char (*Read)(void);
            void (*Write)(char);
            uint8_t (*GetReceivedCount)(void);
            char *ReceivePnt;
        };
    } CliHandle_t;

#define DEFINE_CLI_HANDLE(getRxCount, read, write)   \
{                                                    \
    .Read = read,                                    \
    .Write = write,                                  \
    .GetReceivedCount = getRxCount,                  \
}

    typedef void (*CommandHandle)(CliHandle_t *handle, void *channel);

    typedef struct CommandDefinition
    {
        const char Command[5]; // Needs to hold four letters and a null

        struct CommandDefinition *Children;
        uint8_t ChildrenCount;

        CommandHandle Handle;

    } CommandDefinition_t;

#define DEFINE_COMMAND(command, handle) \
    {                                   \
        .Command = command,             \
        .Handle = handle,               \
        .Children = 0x0000,             \
        .ChildrenCount = 0,             \
    }

#define DEFINE_BRANCH(command, children)                         \
    {                                                            \
        .Command = command,                                      \
        .Handle = 0x0000,                                        \
        .Children = &children[0],                                \
        .ChildrenCount = sizeof(children) / sizeof(children[0]), \
    }

#define DEFINE_COMMAND_W_BRANCH(command, handle, children)       \
    {                                                            \
        .Command = command,                                      \
        .Handle = handle,                                        \
        .Children = &children[0],                                \
        .ChildrenCount = sizeof(children) / sizeof(children[0]), \
    }

    bool SCPICompare(const char *reference, char *input);

    void ProcessCLI(CliHandle_t *handle, CommandDefinition_t *commands);

    void ProcessCommand(CliHandle_t *handle, CommandDefinition_t *commands);

    void SetNumberFormat(NumberFormat_e format);

    NumberFormat_e GetNumberFormat(void);

    void WriteNumber(CliHandle_t *handle, uint24_t input);

    char inline ReadChar(CliHandle_t *handle);

    char *ReadWord(CliHandle_t *handle);

    bool ReadBool(CliHandle_t *handle);

    uint16_t ReadInt(CliHandle_t *handle);

    uint24_t ReadInt24(CliHandle_t *handle);

    uint16_t ReadIEEEHeader(CliHandle_t *handle);

    void WriteIEEEHeader(CliHandle_t *handle, uint16_t dataSize);

    void QueueErrorCode(uint16_t);

    uint16_t DequeueErrorCode(void);

    void ClearAllErrors(void);

    void WriteChar(CliHandle_t *handle, char c);

    void WriteString(CliHandle_t *handle, const char *word);

#ifdef __cplusplus
}
#endif

#endif /* CLI_H */
