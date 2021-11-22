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
    
    typedef enum
    {
        HexFormat,
        DecimalFormat,
    } NumberFormat_e;
    
    typedef struct CliBuffer
    {
        char *InputPnt;
        char *OutputPnt;
        uint8_t InputLength;
        void(*DataHandle)(struct CliBuffer *buffer);
        char InputBuffer[64];
        char OutputBuffer[64];
    } CliBuffer_t;
    
    
    typedef void(*CommandHandle)(CliBuffer_t *buffer, void *channel);
        
    typedef struct CommandDefinition
    {
        const char Command[5];      // Needs to hold four letters and a null
        
        struct CommandDefinition* Children;
        uint8_t ChildrenCount;
        
        CommandHandle Handle;
        
    } CommandDefinition_t;
    
#define DEFINE_COMMAND(command, handle) { \
            .Command = command,           \
            .Handle = handle,             \
            .Children = 0x0000,           \
            .ChildrenCount = 0,           \
        }
    
#define DEFINE_BRANCH(command, children) { \
            .Command = command,           \
            .Handle = 0x0000,             \
            .Children = &children[0],     \
            .ChildrenCount =  sizeof(children) / sizeof(children[0]),    \
        }
    
    void CliInit(void);
    
    bool SCPICompare(const char *reference, char *input);
    
    void FFTilPunctuation(char **input);
    
    void ProcessCommand(CliBuffer_t *buffer);
    
    void SetNumberFormat(NumberFormat_e format);
    
    NumberFormat_e GetNumberFormat(void);
    
    void NumberToString(CliBuffer_t *buffer, uint24_t input);
    
    int16_t ParseInt(char** str);
    
    uint24_t ParseInt24(char** str);
    
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

