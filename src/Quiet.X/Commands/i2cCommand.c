

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
// The address of the register to work with in the slave device
uint16_t i2cRegisterAddress = 0x0000;
// The size of the register to work with in the slave device in bytes
uint8_t i2cRegisterSize = 1;

void I2CEnableCommand(CliBuffer_t *buffer, void* v)
{
    if (*buffer->InputPnt == ' ')
    {
        ++buffer->InputPnt;
        
        char c = *buffer->InputPnt;
        ++buffer->InputPnt;
        
        if (c == '0' || c == 'F')
        {
            I2C1_SetEnabled(false);
        }
        else if (c == '1' || c == 'T')
        {
            I2C1_SetEnabled(true);
        }

    }
    else if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;

        *buffer->OutputPnt++ = I2C1_GetEnabled() ? '1' : '0'; 
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
            else if (!I2C1_GetEnabled())
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
            else if (!I2C1_GetEnabled())
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

void I2CRegisterWriteCommand(CliBuffer_t *buffer, void* v)
{
    if (*buffer->InputPnt == ' ')
    { 
        ++buffer->InputPnt;
        
        int16_t data = ParseInt(&buffer->InputPnt);

        if (I2C1_GetEnabled())
        {
            if (i2cRegisterSize == 2)
            {
                I2C1_Write2ByteRegister(i2cTargetAddress, i2cRegisterAddress, data);
            }
            else if (i2cRegisterSize == 1)
            {
                I2C1_Write1ByteRegister(i2cTargetAddress, i2cRegisterAddress, data);
            }
            else
            {
                // TODO: Error about a bad data size
            }
        }
        else
        {
            // TODO: Error about I2C not being enabled
        }
    }
}

void I2CRegisterReadCommand(CliBuffer_t *buffer, void* v)
{
    if (*buffer->InputPnt == '?')
    { 
        ++buffer->InputPnt;
        
        if (I2C1_GetEnabled())
        {
            int16_t data;

            if (i2cRegisterSize == 2)
            {
                data = I2C1_Read2ByteRegister(i2cTargetAddress, i2cRegisterAddress);
            }
            else if (i2cRegisterSize == 1)
            {
                data = I2C1_Read1ByteRegister(i2cTargetAddress, i2cRegisterAddress);
            }
            else
            {
                // TODO: Error about a bad data size
            }

            NumberToString(buffer, data);
        }
        else
        {
            // TODO: Error about I2C not being enabled
        }
    }
}

void I2CRegisterAddressCommand(CliBuffer_t *buffer, void* v)
{
    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        
        NumberToString(buffer, i2cRegisterAddress);
    }
    else if (*buffer->InputPnt == ' ')
    { 
        ++buffer->InputPnt;
        
        int16_t address = ParseInt(&buffer->InputPnt);

        i2cRegisterAddress = (uint16_t)address;
    }
}

void I2CRegisterRegisterSizeCommand(CliBuffer_t *buffer, void* v)
{
    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        
        NumberToString(buffer, i2cRegisterSize);
    }
    else if (*buffer->InputPnt == ' ')
    { 
        ++buffer->InputPnt;
        
        int16_t size = ParseInt(&buffer->InputPnt);

        if (size >= 0 && size <= 2)
        {
            i2cRegisterSize = (uint8_t)size;
        }
        else
        {
            //TODO: Report Invalid Register Size
        }
    }
}


CommandDefinition_t i2cRegisterCommands[] = {
    DEFINE_COMMAND("WRIT", I2CRegisterWriteCommand),
    DEFINE_COMMAND("READ", I2CRegisterReadCommand),
    DEFINE_COMMAND("ADDR", I2CRegisterAddressCommand),
    DEFINE_COMMAND("RSIZ", I2CRegisterRegisterSizeCommand),
};

CommandDefinition_t i2cCommands[] = {
    DEFINE_BRANCH("REGI", i2cRegisterCommands),
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


