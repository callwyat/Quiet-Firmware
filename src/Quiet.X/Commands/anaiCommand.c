
#include "../analogInputs.h"

#include "../CLI/cli.h"
#include "../constants.h"
#include <stdint.h>

void QueryANAI(CliHandle_t *handle, void *v)
{
    if (handle->LastRead == '?')
    {
        uint8_t channel = *((uint8_t *)v);

        if (channel >= 1 && channel <= 4)
        {
            // Get the value from the analog buffer
            uint16_t value = GetADCValue(channel - 1);
            PrintNumber(handle, value);
        }
        else
        {
            QueueErrorCode(ANAI_ERROR_INVALID_CHANNEL);
        }
    }
}

CommandDefinition_t ANAI_CH_Commands[] = {
    DEFINE_COMMAND("CH", QueryANAI),
};

CommandDefinition_t ANAICommand = DEFINE_BRANCH("ANAI", ANAI_CH_Commands);
