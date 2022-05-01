

#include "../CLI/cli.h"
#include "../mcc_generated_files/spi2.h"
#include "../constants.h"

#include <stdbool.h>
#include <stdint.h>

void SPIExchangeCommand(CliHandle_t *handle, void *v)
{
    if (handle->LastRead == ' ')
    {
        ReadChar(handle);
        if (handle->LastRead == '#')
        {
            // Parse the number of bytes to write
            uint16_t spiExchangeSize = ParseIEEEHeader(handle);

            // Echo back how many bytes we are going to read
            GenerateIEEEHeader(handle, spiExchangeSize);

            while (spiExchangeSize > 0)
            {
                handle->Write(SPI2_ExchangeByte(handle->Read()));
                --spiExchangeSize;
            }
        }
    }
}

void SPIChipSelectCommand(CliHandle_t *handle, void *v)
{
    if (handle->LastRead == ' ')
    {
        SDDetectEnable = false;
        SPICS = ReadBool(handle);
    }
    else if (handle->LastRead == '?')
    {
        WriteChar(handle, (SDDetect ? '1' : '0'));
    }
}

void SPIBaudCommand(CliHandle_t *handle, void *v)
{
    if (handle->LastRead == '?')
    {
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

        PrintNumber(handle, baudRate);
    }
    else if (handle->LastRead == ' ')
    {
        uint24_t baudRate = ReadInt24(handle);

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
