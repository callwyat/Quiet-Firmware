

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
uint8_t i2cRegisterAddress = 0x0000;
// The size of the register to work with in the slave device in bytes
uint8_t i2cRegisterSize = 1;

#define I2C_ERROR_NONE 0x00
#define I2C_ERROR_INVALID_BAUD 0x01
#define I2C_ERROR_INVALID_TIMEOUT 0x02
#define I2C_ERROR_INVALID_SLAVE_ADDRESS 0x03

#define I2C_ERROR_DISABLED_WRITE 0x10
#define I2C_ERROR_DISABLED_READ 0x11
#define I2C_ERROR_NO_ACKNOWLEDGE 0x12

#define I2C_ERROR_INVALID_RSIZE 0x20
#define I2C_ERROR_INVALID_REGISTER_ADDRESS 0x21
#define I2C_ERROR_INVALID_REGISTER_VALUE 0x22

#define I2C_ERROR_BUFFER_OVERFLOW 0x30
#define I2C_ERROR_INVALID_WRITE_SIZE 0x31
#define I2C_ERROR_INVALID_READ_SIZE 0x32

// The last generated error to occur
uint8_t i2cErrorCode = I2C_ERROR_NONE;

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
        
        if (timeout > 0 && timeout < 256)
        {
            I2C1_SetTimeout((uint8_t)timeout);
        }
        else
        {
            i2cErrorCode = I2C_ERROR_INVALID_TIMEOUT;
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
        if (baudRate >= 16000 && baudRate <= 1000000)
        {
            I2C1SetBaudRate(baudRate);
        }
        else
        {
            i2cErrorCode = I2C_ERROR_INVALID_BAUD;
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

        if (addr >= 0 && addr < 128)
        {
            i2cTargetAddress = (uint8_t)addr;
        }
        else
        {
            i2cErrorCode = I2C_ERROR_INVALID_SLAVE_ADDRESS;
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
            if (&buffer->InputPnt[writeCount] >= &buffer->InputBuffer[CLI_BUFFER_SIZE])
            {
                i2cErrorCode = I2C_ERROR_BUFFER_OVERFLOW;
            }
            else if (!I2C1_GetEnabled())
            {
                i2cErrorCode = I2C_ERROR_DISABLED_WRITE;
            }
            else if (writeCount != 0)
            {
                I2C1_WriteNBytes(i2cTargetAddress, (uint8_t*)buffer->InputPnt, writeCount);
            }
            else
            {
                i2cErrorCode = I2C_ERROR_INVALID_WRITE_SIZE;
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
            ++buffer->InputPnt;

            // Get the number of bytes to read
            int8_t readCount = (int8_t)ParseInt(&buffer->InputPnt);

            if (&buffer->OutputPnt[readCount] >= &buffer->OutputBuffer[CLI_BUFFER_SIZE])
            {
                i2cErrorCode = I2C_ERROR_BUFFER_OVERFLOW;
                CopyWordToOutBuffer(buffer, EmptyIEEEHeader);
            }
            else if (!I2C1_GetEnabled())
            {
                i2cErrorCode = I2C_ERROR_DISABLED_READ;
                CopyWordToOutBuffer(buffer, EmptyIEEEHeader);
            }
            else if (readCount > 0)
            {
                GenerateIEEEHeader(buffer, (uint16_t)readCount);

                // Whip out any latent data in the output buffer
                int8_t clearCount = readCount;
                char *pnt = buffer->OutputPnt;
                while (clearCount > 0)
                {
                    *pnt++ = 0x00;
                    --clearCount;
                }

                I2C1_ReadNBytes(i2cTargetAddress, (uint8_t*)buffer->OutputPnt, (size_t)readCount);
                
                buffer->OutputPnt += readCount;
            }
            else
            {
                i2cErrorCode = I2C_ERROR_INVALID_READ_SIZE;                
                CopyWordToOutBuffer(buffer, EmptyIEEEHeader);
            }
        }
    }
}

void I2CErrorCommand(CliBuffer_t *buffer, void* v)
{    
    if (*buffer->InputPnt == '?')
    {
        ++buffer->InputPnt;
        
        // Check for a NACK
        if (I2C1_LastOperationNACKed() && i2cErrorCode == 0)
        {
            i2cErrorCode = I2C_ERROR_NO_ACKNOWLEDGE;
        }
        
        NumberToString(buffer, i2cErrorCode);
                
        i2cErrorCode = I2C_ERROR_NONE;
    }
}

void I2CRegisterWriteCommand(CliBuffer_t *buffer, void* v)
{
    if (*buffer->InputPnt == ' ')
    { 
        ++buffer->InputPnt;
        
        uint16_t data = (uint16_t)ParseInt(&buffer->InputPnt);

        if (I2C1_GetEnabled())
        {
            if (i2cRegisterSize == 2)
            {
                I2C1_Write2ByteRegister(i2cTargetAddress, i2cRegisterAddress, data);
            }
            else if (i2cRegisterSize == 1)
            {
                if (data < 256)
                {
                    I2C1_Write1ByteRegister(i2cTargetAddress, i2cRegisterAddress, (uint8_t)data);
                }
                else
                {
                    i2cErrorCode = I2C_ERROR_INVALID_REGISTER_VALUE;
                }
            }
            else
            {
                i2cErrorCode = I2C_ERROR_INVALID_RSIZE;
            }
        }
        else
        {
            i2cErrorCode = I2C_ERROR_DISABLED_WRITE;
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
            uint16_t data;

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
                i2cErrorCode = I2C_ERROR_INVALID_RSIZE;
            }

            NumberToString(buffer, data);
        }
        else
        {
            i2cErrorCode = I2C_ERROR_DISABLED_READ;
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

        if (address >= 0 && address < 256)
        {
            i2cRegisterAddress = (uint8_t)address;
        }
        else
        {
            i2cErrorCode = I2C_ERROR_INVALID_REGISTER_ADDRESS;
        }
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

        if (size >= 1 && size <= 2)
        {
            i2cRegisterSize = (uint8_t)size;
        }
        else
        {
            i2cErrorCode = I2C_ERROR_INVALID_RSIZE;
        }
    }
}


CommandDefinition_t i2cRegisterCommands[] = {
    DEFINE_COMMAND("ADDR", I2CRegisterAddressCommand),
    DEFINE_COMMAND("RSIZ", I2CRegisterRegisterSizeCommand),
    DEFINE_COMMAND("WRIT", I2CRegisterWriteCommand),
    DEFINE_COMMAND("READ", I2CRegisterReadCommand),
    DEFINE_COMMAND("ERRO", I2CErrorCommand),
};

CommandDefinition_t i2cCommands[] = {
    DEFINE_BRANCH("REGI", i2cRegisterCommands),
    DEFINE_COMMAND("ADDR", I2CAddressCommand),
    DEFINE_COMMAND("WRIT", I2CWriteCommand),
    DEFINE_COMMAND("READ", I2CReadCommand),
    DEFINE_COMMAND("ERRO", I2CErrorCommand),
    DEFINE_COMMAND("ENAB", I2CEnableCommand),
    DEFINE_COMMAND("BAUD", I2CBaudCommand),
    DEFINE_COMMAND("TIME", I2CTimeoutCommand),
};

CommandDefinition_t IICCommand = DEFINE_BRANCH("IIC", i2cCommands);
CommandDefinition_t I2CCommand = DEFINE_BRANCH("I2C", i2cCommands);
CommandDefinition_t I22CCommand = DEFINE_BRANCH("I²C", i2cCommands);


