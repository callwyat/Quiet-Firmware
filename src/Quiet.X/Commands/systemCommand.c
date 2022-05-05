
#include "../CLI/cli.h"
#include "../constants.h"
#include "../settings.h"
#include "../outputs.h"
#include "../buildInfo.h"

#include "anaoCommand.h"
#include "uartCommand.h"
#include "i2cCommand.h"

void SYSTErrorCommand(CliHandle_t *handle, void *v)
{
    if (handle->LastRead == '?')
    {
        ReadChar(handle);
        WriteNumber(handle, DequeueErrorCode());
    }
}

void SYSTErrorClearCommand(CliHandle_t *handle, void *v)
{
    ClearAllErrors();
}

void SYSTSerilalNumber(CliHandle_t *handle, void *v)
{
    QuietSettings_t settings = GetSettings();

    if (handle->LastRead == ' ')
    {
        ReadChar(handle);

        if (handle->LastRead == '"')
        {
            char *c = settings.SerialNumber;

            uint8_t i = 0;
            while (true)
            {
                char ch = ReadChar(handle);
                if (ch == '"')
                {
                    SetSettings(settings);
                    break;
                }
                else if (i++ >= sizeof(settings.SerialNumber))
                {
                    QueueErrorCode(ERROR_CODE_SERIAL_NUMBER_TO_LONG);
                    break;
                }
                else
                {
                    *c++ = ch;
                }
            }
        }
        else
        {
            QueueErrorCode(ERROR_CODE_INVALID_SERIAL_NUMBER_TYPE);
        }
    }
    else if (handle->LastRead == '?')
    {
        ReadChar(handle);
        WriteChar(handle, '"');
        char *c = settings.SerialNumber;

        while (*c)
        {
            WriteChar(handle, *c++);
        }
        WriteChar(handle, '"');
    }
}

void SYSTRestore(CliHandle_t *handle, void *v)
{
    if (handle->LastRead == ' ')
    {
        ReadWord(handle);
        if (SCPICompare("FACT", handle->LastWord))
        {
            RestoreSettings(true);
        }
        else
        {
            QueueErrorCode(ERROR_CODE_INVALID_RESTORE_ARG);
        }
    }
    else
    {
        RestoreSettings(false);
    }
}

void SYSTSave(CliHandle_t *handle, void *v)
{
    SaveSettings();
}

const char *HEXString = "HEX";
const char *DECIString = "DECI";

void SYSTNumberCommand(CliHandle_t *handle, void *v)
{
    if (handle->LastRead == ' ')
    {
        ReadWord(handle);
        if (SCPICompare(DECIString, handle->LastWord))
        {
            SetNumberFormat(DecimalFormat);
        }
        else if (SCPICompare(HEXString, handle->LastWord))
        {
            SetNumberFormat(HexFormat);
        }
        else
        {
            QueueErrorCode(ERROR_CODE_INVALID_NUMBER_MODE);
        }
    }
    else if (handle->LastRead == '?')
    {
        ReadChar(handle);
        const char *word = "";

        switch (GetNumberFormat())
        {
        case DecimalFormat:
            word = DECIString;
            break;
        case HexFormat:
            word = HEXString;
            break;
        }

        WriteString(handle, word);
    }
}

#define INFO_STRING_METHOD(name, input)     \
    void name(CliHandle_t *handle, void *v) \
    {                                       \
        if (handle->LastRead == '?')        \
        {                                   \
            ReadChar(handle);               \
            WriteChar(handle, '"');         \
            const char *c = input;          \
                                            \
            while (*c)                      \
            {                               \
                WriteChar(handle, *c++);    \
            }                               \
                                            \
            WriteChar(handle, '"');         \
        }                                   \
    }

INFO_STRING_METHOD(COMMitHash, COMMIT_HASH)
INFO_STRING_METHOD(COMMitAuthor, COMMIT_AUTHOR)
INFO_STRING_METHOD(COMMitDate, COMMIT_DATE)

CommandDefinition_t COMMitInfoChildrenCommands[] = {
    DEFINE_COMMAND("HASH", COMMitHash),
    DEFINE_COMMAND("AUTH", COMMitAuthor),
    DEFINE_COMMAND("DATE", COMMitDate),
};

INFO_STRING_METHOD(BUILdDate, BUILD_DATE)
INFO_STRING_METHOD(BUILdUser, BUILD_USER)

void BUILdVersion(CliHandle_t *handle, void *v)
{
    if (handle->LastRead == '?')
    {
        ReadChar(handle);
        WriteChar(handle, '"');
        WriteNumber(handle, __XC8_VERSION);
        WriteChar(handle, '"');
    }
}

CommandDefinition_t BUILdInfoChildrenCommands[] = {
    DEFINE_COMMAND("USER", BUILdUser),
    DEFINE_COMMAND("DATE", BUILdDate),
    DEFINE_COMMAND("VERS", BUILdVersion),
};

CommandDefinition_t InfoChildrenCommands[] = {
    DEFINE_BRANCH("COMM", COMMitInfoChildrenCommands),
    DEFINE_BRANCH("BUIL", BUILdInfoChildrenCommands),
};

CommandDefinition_t SYSTErrorChildren[] = {
    DEFINE_COMMAND("NEXT", SYSTErrorCommand),
    DEFINE_COMMAND("CLEA", SYSTErrorClearCommand),
};

CommandDefinition_t SYSTemChildrenCommands[] = {
    DEFINE_COMMAND_W_BRANCH("ERR", SYSTErrorCommand, SYSTErrorChildren),
    DEFINE_COMMAND("REST", SYSTRestore),
    DEFINE_COMMAND("SAVE", SYSTSave),
    DEFINE_COMMAND("SERI", SYSTSerilalNumber),
    DEFINE_COMMAND("NUMB", SYSTNumberCommand),
    DEFINE_BRANCH("INFO", InfoChildrenCommands),
};

CommandDefinition_t SYSTemCommand = DEFINE_BRANCH("SYST", SYSTemChildrenCommands);
