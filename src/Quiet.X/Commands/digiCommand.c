
#include "../CLI/cli.h"
#include "../constants.h"

#define DIGIN_CHNNEL ((uint8_t)(*((uint8_t*)channel)))

void DIGIDiscreetCommand(CliBuffer_t *buffer, void* v) {
    if (*buffer->InputPnt == '?') {
        //Progress the pointer past the query
        ++buffer->InputPnt;
        NumberToString(buffer, DIN);
    }
}

void DIGIChannelValueCommand(CliBuffer_t *buffer, void* channel) {

    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        uint8_t value = (DIN & (1 << ((DIGIN_CHNNEL) - 1))) > 0;
        
        NumberToString(buffer, value);
    }
}
    

CommandDefinition_t digiChanCommands[] = {
    DEFINE_COMMAND("VALU", DIGIChannelValueCommand),
};

CommandDefinition_t digiCommands[] = {
    DEFINE_COMMAND_W_BRANCH("CH", DIGIChannelValueCommand, digiChanCommands),
};

CommandDefinition_t DIGICommand = DEFINE_COMMAND_W_BRANCH("DIGI", DIGIDiscreetCommand, digiCommands);