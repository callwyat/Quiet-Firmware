
#include "../analogInputs.h"

#include "../CLI/cli.h"
#include <stdint.h>

void QueryANAI(CliBuffer_t *buffer, void *v)
{
    uint8_t channel = *((uint8_t*)v);
    
    if (channel >= 1 && channel <= 4)
    {
        // Progress the input past this command
        ++buffer->InputPnt;

        // Get the value from the analog buffer
        uint16_t value = GetADCValue(channel - 1);
        NumberToString(buffer, value);
    }
}

const CommandDefinition_t ANAI_CH_Commands[] = {
    DEFINE_COMMAND("CH", QueryANAI),
};

const CommandDefinition_t ANAICommand = DEFINE_BRANCH("ANAI", ANAI_CH_Commands);

