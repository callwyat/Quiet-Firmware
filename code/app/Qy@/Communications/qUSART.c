/*
 * File:   Setup.c
 * Author: Wyatt
 *
 * Created on May 21, 2018, 7:58 PM
 */

#define USB

#include <xc.h>
#include "qUSART.h"

#include "../Setup.h"
#include "../QyA_Code.h"


extern QyA_Settings Settings;

void USARTSetup(void)
{
    BAUDCON = Settings.BAUDCON;
    BAUDCON2 = Settings.BAUDCON;
    
    SetBaud(Settings.SPBRG.All);
    
    TXSTA1 = Settings.TXSTA;
    RCSTA1 = 0x80;
    
    TXSTA2 = 0x04;
    RCSTA2 = Settings.RCSTA;
    
    IPR3bits.RC2IP = 1;
    //PIE3bits.RC2IE = 1;
}

void SetBaud(unsigned int BaudRate) 
{
    q_int baudRate;
    baudRate.All = BaudRate;
    
    SPBRG1 = baudRate.Lower;
    SPBRGH1 = baudRate.Higher;     	// 0x0271 for 48MHz -> 19200 baud
    
    asm ("MOVF      SPBRG1,W");
    asm ("MULLW     3");
    asm ("MOVFF     PRODL,SPBRG2");
    asm ("MOVFF     PRODH,SPBRGH2");
    asm ("MOVF      SPBRGH1,W");
    asm ("MULLW     3");
    asm ("MOVF      PRODH,W");
    asm ("ADDWF     SPBRGH2,F");
}

    //Write USART Transmit Buffer
unsigned char UT_Buff[UT_BufferSize];
FIFO_Pointers UT_PNTS;

    //Read USART Receive Buffer
unsigned char UR_Buff[UR_BufferSize];
FIFO_Pointers UR_PNTS;

unsigned char USARTDataAvalible(void)
{   //Calculate how much data has been received since the READ command
    char tempByte = UR_PNTS.In - UR_PNTS.Out;
    
    if(UR_PNTS.In < UR_PNTS.Out) tempByte += UR_BufferSize;
    
    return tempByte;
}

//Returns the number of bytes read
unsigned char USARTRead(unsigned char* BuffPNT, unsigned char Length)
{
    //Read in the max amount of data that could be read
    unsigned char rLen = USARTDataAvalible();
    
    //Test if the users limit fits inside this
    if (Length < rLen) rLen = Length;
    
    unsigned char count;
    
    for(count = 0; count < rLen; count++)
    {   //Copy the data to the users buffer
        *BuffPNT = UR_Buff[UR_PNTS.Out++];
        //Limit the pointer
        UR_PNTS.Out &= UR_BufferSize - 1;
        //Move the users pointer to the next location
        BuffPNT++;
    }
    
    return rLen;
}

//Returns the space left in the TX Buffer
void USARTWrite(unsigned char data, unsigned checkFirst)
{   //Test if data can be sent right now, or if it needs to be buffered
//    if (PIR1bits.TX1IF && !PIE1bits.TX1IE)
//    {
//        TXREG1 = data;
//    }
//    else
//    {
    
        if(checkFirst)
        {   //Stall the processor if there is no space in the transmit buffer
            while(USARTWriteAvaliblity() <= 1);
        }
        
        /*Prevent any issues that could arise with being interrupt while adding to
        the send array*/
        DisableUTX;    
        
        //Place the data in the FIFO
        UT_Buff[UT_PNTS.In++] = data;
        //Handle rollover on the pointer
        UT_PNTS.In &= (UT_BufferSize - 1);      

        //Enable interrupts, which takes care of sending the data
        EnableUTX;
//    }
}

//Returns the number of data bytes not used in the USART Transmit buffer
unsigned char USARTWriteAvaliblity(void)
{
    char tempByte = UT_PNTS.In - UT_PNTS.Out;
    
    if(UT_PNTS.In < UT_PNTS.Out) tempByte += UT_BufferSize;
    
    return UT_BufferSize - tempByte;
}

void USART_INT(void)
{
    MoreTX:
    //Put data into the transmit register
    TXREG1 = UT_Buff[UT_PNTS.Out++];
    UT_PNTS.Out &= (UT_BufferSize - 1);

    //Test if there is more data to transmit
    if(UT_PNTS.Out == UT_PNTS.In) 
    {   //If not disable the transmit routine
        DisableUTX;
    }
    else
    {   //Test if more data can be setup to transmit right now
        if(PIR1bits.TX1IF) goto MoreTX;
    }
}

