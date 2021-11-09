
#include "../constants.h"
#include "../CLI/cli.h"
#include "../settings.h"

void Identify(CliBuffer *buffer)
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
