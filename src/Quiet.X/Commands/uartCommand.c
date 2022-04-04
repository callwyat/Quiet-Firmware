

#include "../CLI/cli.h"
#include "../uart.h"

#define UART_ERROR_NONE 0x00

#define UART_ERROR_INVALID_BAUD 0x10
#define UART_ERROR_INVALID_MODE 0x11
#define UART_ERROR_INVALID_WRITE 0x12

#define UART_ERROR_RECEIVE_OVERFLOW 0x20

#define UART_ERROR_WRITE_MODE_ERROR 0x30
#define UART_ERROR_READ_MODE_ERROR 0x31

uint8_t uartErrorCode = UART_ERROR_NONE;


void UARTReadCommand(CliBuffer_t *buffer, void* v)
{
    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        
        uint8_t receivedData = UART_get_rx_count();
        
        uint8_t maxSendSize = (uint8_t)(&buffer->OutputBuffer[sizeof(buffer->OutputBuffer)] - buffer->OutputPnt) - 4;
        
        if (receivedData > maxSendSize)
        {
            receivedData = maxSendSize;
        }
        
        GenerateIEEEHeader(buffer, receivedData);
        
        while (receivedData-- > 0)
        {
            *buffer->OutputPnt++ = UART_Read();
        }
    }
}

uint16_t uartReadSize = 0;

void UARTLargeWrite(CliBuffer_t *buffer, void *v)
{
    uint8_t bufferRemaining = (uint8_t)(&buffer->InputBuffer[buffer->InputLength] - buffer->InputPnt);

    if (uartReadSize <= bufferRemaining)
    {
        // Read out the data in this buffer
        do
        {
            UART_Write((uint8_t)*buffer->InputPnt++);
        } while (--uartReadSize > 0);
        
        ClearLargeDataHandle(buffer);
    }
    else
    {
        uartReadSize -= bufferRemaining;

        // Read out the data until all the data is read
        do 
        {
            UART_Write((uint8_t)*buffer->InputPnt++);
        }   while (--bufferRemaining > 0);

        if (buffer->DataHandle)
        {
            return;
        }
        else
        {
            SetLargeDataHandle(buffer, &UARTLargeWrite);
        }
    }
}

void UARTWriteCommand(CliBuffer_t *buffer, void* v)
{
    if (*buffer->InputPnt == ' ')
    {        
        ++buffer->InputPnt;
        
        if (*buffer->InputPnt == '#')
        {
            ++buffer->InputPnt;
            
            uartReadSize = ParseIEEEHeader(buffer);
            
            // Check for an invalid number
            if (UART_get_mode() == UMODE_USBUART)
            {
                if (uartReadSize != 0)
                {
                    UARTLargeWrite(buffer, v);
                }
                else
                {
                    uartErrorCode = UART_ERROR_INVALID_WRITE;
                }
            }
            else
            {
                uartErrorCode = UART_ERROR_WRITE_MODE_ERROR;
            }
        }
    }
}

void UARTBaudCommand(CliBuffer_t *buffer, void* v)
{
    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        
        uint24_t baudRate = UART_get_baud_rate();
        
        NumberToString(buffer, baudRate);
    }
    else if (*buffer->InputPnt == ' ')
    {
        ++buffer->InputPnt;
        
        uint24_t baudRate = ParseInt24(&buffer->InputPnt);
        
        // BaudRates below 60 cannot be generated with this the system clock
        if (baudRate > 60 && baudRate <= 1000000)
        {
            UART_set_baud_rate(baudRate);
        }
        else
        {
            uartErrorCode = UART_ERROR_INVALID_BAUD;
        }
    }
}

const char* USBUartWord = "USBU";
const char* SCPIUartWord = "SCPI";
const char* MODBusWord = "MODB";

void UARTModeCommand(CliBuffer_t *buffer, void* v)
{
    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;

        const char* word;
        
        switch (UART_get_mode())
        {
            case UMODE_USBUART:
                word = USBUartWord;
            break;
            case UMODE_SCPI:
                word = SCPIUartWord;
            break;
            case UMODE_MODBus:
                word = MODBusWord;
            break;
        }
        
        CopyWordToOutBuffer(buffer, word);
    }
    else if (*buffer->InputPnt == ' ')
    {
        ++buffer->InputPnt;
        
        if (SCPICompare(USBUartWord, buffer->InputPnt))
        {
            UART_set_mode(UMODE_USBUART);
        } 
        else if (SCPICompare(SCPIUartWord, buffer->InputPnt))
        {
            UART_set_mode(UMODE_SCPI);
        }
        else if (SCPICompare(MODBusWord, buffer->InputPnt))
        {
            UART_set_mode(UMODE_MODBus);
        }
        else
        {
            uartErrorCode = UART_ERROR_INVALID_MODE;
        }
    }
}

uint8_t UARTPeakErrorCode(void)
{
    return uartErrorCode;
}

uint8_t UARTPopErrorCode(void)
{
    uint8_t result = uartErrorCode;
    uartErrorCode = UART_ERROR_NONE;
    return result;
}

void UARTErrorCommand(CliBuffer_t *buffer, void* v)
{
    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        
        NumberToString(buffer, UARTPopErrorCode());
    }
}

CommandDefinition_t uartCommandChildren[] = {
  DEFINE_COMMAND("READ", UARTReadCommand),
  DEFINE_COMMAND("WRIT", UARTWriteCommand),
  DEFINE_COMMAND("BAUD", UARTBaudCommand),
  DEFINE_COMMAND("MODE", UARTModeCommand),
  DEFINE_COMMAND("ERR", UARTErrorCommand),
};
 
CommandDefinition_t UARTCommand = DEFINE_BRANCH("UART", uartCommandChildren);
