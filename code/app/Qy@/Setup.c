/*
 * File:   Setup.c
 * Author: Wyatt
 *
 * Created on May 21, 2018, 7:58 PM
 */

#define USB

#include <xc.h>

#include "QyA_Code.h"
#include "Setup.h"
#include "Subroutines.h"

#include "Analog.h"
#include "Servo.h"
#include "Subroutines.h"

#ifdef USB
#include "../../bsp/pic18f46j53_pim/usart.h"
#endif

#include "Communications/qUSART.h"
#include "Communications/qSPI.h"
#include "Communications/qI2C.h"

extern QyA_Settings Settings;

extern unsigned int HardSettingsLocation;

extern q_Status Q_Status;

//ReadHardSettings allows the user to run the Setup without reading the flash
//Memory
void Setup(unsigned ReadHardSettings) 
{
    
    //Memory Setup--------------------------------------
    Q_Status.All = 0;       //Reset all the statisics
    
    if(ReadHardSettings)    //U
    {
        //Test if there are any settings
        unsigned char testByte = 0x00;
        
        //Read the byte that indicates if this is the first run after a flash
        Flash_Read(CoreConfigLocation - 1, &testByte, 1);
        if (testByte == 0xFF)
            FirstRun();
                
        Flash_Read(BaseSettingsLocation, &testByte, 1);   //Read the first byte of the base settings

        if(testByte == 0xFF)
        {//No Settings have been created, generate the default settings
            //Set a mark that the default settings are being used
            HardSettingsLocation = BaseSettingsLocation - 1;

            //Default settings                         +++++++++++Default User ID++++++++++
            Settings.UserID[0] = 'D';
            Settings.UserID[1] = 'e';
            Settings.UserID[2] = 'f';
            Settings.UserID[3] = 'a';
            Settings.UserID[4] = 'u';
            Settings.UserID[5] = 'l';
            Settings.UserID[6] = 't';
            Settings.UserID[7] = ' ';

            for(unsigned char count = 8; count < 54; count++)
            {
                Settings.Byte[count] = 0;
            }

            //Set the PWMs to a 50% duty cycle
            Settings.Servo[0] = 0x80;
            Settings.Servo[1] = 0x80;

            //Default USART Settings
            Settings.SPBRG.All = 0x00CF;    //Default Baud is 19200
            Settings.TXSTA = 0x24;       	// TX enable, BRGH = 1
            Settings.BAUDCON = 0x08;     	// BRG16 = 1
            Settings.TXSTA = 0x04;          //TX Disabled, BRGH = 1
            Settings.RCSTA = 0x90;          //Enable Module, Continuous Receive
            
            //Enable the default slave ports
            Settings.BOOT.USB_Enable = 1;
            Settings.BOOT.USART_Slave = 1;
            Settings.BOOT.SPI_Slave = 1;
            
            //Set the serial Number                         +++++++++++Serial Number Location++++++++++
            Settings.SN[0] = '0';
            Settings.SN[1] = '0';
            Settings.SN[2] = '0';
            Settings.SN[3] = '0';
            Settings.SN[4] = '0';
            Settings.SN[5] = '0';
            Settings.SN[6] = '0';
            Settings.SN[7] = '0';
        }
        else
        {   //There are settings, use the newest copy
            testByte = 0xFF;
            HardSettingsLocation = BaseSettingsLocation + 1024;

            do{
                HardSettingsLocation -= 64;
                Flash_Read(HardSettingsLocation, &testByte, 1); 
            } while(testByte == 0xFF);

            Flash_Read(HardSettingsLocation, &Settings.Byte[0], 64);
        }
    }
    
    // Peripheral Pin Select----------------------------
    EECON2 = 0x55;      //Charge pump!
    EECON2 = 0xAA;
    
    PPSCONbits.IOLOCK = 0;  //Enable writing to the re-mappable functions
    
    //PWM Pin Setup
    RPOR0 = 14;       //PWM1A
    RPOR1 = 18;       //PWM2A
    
    //SSP2 Pin Setup
    RPINR21 = 13;       //MSSP2 SPI DATA IN
    RPOR11 = 0x0a;      //MSSP2 SPI DATA OUT
    RPOR12 = 0x0B;      //MSSP2 SPI CLK
    
    //USART Setup
    RPINR16 = 18;        //USART2 RC
            
    EECON2 = 0x55;
    EECON2 = 0xAA;
    
    // Port Setup---------------------------------------
         
    INTCON2bits.NOT_RBPU = 1;   //Enable Week Pull ups
            
    SetupPort((unsigned char*)&PORTA, 0x00, 0x2C);
    SetupPort((unsigned char*)&PORTB, 0x00, 0x00);
    SetupPort((unsigned char*)&PORTC, 0x00, 0x84);
    SetupPort((unsigned char*)&PORTD, 0x00, 0xFF);
    
    //Port E effects the operation of the USB... so skip it in a Settings reset
    if(ReadHardSettings) SetupPort((unsigned char*)&PORTE, 0x00, 0x3F);

    
    // Interrupt Priority Setup-------------------------
    IPR1 = 0x00;        //Set all interrupt Priorities to Zero!
    IPR2 = 0x00;
    IPR3 = 0x00;
    IPR4 = 0x00;
    IPR5 = 0x00;
    
    
    // Analog Setup-------------------------------------
    AnalogInputSetup();
    
    // PWM / Servo Setup--------------------------------
    ServosSetup();
    
    //USART Setup---------------------------------------
    USARTSetup();    
    
    //Setup BaudCon and all that stuff
    SetBaud(Settings.SPBRG.All);    //Read in the last used baud rate
    
    //SPI Setup-----------------------------------------
    SPISetup();
    
    // Interrupt Setup----------------------------------
    
    return;
}

void InterruptSetup()
{
    PIE3bits.RC2IE = 1;
}

void SetupPort(unsigned char *PortPnt, unsigned char Value, unsigned char TriState)
{
    *PortPnt = Value;       //Set PORT
    PortPnt = PortPnt + (&TRISA - &PORTA);
    *PortPnt = TriState;    //Set TRIS
}


void FirstRun(void)
{
    unsigned char NewConfig[9];
    
    NewConfig[0] = 0;   //Mark this as not the first time ever.
            
    //Read in the current configuration
    Flash_Read(CoreConfigLocation, &NewConfig[1], 8);
    
    //Test if auto clock detections is enabled (CFGPLLEN = 1 and PLLDIV = 7)
    if ((NewConfig[1] & 0x1E) == 0x1E)
    {
        //Clear any unwanted flags
        PIR3 = 0;
        PIR1 = 0;

        //Setup a timer that will use the system clock
        PR2 = 250;
        T2CON = 0x32;

        //Setup a timer to track one second of time
        ALRMCFGbits.AMASK = 0x1;
        ALRMCFGbits.ALRMEN = 1;

        EECON2 = 0x55;              //Pump Charge!!
        EECON2 = 0xAA;
        RTCCFGbits.RTCWREN = 1;
        RTCCFGbits.RTCEN = 1;

        T2CONbits.TMR2ON = 1;

        unsigned char ticks = 0;
        TRISB = 0x00;

        //Wait for the Real Time Clock to raise it's flag
        while (!PIR3bits.RTCCIF)
        {
            if (PIR1bits.TMR2IF)
            {
                ticks++;

                DigitalOutput = ticks;
                PIR1bits.TMR2IF = 0;
            }    
        }

        NewConfig[1] &= 0xE1;

        /* For this to work CONFIG1L needs to be b???1 111? so that a flash 
         * without erase can clear the needed bits. The equation for the number 
         * of ticks is:
         * 
         * FOSC / (   3   * 4 *   16   *  250 *     6  )
         * FOSC / (CPUDIV * 4 * T2CKPS *  PR2 * T2OUTPS)                      */

        if (ticks > 60)    //(Greater then 16MHz)
        {   
            NewConfig[1] |= 0x16;
        }
        else
        {   
            NewConfig[1] |= 0x18;
        }

        //Flash_Erase(CoreConfigLocation);
        Flash_Write(CoreConfigLocation - 1, &NewConfig[0], 2);

        //Test if the PIC is not in DEBUG Mode
        if (NewConfig[1] & 0x80)
            while(1);
        else
            asm("RESET");
    }
    else
    {
        Flash_Write(CoreConfigLocation - 1, &NewConfig[0], 1);
    }
}






