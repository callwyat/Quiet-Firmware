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
            .Command = command,                                   \
            .Handle = handle,                                     \
        }
    
    void ProcessCLI(CliBuffer *buffer);
    


#ifdef	__cplusplus
}
#endif

#endif	/* CLI_H */

