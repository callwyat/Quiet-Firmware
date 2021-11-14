
#include "../CLI/cli.h"
#include "../constants.h"
#include "../settings.h"
#include "../outputs.h"

void SYSTSerilalNumberCommand(CliBuffer_t *buffer)
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

void SYSTRestoreCommand(CliBuffer_t *buffer)
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

void SYSTSaveCommand(CliBuffer_t *buffer)
{
    SaveSettings();
}

const char* HEXString = "HEX";
const char* DECIString = "DECI";

void SYSTNumberCommand(CliBuffer_t *buffer)
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

const CommandDefinition systemCommands[] = {
  DEFINE_COMMAND("REST", SYSTRestoreCommand),
  DEFINE_COMMAND("SAVE", SYSTSaveCommand),
  DEFINE_COMMAND("SERI", SYSTSerilalNumberCommand),
  DEFINE_COMMAND("NUMB", SYSTNumberCommand),
};

const uint8_t systemCommandCount = sizeof(systemCommands) / sizeof(systemCommands[0]);

void SYSTemCommand(CliBuffer_t *buffer)
{
    if (*buffer->InputPnt == ':')
    {
        ++buffer->InputPnt;
        ProcessCommand(systemCommands, systemCommandCount, buffer, false);
    }
}