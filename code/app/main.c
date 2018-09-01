/********************************************************************
 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the "Company") for its PIC(R) Microcontroller is intended and
 supplied to you, the Company's customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *******************************************************************/



/** INCLUDES *******************************************************/
#include "main.h"
#include "system.h"
#include "system_config.h"

#include "app_device_cdc_basic.h"
#include "app_led_usb_status.h"

#include "../framework/usb/usb.h"
#include "../framework/usb/usb_device.h"
#include "../framework/usb/usb_device_cdc.h"

#include    "Qy@/Setup.h"
#//include  "SD Card.h"
#include    "Qy@/QyA_Code.h"

#include "Qy@/Communications/qUSART.h"
#include "Qy@/Communications/qSPI.h"
#include "Qy@/Communications/qI2C.h"

uint8_t InBuff[CDC_DATA_IN_EP_SIZE];

uint8_t OutPNT;

//Stores the settings
extern QyA_Settings Settings;

typedef enum 
{
    NONE,
    USB,
    USART,
    SPI,
    I2C,
} ComMode;


ComMode TXMode;
/********************************************************************
 * Function:        void main(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Main program entry point.
 *
 * Note:            None
 *******************************************************************/
MAIN_RETURN main(void)
{
    Setup(1);   //Run the setup and read the Hard Settings
    
    //If user wants USB, then boot it up
    if (Settings.BOOT.USB_Enable)
    {
        //USB Setup-------------------------------
        SYSTEM_Initialize(SYSTEM_STATE_USB_START);

        //Test if USB is present or not
        USBDeviceInit();
        USBDeviceAttach();
    }
        
    LATEbits.LATE1 = 0;
    
    do
    {   
        //Test if a USB device is attached
        TRISEbits.TRISE1 = 0;       //Pull extra energy down before reading.
        TRISEbits.TRISE1 = 1;
        if (PORTEbits.RE1)
        {
            TXMode = USB;
            USBMode();
        }
        //Test for USART activity
        if (USARTDataAvalible() > 0)
        {
            TXMode = USART;
            USARTMode();
        }
        //Test for SPI activity
        if (PIR3bits.SSP2IF)
        {
            TXMode = SPI;
            SPIMode();
        }
        //Test for I2C activity
        if (PIR1bits.SSP1IF)
        {
            TXMode = I2C;
            I2CMode();
        }
    } while(1); 
}
    
void USBMode(void)
{
    /* This code will never be going back (except for a reset) so clean up the 
     * stack by popping off the value that got us here*/
    asm("POP");
         
    //Set the USB indicator LED off
    TRISEbits.TRISE1 = 0;
    
    while(1)
    {
        
        #if defined(USB_POLLING)
            // Interrupt or polling method.  If using polling, must call
            // this function periodically.  This function will take care
            // of processing and responding to SETUP transactions
            // (such as during the enumeration process when you first
            // plug in).  USB hosts require that USB devices should accept
            // and process SETUP packets in a timely fashion.  Therefore,
            // when using polling, this function should be called
            // regularly (such as once every 1.8ms or faster** [see
            // inline code comments in usb_device.c for explanation when
            // "or faster" applies])  In most cases, the USBDeviceTasks()
            // function does not take very long to execute (ex: <100
            // instruction cycles) before it returns.
            USBDeviceTasks();
        #endif
        /* If the USB device isn't configured yet, we can't really do anything
         * else since we don't have a host to talk to.  So jump back to the
         * top of the while loop. */
        if( USBGetDeviceState() < CONFIGURED_STATE )
        {
            /* Jump back to the top of the while loop. */
            continue;
        }

        /* If we are currently suspended, then we need to see if we need to
         * issue a remote wakeup.  In either case, we shouldn't process any
         * keyboard commands since we aren't currently communicating to the host
         * thus just continue back to the start of the while loop. */
        if( USBIsDeviceSuspended()== true )
        {
            /* Jump back to the top of the while loop. */
            continue;
        }
            
            
        //Copy the Data from the USB Buffer 
        unsigned char readCount = getsUSBUSART(InBuff, 64);
        
        //If more then 0 bytes were read, then it is time to execute some commands!
        if(readCount > 0)
        {
            unsigned char readPNT = 0;
            
            while(readPNT < readCount)
                QyA_Command(InBuff[readPNT++]);
            
            //After all the commands have been processed, send the result
            StartUSBTransmit();
        }
        
        //Handles the Transmission Service 
        CDCTxService();
            
        //Application specific tasks
        //APP_DeviceCDCEmulatorTasks();
    }
}

void USARTMode(void)
{
    /* This code will never be going back (except for a reset) so clean up the 
     * stack by popping off the value that got us here*/
    asm("POP");
    
    while(1)
    {      
        //Copy the Data from the USB Buffer 
        unsigned char readCount = USARTRead(&InBuff[0], CDC_DATA_IN_EP_SIZE);
        
        //If more then 0 bytes were read, then it is time to execute some commands!
        if(readCount > 0)
        {
            unsigned char readPNT = 0;
            
            while(readPNT < readCount)
                QyA_Command(InBuff[readPNT++]);
            
        }
    }
}

void SPIMode(void)
{
    /* This code will never be going back (except for a reset) so clean up the 
     * stack by popping off the value that got us here*/
    asm("POP");
    
    while(1)
    {
        //Copy the Data from the USB Buffer 
        unsigned int readCount = SPIRead(&InBuff[0], (int)CDC_DATA_IN_EP_SIZE);
        
        //If more then 0 bytes were read, then it is time to execute some commands!
        if(readCount > 0)
        {
            unsigned char readPNT = 0;
            
            while(readPNT < readCount)
                QyA_Command(InBuff[readPNT++]);
            
        }
    }
}

void I2CMode(void)
{
    /* This code will never be going back (except for a reset) so clean up the 
     * stack by popping off the value that got us here*/
    asm("POP");
    
    while(1)
    {
        
    }
}

uint8_t OutBuff[CDC_DATA_OUT_EP_SIZE];
unsigned char OutPNT = 0;

void Send(unsigned char input)
{
    switch(TXMode)
    {
        case USB:
            OutBuff[OutPNT++] = input;
            if (OutPNT >= CDC_DATA_OUT_EP_SIZE)
                StartUSBTransmit();
            break;
            
        case USART:
            //Write the data, check if the buffer is full
            USARTWrite(input, true);
            break;
            
        case SPI:
            SPIWrite(input);
            break;
            
        case I2C:
            
            break;
            
        default:
            
            break;     
    }
    
    return;
}

unsigned StartUSBTransmit(void)
{
    while (!USBUSARTIsTxTrfReady())
    {
        //Handles the Transmission Service 
        CDCTxService();
    }
    
    if(OutPNT > 0)
    {
        putUSBUSART(OutBuff, OutPNT);
        OutPNT = 0;

        //Handles the Transmission Service 
        CDCTxService();
        return 1;
    }

    return 0;
}


bool USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, uint16_t size)
{
    switch( (int) event )
    {
        case EVENT_TRANSFER:
            break;

        case EVENT_SOF:
            /* We are using the SOF as a timer to time the LED indicator.  Call
             * the LED update function here. */
            APP_LEDUpdateUSBStatus();
            break;

        case EVENT_SUSPEND:
            /* Update the LED status for the suspend event. */
            APP_LEDUpdateUSBStatus();
            break;

        case EVENT_RESUME:
            /* Update the LED status for the resume event. */
            APP_LEDUpdateUSBStatus();
            break;

        case EVENT_CONFIGURED:
            /* When the device is configured, we can (re)initialize the 
             * demo code. */
            APP_DeviceCDCEmulatorInitialize();
            break;

        case EVENT_SET_DESCRIPTOR:
            break;

        case EVENT_EP0_REQUEST:
            /* We have received a non-standard USB request.  The HID driver
             * needs to check to see if the request was for it. */
            USBCheckCDCRequest();
            break;

        case EVENT_BUS_ERROR:
            break;

        case EVENT_TRANSFER_TERMINATED:
            break;

        default:
            break;
    }
    return true;
}
/*******************************************************************************
 End of File
*/
