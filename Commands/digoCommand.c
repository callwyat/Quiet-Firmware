
#include "../CLI/cli.h"
#include "../constants.h"

void DigitalOutputs(CliBuffer *buffer)
{
    if (IS_QUERY(buffer))
    {
        //Progress the pointer past the query
        ++buffer->InputPnt;

        ByteToHexString(buffer->OutputPnt, DOUT);
        buffer->OutputPnt += 4;
    }
    else if (*buffer->InputPnt == ' ')
    {
        //Progress the pointer past the query
        ++buffer->InputPnt;
        
        int16_t value = ParseInt(&buffer->InputPnt);
        
        if (value >= 0)
        {
            DOUT = (uint8_t)value;
        }
        else
        {
            // TODO: Record an error?
        }
    }
}
