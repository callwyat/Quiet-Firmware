/* 
 * File:   i.h
 * Author: callwyat
 *
 * Created on October 17, 2021, 2:41 PM
 */

#ifndef CLI_H
#define	CLI_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdint.h>
#include <stdbool.h>
    
    
    typedef struct
    {
        char *InputPnt;
        char *OutputPnt;
        char InputBuffer[64];
        char OutputBuffer[64];
    } CliBuffer;
    
    
    typedef void(*CommandHandle)(CliBuffer *buffer);
    
    typedef void(*ChannelCommandHandle)(CliBuffer *buffer, uint8_t channel);
        
    typedef struct
    {
        const char Command[4];
        CommandHandle Handle;
        ChannelCommandHandle ChannelHandle;
        
    } CommandDefinition;
    
#define DEFINE_COMMAND(command, handle) { \
            .Command = command,           \
            .Handle = handle,             \
        }
    
#define DEFINE_CHANNEL_COMMAND(command, handle) { \
            .Command = command,           \
            .ChannelHandle = handle,      \
        }
    
    bool SCPICompare(const char *reference, char *input);
    
    void FFTilPunctuation(char **input);
    
    void ProcessCommand(const CommandDefinition commands[], uint8_t commandsLength, 
        CliBuffer *buffer, bool isRoot);
    
    void ByteToHexString(CliBuffer *buffer, uint8_t b);
    
    void IntToString(CliBuffer *buffer, uint16_t i);
    
    int16_t ParseInt(char** str);
    
    bool IsNumber(char c);
    
    void CopyWordToOutBuffer(CliBuffer *buffer, const char* word);
    
    void ProcessCLI(CliBuffer *buffer);

#ifdef	__cplusplus
}
#endif

#endif	/* CLI_H */

