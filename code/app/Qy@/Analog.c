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

void AnalogInputSetup(void)
{
    ANCON0 = 0xC3;      //Enable the analog inputs
    ANCON1 = 0x3F;
    
    TMR1H = 0xFB;       //Setup Timer 1 to help with automatic analog conversion
    TMR1L = 0xAC;
    
    T1CON = 0x01;
    
    ADCTRIG = 0x02;     //Set Timer 1 as the conversion trigger
    
    ADCON1 = 0x2D;      //Left Justifiy, TAD = 0, FOSC/16
    
    ADCON0 = 0x09;      //Enable A/D Module
    
    ADCON0bits.GO = 1;  //Start the first convertion
    
    IPR1bits.ADIP = 1;  //Set A/D Conversion as high interrupt
    PIE1bits.ADIE = 1;  //Enable A/D Conversion interrupts
}

extern q_int Analog_Buffers[4]; 

void AnalogInt(void)
{   
    ADCON0bits.ADON = 0;        //Disable the analog module
    
    //Reload the timer. Should generate an interrupt in 250uS
    TMR1 = 0xFC89;
    
//    PORTBbits.RB0 = 1;          //Test Timing code
//    PORTBbits.RB0 = 0;
    
    //Test if using less then the PRO complier. The PRO complier will check what
    //Context saving is needed, but the Free will save all context, which takes
    //a lot of instructions
#if _HTC_EDITION_ < 2
    
    //This is not the most code friendly way to read in the analog input, but
    //It doesn't take any registers outside of WREG and Status to do, so it
    //can be recovered from using just RETFIE F
    
    switch(ADCON0bits.CHS)
    {
        case 2:
            Analog_Buffers[0].Higher = ADRESH;
            Analog_Buffers[0].Lower = ADRESL;
    
            ADCON0bits.CHS = 3;
            break;
        case 3:
            Analog_Buffers[1].Higher = ADRESH;
            Analog_Buffers[1].Lower = ADRESL;
            
            ADCON0bits.CHS = 4;
            break;
        case 4:
            Analog_Buffers[2].Higher = ADRESH;
            Analog_Buffers[2].Lower = ADRESL;
            
            ADCON0bits.CHS = 5;
            break;
        case 5:
            Analog_Buffers[3].Higher = ADRESH;
            Analog_Buffers[3].Lower = ADRESL;
            
            ADCON0bits.CHS = 2;
            break;
        default:
            ADCON0bits.CHS = 2;
            break;
    }
#else
    //If using the C compliers context saving, then a more scientific way can be
    //used to update the channel. However, if you look at the Program memory on
    //how to get here, this is really rough code.
    unsigned char CurrentChannel = ADCON0bits.CHS - 2;
    
    Analog_Buffers[CurrentChannel].Higher = ADRESH;
    Analog_Buffers[CurrentChannel].Lower = ADRESL;
    
    CurrentChannel++;
    if(CurrentChannel >= 4) CurrentChannel = 0;
    
    ADCON0bits.CHS = CurrentChannel + 2;
#endif
    
    ADCON0bits.ADON = 1;
}


