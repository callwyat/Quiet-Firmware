
#include "../CLI/cli.h"

uint24_t value = 0;

void TestParserCommand(CliBuffer_t *buffer, void *channel)
{
    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        NumberToString(buffer, value);
    }
    else if (*buffer->InputPnt == ' ')
    {
        ++buffer->InputPnt;
        value = (uint24_t)ParseInt(&buffer->InputPnt);
    }
}


CommandDefinition_t testCommands[] = {
    DEFINE_COMMAND("PARS", TestParserCommand),
};

CommandDefinition_t TESTCommand = DEFINE_BRANCH("TEST", testCommands);
