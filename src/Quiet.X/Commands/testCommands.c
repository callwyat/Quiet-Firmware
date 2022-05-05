
#include "../CLI/cli.h"


void TestParserCommand(CliHandle_t *handle, void *v)
{
    static uint24_t testParserValue = 0;
    
    if (handle->LastRead == '?')
    {
        ReadChar(handle);
        WriteNumber(handle, testParserValue);
    }
    else if (handle->LastRead == ' ')
    {
        testParserValue = (uint24_t)ReadInt(handle);
    }
}

void TestLongWriteCommand(CliHandle_t *handle, void *v)
{
    static uint16_t longWriteCount = 4096;

    if (handle->LastRead == '?')
    {
        ReadChar(handle);
        for (uint16_t i = 0; i < longWriteCount; ++i)
        {
            handle->Write('L');
        }
    }
    else if (handle->LastRead == ' ')
    {
        longWriteCount = (uint24_t)ReadInt(handle);
    }
}

CommandDefinition_t testCommands[] = {
    DEFINE_COMMAND("PARS", TestParserCommand),
    DEFINE_COMMAND("LWRI", TestLongWriteCommand),
};

CommandDefinition_t TESTCommand = DEFINE_BRANCH("TEST", testCommands);
