
#include "../constants.h"
#include "../CLI/cli.h"

void Identify(CliBuffer *buffer)
{
    if (IS_QUERY(buffer))
    {
        char *c = buffer->OutputBuffer;

        const char *cc = MakeString;
        while (*cc != 0x00) *c++ = *cc++;
        *c++ = ',';

        cc = ModelString;
        while (*cc != 0x00) *c++ = *cc++;
        *c++ = ',';

        cc = SerialNumberString;
        while (*cc != 0x00) *c++ = *cc++;
        *c++ = ',';

        cc = VersionString;
        while (*cc != 0x00) *c++ = *cc++;   
    }
}
