
#include "../CLI/cli.h"
#include "../constants.h"
#include "../settings.h"
#include "../outputs.h"
#include "../buildInfo.h"

void SYSTSerilalNumber(CliBuffer_t *buffer, void* v)
{
    QuietSettings_t settings = GetSettings();
    
    if (*buffer->InputPnt == ' ')
    {
        ++buffer->InputPnt;
        
        if (*buffer->InputPnt == '"')
        {
            ++buffer->InputPnt;
            
            bool foundEnd = false;
            
            char *c = settings.SerialNumber;
            
            for (uint8_t i = 0; i < (sizeof(settings.SerialNumber) - 1) && !foundEnd; ++i)
            {
                switch (*buffer->InputPnt)
                {
                    case '"':
                        foundEnd = true;
                        break;
                    
                    default:
                        *c++ = *buffer->InputPnt;
                        break;
                }
                
                ++buffer->InputPnt;
            }
            
            if (foundEnd)
            {
                *c++ = '\x00';
                SetSettings(settings);
            }
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
    }
    else
    {
        RestoreSettings(false);
    }
}

void SYSTSave(CliBuffer_t *buffer, void* v)
{
    SaveSettings();
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

CommandDefinition_t SYSTemChildrenCommands[] = {
    DEFINE_COMMAND("REST", SYSTRestore),
    DEFINE_COMMAND("SAVE", SYSTSave),
    DEFINE_COMMAND("SERI", SYSTSerilalNumber),
    DEFINE_COMMAND("NUMB", SYSTNumberCommand),
    DEFINE_BRANCH("INFO", InfoChildrenCommands),
};

CommandDefinition_t SYSTemCommand = DEFINE_BRANCH("SYST", SYSTemChildrenCommands);
