
#include "../CLI/cli.h"

uint24_t value = 0;

void TestParserCommand(CliHandle_t *handle, void *channel)
{
    if (handle->LastRead == '?')
    {
        PrintNumber(handle, value);
    }
    else if (handle->LastRead == ' ')
    {
        value = (uint24_t)ReadInt(handle);
    }
}

CommandDefinition_t testCommands[] = {
    DEFINE_COMMAND("PARS", TestParserCommand),
};

CommandDefinition_t TESTCommand = DEFINE_BRANCH("TEST", testCommands);
