
#include "../CLI/cli.h"
#include "../constants.h"

void DigitalInputs(CliBuffer *buffer)
{
    if (IS_QUERY(buffer))
    {
        //Progress the pointer past the query
        ++buffer->InputPnt;

        ByteToHexString(buffer->OutputPnt, DIN);
        buffer->OutputPnt += 4;
    }
}
