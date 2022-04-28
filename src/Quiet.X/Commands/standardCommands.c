
#include "../constants.h"
#include "../CLI/cli.h"
#include "../settings.h"

void IDNCommand(CliBuffer_t *buffer, void* v)
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

void RSTCommand(CliBuffer_t *buffer, void* v)
{
    ClearAllErrors();
    RestoreSettings(false);
    FFTilPunctuation(&buffer->InputPnt);
    
    // Convince the parser this there is no command error.
    --buffer->InputPnt;
}

CommandDefinition_t starCommands[] = {
  DEFINE_COMMAND("IDN", IDNCommand),
  DEFINE_COMMAND("RST", RSTCommand),
};

CommandDefinition_t StarCommand = DEFINE_BRANCH("*", starCommands);
