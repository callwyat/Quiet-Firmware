

#include "../CLI/cli.h"
#include "../uart.h"
#include "../constants.h"

void UARTReadCommand(CliHandle_t *handle, void *v)
{
    if (handle->LastRead == '?')
    {
        ReadChar(handle);
        uint8_t receivedCount = UART_get_rx_count();
        WriteIEEEHeader(handle, receivedCount);

        while (receivedCount-- > 0)
        {
            handle->Write(UART_Read());
        }
    }
}

void UARTWriteCommand(CliHandle_t *handle, void *v)
{
    if (handle->LastRead == ' ')
    {
        ReadChar(handle);
        if (handle->LastRead == '#')
        {
            uint16_t uartWriteSize = ReadIEEEHeader(handle);

            // Check for an invalid number
            if (UART_get_mode() == UMODE_USBUART)
            {
                if (uartWriteSize == 0)
                {
                    QueueErrorCode(UART_ERROR_INVALID_WRITE);
                }
                
                while (uartWriteSize > 0)
                {
                    UART_Write(handle->Read());
                    --uartWriteSize;
                }

                ReadWord(handle);
            }
            else
            {
                QueueErrorCode(UART_ERROR_WRITE_MODE_ERROR);
            }
        }
    }
}

void UARTBaudCommand(CliHandle_t *handle, void *v)
{
    if (handle->LastRead == '?')
    {
        ReadChar(handle);
        uint24_t baudRate = UART_get_baud_rate();

        WriteNumber(handle, baudRate);
    }
    else if (handle->LastRead == ' ')
    {
        uint24_t baudRate = ReadInt24(handle);

        // BaudRates below 60 cannot be generated with this the system clock
        if (baudRate > 60 && baudRate <= 1000000)
        {
            UART_set_baud_rate(baudRate);
        }
        else
        {
            QueueErrorCode(UART_ERROR_INVALID_BAUD);
        }
    }
}

const char *USBUartWord = "USBU";
const char *SCPIUartWord = "SCPI";
const char *MODBusWord = "MODB";

void UARTModeCommand(CliHandle_t *handle, void *v)
{
    if (handle->LastRead == '?')
    {
        ReadChar(handle);
        const char *word;

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

        WriteString(handle, word);
    }
    else if (handle->LastRead == ' ')
    {
        ReadWord(handle);
        if (SCPICompare(USBUartWord, handle->LastWord))
        {
            UART_set_mode(UMODE_USBUART);
        }
        else if (SCPICompare(SCPIUartWord, handle->LastWord))
        {
            UART_set_mode(UMODE_SCPI);
        }
        else if (SCPICompare(MODBusWord, handle->LastWord))
        {
            UART_set_mode(UMODE_MODBus);
        }
        else
        {
            QueueErrorCode(UART_ERROR_INVALID_MODE);
        }
    }
}

void UARTOverflowCommand(CliHandle_t *handle, void *v)
{
    if (handle->LastRead == '?')
    {
        ReadChar(handle);
        WriteNumber(handle, UART_RxBufferOverflow());
    }
}

void UARTClearOverflowCommand(CliHandle_t *handle, void *v)
{
    UART_ClearRxBufferOverflow();
}

CommandDefinition_t uartClearCommandChildren[] = {
    DEFINE_COMMAND("CLEA", UARTOverflowCommand),
};

CommandDefinition_t uartCommandChildren[] = {
    DEFINE_COMMAND("READ", UARTReadCommand),
    DEFINE_COMMAND("WRIT", UARTWriteCommand),
    DEFINE_COMMAND("BAUD", UARTBaudCommand),
    DEFINE_COMMAND("MODE", UARTModeCommand),
    DEFINE_COMMAND_W_BRANCH("OVER", UARTOverflowCommand, uartClearCommandChildren),
};

CommandDefinition_t UARTCommand = DEFINE_BRANCH("UART", uartCommandChildren);
