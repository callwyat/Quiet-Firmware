/*
 * File:   Setup.c
 * Author: Wyatt
 *
 * Created on May 21, 2018, 7:58 PM
 */

#include <xc.h>
#include "Setup.h"
#include "QyA_Code.h"
#include "Interrupts.h"

#include "Servo.h"
#include "Analog.h"

#include "Communications/qUSART.h"
#include "Communications/qSPI.h"
#include "Communications/qI2C.h"

q_int ShadowFSR;        //This is a global varible to prevent using the Heap

    //Read Status Byte Memory
extern q_Status Q_Status;

    //Read USART Receive Buffer
extern unsigned char UR_Buff[UR_BufferSize];
extern FIFO_Pointers UR_PNTS;

void HighInterrupt(void)
{
    if(PIR1bits.ADIF)       //Test if new analog data is available
    {
        PIR1bits.ADIF = 0;  //Clear the flag
        AnalogInt();
    }
    
    if(PIR3bits.RC2IF)      //Test if a byte has been received
    {
        PIR3bits.RC2IF = 0; //Clear the flag
        
        ShadowFSR.Higher = FSR1H;   //Save some context
        ShadowFSR.Lower = FSR1L;
        
        FSR1 = (unsigned short)&UR_Buff[0]; //Setup a pointer without using the heap
        FSR1L += UR_PNTS.In;
        if(STATUSbits.C) FSR1H++;
        
        INDF1 = RCREG2;     //Save the data
        
        UR_PNTS.In++;       //Increment the in pointer 
        UR_PNTS.In &= (UR_BufferSize - 1);      //Handle a rollover
        
        FSR1H = ShadowFSR.Higher;   //Restore context
        FSR1L = ShadowFSR.Lower;
        
        Q_Status.URC_DataAvailable = 1;      //Mark that data is available
        if(UR_PNTS.In == UR_PNTS.Out) Q_Status.URC_OverFill = 1;
        
    }
    
    if(PIR1bits.TMR2IF)     //Test if the servos need to be updated
    {
        PIR1bits.TMR2IF = 0;    //Clear the flag
        UpdateServo();
    }
    
#if _HTC_EDITION_ < 2
    asm ("POP");
    asm ("RETFIE F");
#endif
}

void LowInterrupt(void)
{   
    //Test for a transmit interrupt
    if(PIE1bits.TX1IE && PIR1bits.TX1IF)
    {
        USART_INT();
    }
    
    //Test for a SPI Interrupt - This interrupt is used to start a DMA transfer
    if(PIE3bits.SSP2IE && PIR3bits.SSP2IF)
    {   
        PIR3bits.SSP2IF = 0;
        
        SPIINT();
    }
}