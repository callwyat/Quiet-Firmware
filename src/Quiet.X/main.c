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

// Disable function never called warning
#pragma warning disable 520
// Disable function never called warning
#pragma warning disable 2053

#include "mcc_generated_files/mcc.h"
#include "CLI/cli.h"
#include "analogInputs.h"
#include "uart.h"
#include "outputs.h"
#include "settings.h"
#include "constants.h"

extern CommandDefinition_t PWMCommand;
extern CommandDefinition_t SERVoCommand;
extern CommandDefinition_t DIGOCommand;
extern CommandDefinition_t ANAOCommand;
extern CommandDefinition_t ANAICommand;
extern CommandDefinition_t DIGICommand;
extern CommandDefinition_t SYSTemCommand;
extern CommandDefinition_t UARTCommand;
extern CommandDefinition_t SPICommand;
extern CommandDefinition_t IICCommand;
extern CommandDefinition_t DIAGnosticsCommand;
extern CommandDefinition_t TESTCommand;

// Put the commands that have the most branches towards the top
CommandDefinition_t allCommands[16];
CommandDefinition_t limitedCommands[16];

uint8_t USB_get_read_count(void);
char USB_Read(void);
void USB_Write(char);

static CliHandle_t usbHandle = DEFINE_CLI_HANDLE(USB_get_read_count, USB_Read, USB_Write);
static CliHandle_t uartHandle = DEFINE_CLI_HANDLE(UART_get_rx_count, UART_Read, UART_Write);

#define USB_BUFFER_SIZE 64
uint8_t usbInBuffer[USB_BUFFER_SIZE];
uint8_t *usbInPnt = usbInBuffer;
uint8_t usbInCount;
uint8_t usbOutBuffer[USB_BUFFER_SIZE];
uint8_t *usbOutPnt = usbOutBuffer;
uint8_t usbOutCount;

void CliInit(void)
{
    // Setup the command groups
    CommandDefinition_t *pnt = allCommands;
    *pnt++ = PWMCommand;
    *pnt++ = SERVoCommand;
    *pnt++ = DIGOCommand;
    *pnt++ = ANAOCommand;
    *pnt++ = ANAICommand;
    *pnt++ = DIGICommand;
    *pnt++ = UARTCommand;
    *pnt++ = SPICommand;
    *pnt++ = IICCommand;
    *pnt++ = SYSTemCommand;
    *pnt++ = DIAGnosticsCommand;
    *pnt++ = TESTCommand;

    pnt = limitedCommands;
    *pnt++ = PWMCommand;
    *pnt++ = SERVoCommand;
    *pnt++ = DIGOCommand;
    *pnt++ = ANAOCommand;
    *pnt++ = ANAICommand;
    *pnt++ = DIGICommand;
    *pnt++ = SYSTemCommand;
    *pnt++ = DIAGnosticsCommand;

    usbHandle.ReceivePnt = usbHandle.LastWord;
    uartHandle.ReceivePnt = uartHandle.LastWord;
}

uint8_t USB_get_read_count(void)
{
    return usbInCount;
}

char USB_Read(void)
{
    while (usbInCount == 0)
    {
        usbInCount = getsUSBUSART((uint8_t *)usbInBuffer, USB_BUFFER_SIZE);
        usbInPnt = usbInBuffer;
    }

    --usbInCount;
    return *usbInPnt++;
}

void USB_Flush_Write()
{
    if (usbOutCount > 0)
    {
        while (!USBUSARTIsTxTrfReady())
        {
            CDCTxService();
        }
        
        putUSBUSART((uint8_t*)usbOutBuffer, usbOutCount);
        CDCTxService();
        usbOutPnt = usbOutBuffer;
        usbOutCount = 0;   
    }
}

void USB_Write(char c)
{
    *usbOutPnt++ = c;

    if (++usbOutCount > USB_BUFFER_SIZE)
    {
        USB_Flush_Write();
    }
}

void USB_CDC_Tasks(void)
{
    if (USBGetDeviceState() < CONFIGURED_STATE)
    {
        USBLED = 0;
        return;
    }
    else
    {
        USBLED = 1;
    }

    if (USBIsDeviceSuspended() == true)
    {
        return;
    }

    if (USBUSARTIsTxTrfReady() == true)
    {
        usbInCount = getsUSBUSART((uint8_t *)usbInBuffer, USB_BUFFER_SIZE);

        if (usbInCount != 0)
        {
            usbInPnt = usbInBuffer;

            USBLED = 0;
            ProcessCLI(&usbHandle, allCommands);
            USBLED = 1;

            USB_Flush_Write();
        }
    }

    CDCTxService();
}

typedef void (*TaskHandle)(void);

typedef struct
{
    TaskHandle Handle;
    uint8_t Tick;
    uint8_t Top;
} Task_t;

#define DEFINE_TASK(handle, tick, top) \
    {                                  \
        .Handle = handle,              \
        .Tick = tick,                  \
        .Top = top                     \
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
#if 1 || __DEBUG

    //    FSR0 = 0x0E00;
    //
    //    do
    //    {
    //        POSTDEC0 = 0x00;
    //    } while (FSR0 != 0x00);

#endif
    // TODO: Auto Crystal detection

    // Initialize the device
    SYSTEM_Initialize();

    // Load up settings
    RestoreSettings(false);

    // USB Setup-------------------------------
    //  If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    //  If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    //  Use the following macros to:

    ADC_SetInterruptHandler(ADCTick);
    TMR2_SetInterruptHandler(ProcessTasks);

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptHighEnable();
    INTERRUPT_GlobalInterruptLowEnable();

    // Disable the Global Interrupts
    // INTERRUPT_GlobalInterruptDisable();

    // Disable high priority global interrupts
    // INTERRUPT_GlobalInterruptHighDisable();

    // Disable low priority global interrupts.
    // INTERRUPT_GlobalInterruptLowDisable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts
    // INTERRUPT_PeripheralInterruptDisable();

    CliInit();

    while (1)
    {
        USB_CDC_Tasks();

        if (UART_get_mode() == UMODE_SCPI)
        {
            ProcessCLI(&uartHandle, limitedCommands);
        }
    }
}
/**
 End of File
*/