
#include "../CLI/cli.h"
#include "../constants.h"

void DIGICommand(CliBuffer_t *buffer)
{
    if (*buffer->InputPnt == '?')
    {
        //Progress the pointer past the query
        ++buffer->InputPnt;
        NumberToString(buffer, DIN);
    }
}
