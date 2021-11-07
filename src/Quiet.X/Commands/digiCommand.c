
#include "../CLI/cli.h"
#include "../constants.h"

void DigitalInputs(CliBuffer *buffer)
{
    if (*buffer->InputPnt == '?')
    {
        //Progress the pointer past the query
        ++buffer->InputPnt;
        ByteToHexString(buffer, DIN);
    }
}
