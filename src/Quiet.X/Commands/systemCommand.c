
#include "../CLI/cli.h"
#include "../constants.h"
#include "../settings.h"
#include "../outputs.h"

void SERIalNumberCommand(CliBuffer_t *buffer)
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

void RESToreCommand(CliBuffer_t *buffer)
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

void SAVECommand(CliBuffer_t *buffer)
{
    SaveSettings();
}

const CommandDefinition systemCommands[] = {
  DEFINE_COMMAND("REST", RESToreCommand),
  DEFINE_COMMAND("SAVE", SAVECommand),
  DEFINE_COMMAND("SERI", SERIalNumberCommand),
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