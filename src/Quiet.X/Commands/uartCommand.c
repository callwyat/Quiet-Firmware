

#include "../CLI/cli.h"
#include "../mcc_generated_files/eusart1.h"
#include "../mcc_generated_files/eusart2.h"


void UARTReadCommand(CliBuffer *buffer)
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
        
        while (receivedData > 0)
        {
            *buffer->OutputPnt = EUSART2_Read();
        }
    }
}

uint16_t readSize = 0;

void UARTLargeWrite(CliBuffer *buffer)
{
    uint8_t bufferRemaining = &buffer->InputBuffer[buffer->InputLength] - buffer->InputBuffer;

    if (readSize <= bufferRemaining)
    {
        // Read out the data in this buffer
        do
        {
            EUSART1_Write((uint8_t)buffer->InputPnt++);
        } while (--readSize > 0);
        
        ClearLargeDataHandle(buffer);
    }
    else
    {
        readSize -= bufferRemaining;

        // Read out the data until all the data is read
        do 
        {
            EUSART1_Write((uint8_t)buffer->InputPnt++);
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

void UARTWriteCommand(CliBuffer *buffer)
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

            }
        }
    }
}

const CommandDefinition uartCommands[] = {
  DEFINE_COMMAND("READ", UARTReadCommand),
  DEFINE_COMMAND("WRIT", UARTWriteCommand),
};

const uint8_t uartCommandCount = sizeof(uartCommands) / sizeof(uartCommands[0]);

void UARTCommand(CliBuffer *buffer)
{
    if (*buffer->InputPnt == ':')
    {
        ++buffer->InputPnt;
        ProcessCommand(uartCommands, uartCommandCount, buffer, false);
    }
}