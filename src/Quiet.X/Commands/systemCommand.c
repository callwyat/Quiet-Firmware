
#include "../CLI/cli.h"
#include "../constants.h"
#include "../settings.h"
#include "../outputs.h"

void SerialNumberCommand(CliBuffer_t *buffer)
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

void RestoreCommand(CliBuffer_t *buffer)
{
    RestoreSettings();
}

void SaveCommand(CliBuffer_t *buffer)
{
    SaveSettings();
}

const CommandDefinition systemCommands[] = {
  DEFINE_COMMAND("REST", RestoreCommand),
  DEFINE_COMMAND("SAVE", SaveCommand),
  DEFINE_COMMAND("SERI", SerialNumberCommand),
};

const uint8_t systemCommandCount = sizeof(systemCommands) / sizeof(systemCommands[0]);

void SystemCommand(CliBuffer_t *buffer)
{
    if (*buffer->InputPnt == ':')
    {
        ++buffer->InputPnt;
        ProcessCommand(systemCommands, systemCommandCount, buffer, false);
    }
}