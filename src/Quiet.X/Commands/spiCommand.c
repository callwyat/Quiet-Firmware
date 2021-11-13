

#include "../CLI/cli.h"
#include "../mcc_generated_files/spi2.h"
#include "../constants.h"

#include <stdbool.h>
#include <stdint.h>

uint16_t spiExchangeSize = 0;

void SPILargeExchange(CliBuffer_t *buffer)
{
    uint8_t bufferRemaining = &buffer->InputBuffer[buffer->InputLength] - buffer->InputPnt;

    if (spiExchangeSize <= bufferRemaining)
    {
        // Read out the data in this buffer
        do
        {
            *buffer->OutputPnt++ = SPI2_ExchangeByte(*buffer->InputPnt++);
        } while (--spiExchangeSize > 0);
        
        ClearLargeDataHandle(buffer);
    }
    else
    {
        spiExchangeSize -= bufferRemaining;

        // Read out the data until all the data is read
        do 
        {
            *buffer->OutputPnt++ = SPI2_ExchangeByte(*buffer->InputPnt++);
        }   while (--bufferRemaining > 0);

        if (buffer->DataHandle)
        {
            return;
        }
        else
        {
            SetLargeDataHandle(buffer, &SPILargeExchange);
        }
    }
}

void SPIExchangeCommand(CliBuffer_t *buffer)
{
    if (*buffer->InputPnt == ' ')
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
                SPILargeExchange(buffer);
            }
        }
    }
}

void SPIChipSelectCommand(CliBuffer_t *buffer)
{
    if (*buffer->InputPnt == ' ')
    {
        ++buffer->InputPnt;
        SDDetectEnable = false;
        
        if (*buffer->InputPnt == '0')
        {
            ++buffer->InputPnt;
            SPICS = 0;
        }
        else if (*buffer->InputPnt == '1')
        {
            ++buffer->InputPnt;
            SPICS = 1;
        }
        else if (*buffer->InputPnt == 'F')
        {
            FFTilPunctuation(&buffer->InputPnt);
            --buffer->InputPnt;
            SPICS = 0;
        }
        else if (*buffer->InputPnt == 'T')
        {
            FFTilPunctuation(&buffer->InputPnt);
            --buffer->InputPnt;
            SPICS = 1;
        }
    }
    else if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        
        *buffer->OutputPnt++ = SDDetect ? '1' : '0'; 
    }
}

void SPIBaudCommand(CliBuffer_t *buffer)
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
        
        Int24ToString(buffer, baudRate);
    }
    else if (*buffer->InputPnt == ' ')
    {
        ++buffer->InputPnt;
        
        uint24_t buadRate = ParseInt24(&buffer->InputPnt);
        
        if (buadRate >= 4000000)
        {
            SSP2CON1bits.SSPM = 0x00;
        } 
        else if (buadRate >= 2000000)
        {
            SSP2CON1bits.SSPM = 0x0A;
        } 
        else if (buadRate >= 1000000)
        {
            SSP2CON1bits.SSPM = 0x01;
        }
        else
        {
            SSP2CON1bits.SSPM = 0x02;
        }
    }
}

const CommandDefinition spiCommands[] = {
  DEFINE_COMMAND("EXCH", SPIExchangeCommand),
  DEFINE_COMMAND("CS", SPIChipSelectCommand),
  DEFINE_COMMAND("BAUD", SPIBaudCommand),
};

const uint8_t spiCommandCount = sizeof(spiCommands) / sizeof(spiCommands[0]);

void SPICommand(CliBuffer_t *buffer)
{
    if (*buffer->InputPnt == ':')
    {
        ++buffer->InputPnt;
        ProcessCommand(spiCommands, spiCommandCount, buffer, false);
    }
}