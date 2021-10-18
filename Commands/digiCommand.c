
#include "../CLI/cli.h"
#include "../constants.h"

void DigitalInputs(CliBuffer *buffer)
{
    if (IS_QUERY(buffer))
    {
      ByteToHexString(buffer->OutputBuffer, DIN);
    }
}
