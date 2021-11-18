
#include "../CLI/cli.h"
#include "../constants.h"

void QueryDIGI(CliBuffer_t *buffer, void* v)
{
    if (*buffer->InputPnt == '?')
    {
        //Progress the pointer past the query
        ++buffer->InputPnt;
        NumberToString(buffer, DIN);
    }
}

const CommandDefinition_t DIGICommand = DEFINE_COMMAND("DIGI", QueryDIGI);
        
        