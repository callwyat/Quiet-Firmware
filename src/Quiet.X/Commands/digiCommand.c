
#include "../CLI/cli.h"
#include "../constants.h"

void DIGIDiscreetCommand(CliBuffer_t *buffer, void* v) {
    if (*buffer->InputPnt == '?') {
        //Progress the pointer past the query
        ++buffer->InputPnt;
        NumberToString(buffer, DIN);
    }
}

void DIGIChannelValueCommand(CliBuffer_t *buffer, void* v) {

    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;

        uint8_t channel = *((uint8_t*)v);

        if (channel >= 1 && channel <= 8)
        {
            uint8_t value = (DIN & (1 << ((channel) - 1))) > 0;
            NumberToString(buffer, value);
        }
        else
        {
            QueueErrorCode(DIGI_ERROR_INVALID_CHANNEL);
        }
    }
}
    

CommandDefinition_t digiChanCommands[] = {
    DEFINE_COMMAND("VALU", DIGIChannelValueCommand),
};

CommandDefinition_t digiCommands[] = {
    DEFINE_COMMAND_W_BRANCH("CH", DIGIChannelValueCommand, digiChanCommands),
};

CommandDefinition_t DIGICommand = DEFINE_COMMAND_W_BRANCH("DIGI", DIGIDiscreetCommand, digiCommands);