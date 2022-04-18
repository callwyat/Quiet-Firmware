

#include "../CLI/cli.h"
#include "../mcc_generated_files/spi2.h"
#include "../constants.h"

#include <stdbool.h>
#include <stdint.h>

uint16_t spiExchangeSize = 0;

void SPILargeExchange(CliBuffer_t *buffer, void *v)
{
    uint8_t bufferRemaining = (uint8_t)(&buffer->InputBuffer[buffer->InputLength] - buffer->InputPnt);

    if (spiExchangeSize <= bufferRemaining)
    {
        // Read out the data in this buffer
        SPI2_ExchangeBlock((uint8_t*)buffer->InputPnt, (uint8_t*)buffer->OutputPnt,
                spiExchangeSize);    
        buffer->InputPnt += spiExchangeSize;
        buffer->OutputPnt += spiExchangeSize;
        
        ClearLargeDataHandle(buffer);
    }
    else
    {
        spiExchangeSize -= bufferRemaining;

        // Read out the data until all the data is read
        SPI2_ExchangeBlock((uint8_t*)buffer->InputPnt, (uint8_t*)buffer->OutputPnt, 
                bufferRemaining);
        buffer->InputPnt += bufferRemaining;
        buffer->OutputPnt += bufferRemaining;
        
        if (buffer->DataHandle)
        {
            return;
        }
        else
        {
            SetLargeDataHandle(buffer, SPILargeExchange);
        }
    }
}

void SPIExchangeCommand(CliBuffer_t *buffer, void* v)
{    
    ++buffer->InputPnt;

    if (*buffer->InputPnt == '#')
    {
        ++buffer->InputPnt;

        // Parse the number of bytes to write
        spiExchangeSize = ParseIEEEHeader(buffer);

        // Echo back how many bytes we are going to read
        GenerateIEEEHeader(buffer, spiExchangeSize);

        // Check for an invalid number
        if (spiExchangeSize != 0)
        {
            SPILargeExchange(buffer, v);
        }
    }
}

void SPIChipSelectCommand(CliBuffer_t *buffer, void* v)
{
    if (*buffer->InputPnt == ' ')
    {
        ++buffer->InputPnt;
        SDDetectEnable = false;
        
        char c = *buffer->InputPnt;

        if (c == '0')
        {
            ++buffer->InputPnt;
            SPICS = 0;
        }
        else if (c == '1')
        {
            ++buffer->InputPnt;
            SPICS = 1;
        }
        else if (c == 'F')
        {
            FFTilPunctuation(&buffer->InputPnt);
            --buffer->InputPnt;
            SPICS = 0;
        }
        else if (c == 'T')
        {
            FFTilPunctuation(&buffer->InputPnt);
            --buffer->InputPnt;
            SPICS = 1;
        }
        else
        {
            QueueErrorCode(SPI_ERROR_INVALID_CS_VALUE);
        }
    }
        else if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;

        *buffer->OutputPnt++ = SDDetect ? '1' : '0'; 
    }
}

void SPIBaudCommand(CliBuffer_t *buffer, void* v)
{
    if (*buffer->InputPnt == '?')
    {
    ++buffer->InputPnt;

    uint24_t baudRate;
    switch (SSP2CON1bits.SSPM)
    {
        case 0x00:
            baudRate = 4000000;
            break;
        case 0x0A:
            baudRate = 2000000;
            break;
        case 0x01:
            baudRate = 1000000;
            break;
        default:
            baudRate = 250000;
            break;
    }

    NumberToString(buffer, baudRate);
}
    else if (*buffer->InputPnt == ' ')
    {
        ++buffer->InputPnt;

        uint24_t baudRate = ParseInt24(&buffer->InputPnt);
        
        if (baudRate >= 4000000)
        {
            SSP2CON1bits.SSPM = 0x00;
        } 
        else if (baudRate >= 2000000)
        {
            SSP2CON1bits.SSPM = 0x0A;
        } 
        else if (baudRate >= 1000000)
        {
            SSP2CON1bits.SSPM = 0x01;
        }
        else if (baudRate >= 250000)
        {
            SSP2CON1bits.SSPM = 0x02;
        }
        else
        {
            QueueErrorCode(SPI_ERROR_INVALID_BAUD);
        }
    }
}

CommandDefinition_t spiCommands[] = {
    DEFINE_COMMAND("EXCH", SPIExchangeCommand),
    DEFINE_COMMAND("CS", SPIChipSelectCommand),
    DEFINE_COMMAND("BAUD", SPIBaudCommand),
};

CommandDefinition_t SPICommand = DEFINE_BRANCH("SPI", spiCommands);

