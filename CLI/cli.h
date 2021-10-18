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
        char InputBuffer[64];
        char OutputBuffer[64];
    } CliBuffer;
    
    
    typedef void(*CommandHandle)(CliBuffer *buffer);
        
    typedef struct
    {
        const char Command[4];
        CommandHandle Handle;
        
    } CommandDefinition;
    
#define DEFINE_COMMAND(command, handle) { \
            .Command = command,           \
            .Handle = handle,             \
        }
    
    
    bool CompareStrings(const char *a, char *b, int length);
    
    int CountTillCommandEnd(char *input);
    
    void ProcessCommand(CommandDefinition commands[], uint8_t commandsLength, 
        CliBuffer *buffer);
    
    void ByteToHexString(char* str, uint8_t b);
    
    void ProcessCLI(CliBuffer *buffer);
    
#define IS_QUERY(buffer) (*buffer->InputPnt == '?')

#ifdef	__cplusplus
}
#endif

#endif	/* CLI_H */

