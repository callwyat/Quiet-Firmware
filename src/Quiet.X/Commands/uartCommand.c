

#include "../CLI/cli.h"
#include "../mcc_generated_files/eusart1.h"
#include "../mcc_generated_files/eusart2.h"


void UARTReadCommand(CliBuffer_t *buffer)
{
    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        
        uint8_t receivedData = EUSART2_get_rx_count();
        
        uint8_t maxSendSize = (&buffer->OutputBuffer[sizeof(buffer->OutputBuffer)] - buffer->OutputPnt) - 4;
        
        if (receivedData > maxSendSize)
        {
            receivedData = maxSendSize;
        }
        
        GenerateIEEEHeader(buffer, receivedData);
        
        while (receivedData-- > 0)
        {
            *buffer->OutputPnt++ = EUSART2_Read();
        }
    }
}

uint16_t readSize = 0;

void UARTLargeWrite(CliBuffer_t *buffer)
{
    uint8_t bufferRemaining = &buffer->InputBuffer[buffer->InputLength] - buffer->InputPnt;

    if (readSize <= bufferRemaining)
    {
        // Read out the data in this buffer
        do
        {
            EUSART1_Write((uint8_t)*buffer->InputPnt++);
        } while (--readSize > 0);
        
        ClearLargeDataHandle(buffer);
    }
    else
    {
        readSize -= bufferRemaining;

        // Read out the data until all the data is read
        do 
        {
            EUSART1_Write((uint8_t)*buffer->InputPnt++);
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

void UARTWriteCommand(CliBuffer_t *buffer)
{
    if (*buffer->InputPnt == ' ')
    {        
        ++buffer->InputPnt;
        
        if (*buffer->InputPnt == '#')
        {
            ++buffer->InputPnt;
            
            readSize = ParseIEEEHeader(buffer);
            
            // Check for an invalid number
            if (readSize != 0)
            {
                UARTLargeWrite(buffer);
            }
        }
    }
}

void UARTBaudCommand(CliBuffer_t *buffer)
{
    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        
        uint24_t baudRate = EUART1_get_baud_rate();
        
        Int24ToString(buffer, baudRate);
    }
    else if (*buffer->InputPnt == ' ')
    {
        ++buffer->InputPnt;
        
        uint24_t buadRate = ParseInt24(&buffer->InputPnt);
        
        // BaudRates below 60 cannot be generated with this the system clock
        if (buadRate > 60)
        {
            EUART1_set_baud_rate(buadRate);
        }

    }
}

const CommandDefinition uartCommands[] = {
  DEFINE_COMMAND("READ", UARTReadCommand),
  DEFINE_COMMAND("WRIT", UARTWriteCommand),
  DEFINE_COMMAND("BAUD", UARTBaudCommand),
};

const uint8_t uartCommandCount = sizeof(uartCommands) / sizeof(uartCommands[0]);

void UARTCommand(CliBuffer_t *buffer)
{
    if (*buffer->InputPnt == ':')
    {
        ++buffer->InputPnt;
        ProcessCommand(uartCommands, uartCommandCount, buffer, false);
    }
}