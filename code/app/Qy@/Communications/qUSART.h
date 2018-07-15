/* 
 * File:   Setup.h
 * Author: Wyatt
 *
 * Created on May 21, 2018, 7:59 PM
 */

#define UT_BufferSize 32
#define UR_BufferSize 32

#define EnableUTX PIE1bits.TX1IE = 1
#define DisableUTX PIE1bits.TX1IE = 0

void USARTSetup(void);

void SetBaud(unsigned int BaudRate);

unsigned char USARTDataAvalible(void);

unsigned char USARTRead(unsigned char* BuffPNT, unsigned char Length);

void USARTWrite(unsigned char, unsigned checkFirst);

unsigned char USARTWriteAvaliblity(void);

void USART_INT(void);