
#include "../CLI/cli.h"
#include "../constants.h"
#include "../settings.h"
#include "../outputs.h"

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

const CommandDefinition_t SYSTemChildrenCommands[] = {
    DEFINE_COMMAND("REST", SYSTRestore),
    DEFINE_COMMAND("SAVE", SYSTSave),
    DEFINE_COMMAND("SERI", SYSTSerilalNumber),
    DEFINE_COMMAND("NUMB", SYSTNumberCommand),
};

const CommandDefinition_t SYSTemCommand = DEFINE_BRANCH("SYST", SYSTemChildrenCommands);
