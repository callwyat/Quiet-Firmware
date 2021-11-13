
#include "../constants.h"
#include "../CLI/cli.h"
#include "../settings.h"

void IDNCommand(CliBuffer_t *buffer)
{
    if (*buffer->InputPnt == '?')
    {
        //Progress the pointer past the query
        ++buffer->InputPnt;
        
        const char *cc = MakeString;
        while (*cc != 0x00) *buffer->OutputPnt++ = *cc++;
        *buffer->OutputPnt++ = ',';

        cc = ModelString;
        while (*cc != 0x00) *buffer->OutputPnt++ = *cc++;
        *buffer->OutputPnt++ = ',';

        QuietSettings_t settings = GetSettings();
        char *snc = settings.SerialNumber;
        while (*snc != 0x00) *buffer->OutputPnt++ = *snc++;
        *buffer->OutputPnt++ = ',';

        cc = VersionString;
        while (*cc != 0x00) *buffer->OutputPnt++ = *cc++;   
    }
}

void RSTCommand(CliBuffer_t *buffer)
{
    RestoreSettings(false); 
}

const CommandDefinition starCommands[] = {
  DEFINE_COMMAND("IDN", IDNCommand),
  DEFINE_COMMAND("RST", RSTCommand),
};

const uint8_t starCommandsCount = sizeof(starCommands) / sizeof(starCommands[0]);

void StarCommand(CliBuffer_t *buffer)
{
    // Process command will have fast forwarded, we must back up to the star
    
    while (*buffer->InputPnt != '*')
    {
        --buffer->InputPnt;
    }
    
    ++buffer->InputPnt;
    
    ProcessCommand(starCommands, starCommandsCount, buffer, false);
}
