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
    
    typedef struct CliBuffer
    {
        char *InputPnt;
        char *OutputPnt;
        uint8_t InputLength;
        void(*DataHandle)(struct CliBuffer *buffer);
        char InputBuffer[64];
        char OutputBuffer[64];
    } CliBuffer_t;
    
    
    typedef void(*CommandHandle)(CliBuffer_t *buffer);
    
    typedef void(*ChannelCommandHandle)(CliBuffer_t *buffer, uint8_t channel);
        
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
        CliBuffer_t *buffer, bool isRoot);
    
    void ByteToHexString(CliBuffer_t *buffer, uint8_t b);
    
    void IntToString(CliBuffer_t *buffer, uint16_t i);
    
    int16_t ParseInt(char** str);
    
    uint16_t ParseIEEEHeader(CliBuffer_t *buffer);
    
    void GenerateIEEEHeader(CliBuffer_t *buffer, uint16_t dataSize);
    
    void CopyWordToOutBuffer(CliBuffer_t *buffer, const char* word);
    
    void ProcessCLI(CliBuffer_t *buffer);
    
    void SetLargeDataHandle(CliBuffer_t *buffer, CommandHandle handle);
    
    void ClearLargeDataHandle(CliBuffer_t *buffer);

#ifdef	__cplusplus
}
#endif

#endif	/* CLI_H */

