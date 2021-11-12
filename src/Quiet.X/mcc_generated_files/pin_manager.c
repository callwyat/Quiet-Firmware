/**
  Generated Pin Manager File

  Company:
    Microchip Technology Inc.

  File Name:
    pin_manager.c

  Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for pin APIs for all pins selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.78
        Device            :  PIC18F46J53
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.05 and above
        MPLAB             :  MPLAB X 5.20

    Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "pin_manager.h"


void PIN_MANAGER_Initialize(void)
{
    /**
    LATx registers
    */
    LATE = 0x00;
    LATD = 0x00;
    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x00;

    /**
    TRISx registers
    */
    TRISE = 0x05;
    TRISA = 0xEC;
    TRISB = 0x00;
    TRISC = 0x84;
    TRISD = 0xFF;

    /**
    ANSELx registers
    */
    ANCON0 = 0xC3;
    ANCON1 = 0x1F;

    /**
    WPUx registers
    */
    INTCON2bits.nRBPU = 1;
    TRISEbits.RDPU = 0;
    TRISEbits.REPU = 0;

    /**
    ODx registers
    */
    ODCON1 = 0x00;
    ODCON2 = 0x00;
    ODCON3 = 0x00;

    // Peripheral Pin Select----------------------------
    EECON2 = 0x55;      //Charge pump!
    EECON2 = 0xAA;
    
    PPSCONbits.IOLOCK = 0;  //Enable writing to the re-mappable functions
    
    //PWM Pin Setup
    RPOR0 = 14;       //PWM1A
    RPOR1 = 18;       //PWM2A
    RPOR6 = 22;       //PMW3A
    RPOR5 = 23;       //PMW3B
    RPOR4 = 24;       //PMW3C
    RPOR3 = 25;       //PMW3D
    
    //SSP2 Pin Setup
    RPINR21 = 13;       //MSSP2 SPI DATA IN
    RPOR11 = 0x0a;      //MSSP2 SPI DATA OUT
    RPOR12 = 0x0B;      //MSSP2 SPI CLK
    
    //USART Setup
    RPINR16 = 18;        //USART2 RC
            
    EECON2 = 0x55;
    EECON2 = 0xAA;

}

void PIN_MANAGER_IOC(void)
{	
	// Clear global Interrupt-On-Change flag
    INTCONbits.RBIF = 0;
}

/**
 End of File
*/