/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.78
        Device            :  PIC18F46J53
        Driver Version    :  2.00
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

#include "mcc_generated_files/mcc.h"
#include "CLI/cli.h"
#include "analogInputs.h"
#include "outputs.h"

static CliBuffer usbBuffer;

void USB_CDC_Tasks(void)
{
    if( USBGetDeviceState() < CONFIGURED_STATE )
    {
        return;
    }

    if( USBIsDeviceSuspended()== true )
    {
        return;
    }

    if( USBUSARTIsTxTrfReady() == true)
    {
        uint8_t i;
        uint8_t numBytesRead;

        numBytesRead = getsUSBUSART((uint8_t*)usbBuffer.InputBuffer, sizeof(usbBuffer.InputBuffer));

        if (numBytesRead > sizeof(usbBuffer.InputBuffer))
        {
            numBytesRead = 64;
        }
        
        for(i=0; i<numBytesRead; i++)
        {
            switch(usbBuffer.InputBuffer[i])
            {
                /* echo line feeds and returns without modification. */
                case 0x0A:
                case 0x0D:
                {
                    LATBbits.LATB0 = 1;
                    
                    ProcessCLI(&usbBuffer);
                    
                    putUSBUSART((uint8_t*)usbBuffer.OutputBuffer, 
                    (uint8_t)(usbBuffer.OutputPnt - usbBuffer.OutputBuffer)); 
                    
                    LATBbits.LATB0 = 0;
                }
                    break;

                /* all other characters get +1 (e.g. 'a' -> 'b') */
                default:
                    break;
            }
        }
    }

    CDCTxService();
}

typedef void(*TaskHandle)(void);

typedef struct {
    TaskHandle Handle;
    uint8_t Tick;
    uint8_t Top;
} Task_t;

#define DEFINE_TASK(handle, tick, top) {        \
    .Handle = handle,                           \
    .Tick = tick,                               \
    .Top = top                                  \
}

Task_t Tasks[] = {
    DEFINE_TASK(StartADCUpdate, 0, 1),
    DEFINE_TASK(ServoTick, 0, 1),
};

uint8_t tasksSize = sizeof(Tasks) / sizeof(Tasks[1]);

void ProcessTasks(void)
{
    Task_t *task;
    
    for (uint8_t i = 0; i < tasksSize; ++i)
    {
        task = &Tasks[i];
        
        if (++task->Tick >= task->Top)
        {
            task->Tick = 0;
            task->Handle();
        }
    }
}


/*
                         Main application
 */
void main(void)
{
    // TODO: Auto Crystal detection
    
    // Initialize the device
    SYSTEM_Initialize();
    
    
    
    //USB Setup-------------------------------
    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    // Use the following macros to:

    ADC_SetInterruptHandler(ADCTick);
    TMR2_SetInterruptHandler(ProcessTasks);
    
    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptHighEnable();
    INTERRUPT_GlobalInterruptLowEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable high priority global interrupts
    //INTERRUPT_GlobalInterruptHighDisable();

    // Disable low priority global interrupts.
    //INTERRUPT_GlobalInterruptLowDisable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    while (1)
    {
        USB_CDC_Tasks();
    }
}
/**
 End of File
*/