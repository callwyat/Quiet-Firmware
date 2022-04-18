/* 
 * File:   outputCommand.h
 * Author: callwyat
 *
 * Created on April 18, 2022, 9:50 PM
 */

#ifndef OUTPUTCOMMAND_H
#define	OUTPUTCOMMAND_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t MaxChannels;
    int8_t ChannelsOffset;
    uint8_t ErrorGroup;
} OutputCommand_t;

#define DEFINE_OUTPUT_COMMAND_T(channels, offset, error) {  \
    .MaxChannels = channels,                                \
    .ChannelsOffset = offset,                               \
    .ErrorGroup = error,                                    \
}
    
void OutputChannelModeCommand(CliBuffer_t *buffer, const OutputCommand_t settings, void *v);

void OutputChannelValueCommand(CliBuffer_t *buffer, const OutputCommand_t settings, void *v);

#ifdef	__cplusplus
}
#endif

#endif	/* OUTPUTCOMMAND_H */

