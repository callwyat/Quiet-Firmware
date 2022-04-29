
#include "../CLI/cli.h"
#include "../constants.h"
#include "../settings.h"
#include "../outputs.h"
#include "../buildInfo.h"

#include "anaoCommand.h"
#include "uartCommand.h"
#include "i2cCommand.h"

void SYSTErrorCommand(CliBuffer_t *buffer, void* v)
{
    if (*buffer->InputPnt == '?')              
    {                                          
        ++buffer->InputPnt;                    
                                                
        NumberToString(buffer, DequeueErrorCode());
    }    
}

void SYSTErrorClearCommand(CliBuffer_t *buffer, void* v)
{
    FFTilPunctuation(&buffer->InputPnt);
    ClearAllErrors();
    
    // Convince the parser this there is no command error.
    --buffer->InputPnt;
}

void SYSTSerilalNumber(CliBuffer_t *buffer, void* v)
{
    QuietSettings_t settings = GetSettings();
    
    if (*buffer->InputPnt == ' ')
    {
        ++buffer->InputPnt;
        
        if (*buffer->InputPnt == '"')
        {
            ++buffer->InputPnt;
            
            char *c = settings.SerialNumber;
            
            uint8_t i = 0;
            while (true)
            {
                char ch = *buffer->InputPnt++;
                if (ch == '"') {
                    SetSettings(settings);
                    break;
                } else if (i++ >= sizeof(settings.SerialNumber)) {
                    QueueErrorCode(ERROR_CODE_SERIAL_NUMBER_TO_LONG);
                    break;
                } else {
                    *c++ = ch;
                }
            }
        }
        else
        {
            QueueErrorCode(ERROR_CODE_INVALID_SERIAL_NUMBER_TYPE);
        }
    }
    else if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        
        *buffer->OutputPnt++ = '"';
        char *c = settings.SerialNumber;
        
        while (*c)
        {
            *buffer->OutputPnt++ = *c++;
        }
        
        *buffer->OutputPnt++ = '"';
    }
}

void SYSTRestore(CliBuffer_t *buffer, void* v)
{
    if (*buffer->InputPnt == ' ')
    {
        ++buffer->InputPnt;
        
        if (SCPICompare("FACT", buffer->InputPnt))
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
            
        // Convince the parser this there is no command error.
        --buffer->InputPnt;
    }
}

void SYSTSave(CliBuffer_t *buffer, void* v)
{
    SaveSettings();
    
    // Convince the parser this there is no command error.
    --buffer->InputPnt;
}

const char* HEXString = "HEX";
const char* DECIString = "DECI";

void SYSTNumberCommand(CliBuffer_t *buffer, void* v)
{
    if (*buffer->InputPnt == ' ')
    {
        ++buffer->InputPnt;
        
        if (SCPICompare(DECIString, buffer->InputPnt))
        {
            SetNumberFormat(DecimalFormat);
        }
        else if (SCPICompare(HEXString, buffer->InputPnt))
        {
            SetNumberFormat(HexFormat);
        }
        else
        {
            QueueErrorCode(ERROR_CODE_INVALID_NUMBER_MODE);
        }
        
        FFTilPunctuation(&buffer->InputPnt);
    }
    else if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        
        const char* word = "";
        
        switch (GetNumberFormat())
        {
            case DecimalFormat:
                word = DECIString;
                break;
            case HexFormat:
                word = HEXString;
                break;
        }
        
        CopyWordToOutBuffer(buffer, word);
    }
}

#define INFO_STRING_METHOD(name, input)         \
void name(CliBuffer_t *buffer, void* v)         \
{                                               \
    if (*buffer->InputPnt == '?')               \
    {                                           \
        ++buffer->InputPnt;                     \
                                                \
        *buffer->OutputPnt++ = '"';             \
        const char *c = input;                  \
                                                \
        while (*c)                              \
        {                                       \
            *buffer->OutputPnt++ = *c++;        \
        }                                       \
                                                \
        *buffer->OutputPnt++ = '"';             \
    }                                           \
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

void BUILdVersion(CliBuffer_t *buffer, void* v) 
{                                               
    if (*buffer->InputPnt == '?')               
    {                                           
        ++buffer->InputPnt;                     
                                                
        *buffer->OutputPnt++ = '"';             
        NumberToString(buffer, __XC8_VERSION);
        *buffer->OutputPnt++ = '"';             
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
