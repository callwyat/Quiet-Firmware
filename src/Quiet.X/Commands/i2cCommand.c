

#include "../CLI/cli.h"
#include "../mcc_generated_files/i2c1_master.h"
#include "../constants.h"
#include "../outputs.h"
#include "i2cCommand.h"
#include "../mcc_generated_files/examples/i2c1_master_example.h"

#include <stdbool.h>
#include <stdint.h>

// The address to send the message to
i2c1_address_t i2cTargetAddress = 0x00;
bool i2cEnabled = false;

void I2CEnableCommand(CliBuffer_t *buffer, void* v)
{
    if (*buffer->InputPnt == ' ')
    {
        ++buffer->InputPnt;
        
        char c = *buffer->InputPnt;
        ++buffer->InputPnt;
        
        if (c == '0' || c == 'F')
        {
            i2cEnabled = false;
            I2C1_Close();
            SetOutputMode(I2C_DATA_OUTPUT, OUT_DISCREET);
            SetOutputMode(I2C_CLOCK_OUTPUT, OUT_DISCREET);
        }
        else if (c == '1' || c == 'T')
        {
            i2cEnabled = true;
            SetOutputMode(I2C_DATA_OUTPUT, OUT_I2C);
            SetOutputMode(I2C_CLOCK_OUTPUT, OUT_I2C);
        }
    }
    else if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;

        *buffer->OutputPnt++ = GetOutputMode(I2C_CLOCK_OUTPUT) == OUT_I2C ? '1' : '0'; 
    }
}

void I2CTimeoutCommand(CliBuffer_t *buffer, void* v)
{
    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        
        uint24_t timeout = I2C1_GetTimeout();
        
        NumberToString(buffer, timeout);
    }
    else if (*buffer->InputPnt == ' ')
    {
        ++buffer->InputPnt;
        
        int16_t timeout = ParseInt(&buffer->InputPnt);
        
        if (timeout > 0)
        {
            I2C1_SetTimeout((uint8_t)timeout);
        }
        else
        {
            //TODO: Report invalid timeout
        }
    }
}

void I2CBaudCommand(CliBuffer_t *buffer, void* v)
{
    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        
        uint24_t baudRate = I2C1GetBaudRate();
        
        NumberToString(buffer, baudRate);
    }
    else if (*buffer->InputPnt == ' ')
    {
        ++buffer->InputPnt;
        
        uint24_t baudRate = ParseInt24(&buffer->InputPnt);
        
        // BaudRates below 16k cannot be generated with this the system clock
        if (baudRate > 16000)
        {
            I2C1SetBaudRate(baudRate);
        }
        else
        {
            //TODO: Report invalid baud rate
        }
    }
}

void I2CAddressCommand(CliBuffer_t *buffer, void *v)
{
    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        
        NumberToString(buffer, i2cTargetAddress);
    }
    else if (*buffer->InputPnt == ' ')
    { 
        ++buffer->InputPnt;
        
        int16_t addr = ParseInt(&buffer->InputPnt);

        if (addr >= 0)
        {
            i2cTargetAddress = (uint8_t)addr;
        }
        else
        {
            //TODO: Report Invalid Address
        }
    }
}

void I2CWriteCommand(CliBuffer_t *buffer, void* v)
{
    if (*buffer->InputPnt == ' ')
    {
        ++buffer->InputPnt;

        if (*buffer->InputPnt == '#')
        {
            ++buffer->InputPnt;

            // Parse the number of bytes to write
            uint16_t writeCount = ParseIEEEHeader(buffer);

            // Check for an invalid number
            if (&buffer->InputPnt[writeCount] >= &buffer->InputPnt[CLI_BUFFER_SIZE])
            {
                // TODO: Invalid Write Count
            }
            else if (!i2cEnabled)
            {
                // TODO: Show message about IIC not being enabled
            }
            else if (writeCount != 0)
            {
                I2C1_WriteNBytes(i2cTargetAddress, buffer->InputPnt, writeCount);
            }
            else
            {
                // TODO: Invalid writeCount
            }
        }
    }
}

void I2CReadCommand(CliBuffer_t *buffer, void* v)
{
    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;

        if (*buffer->InputPnt == ' ')
        {
            // Get the number of bytes to read
            int8_t readCount = ParseInt(&buffer->InputPnt);

            if (&buffer->InputPnt[readCount] >= &buffer->InputBuffer[CLI_BUFFER_SIZE])
            {
                // TODO: Buffer Overflow Exception
            }
            else if (!i2cEnabled)
            {
                // TODO: Show message about IIC not being enabled
            }
            else if (readCount > 0)
            {
                GenerateIEEEHeader(buffer, readCount);

                // Whip out any latent data in the output buffer
                while (readCount > 0)
                {
                    *buffer->InputPnt++ = 0x00;

                    --readCount;
                }

                I2C1_ReadNBytes(i2cTargetAddress, buffer->OutputPnt, readCount);
            }
            else
            {
                // TODO: Invalid Read Count
            }
        }
    }
}

void I2CReportCommand(CliBuffer_t *buffer, void* v)
{    
    ++buffer->InputPnt;

    if (*buffer->InputPnt == '?')
    {
        // TODO
    }
}



CommandDefinition_t i2cCommands[] = {
    DEFINE_COMMAND("WRIT", I2CWriteCommand),
    DEFINE_COMMAND("READ", I2CReadCommand),
    DEFINE_COMMAND("REPO", I2CReportCommand),
    DEFINE_COMMAND("ADDR", I2CAddressCommand),
    DEFINE_COMMAND("ENAB", I2CEnableCommand),
    DEFINE_COMMAND("BAUD", I2CBaudCommand),
    DEFINE_COMMAND("TIME", I2CTimeoutCommand),
};

CommandDefinition_t IICCommand = DEFINE_BRANCH("IIC", i2cCommands);
CommandDefinition_t I2CCommand = DEFINE_BRANCH("I2C", i2cCommands);
CommandDefinition_t I22CCommand = DEFINE_BRANCH("I²C", i2cCommands);


