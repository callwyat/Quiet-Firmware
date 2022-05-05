

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

const char *OFFWord = "OFF";
const char *MASTerWord = "MAST";

#define I2C_N_BUFFER_SIZE 64
uint8_t nBuffer[I2C_N_BUFFER_SIZE];

void I2CModeCommand(CliHandle_t *handle, void *v)
{
    if (handle->LastRead == ' ')
    {
        ReadWord(handle);
        if (SCPICompare(OFFWord, handle->LastWord))
        {
            I2C1_SetEnabled(false);
        }
        else if (SCPICompare(MASTerWord, handle->LastWord))
        {
            I2C1_SetEnabled(true);
        }
        else
        {
            QueueErrorCode(I2C_ERROR_INVALID_MODE);
        }
    }
    else if (handle->LastRead == '?')
    {
        ReadChar(handle);
        const char *word = I2C1_GetEnabled() ? MASTerWord : OFFWord;

        WriteString(handle, word);
    }
}

void I2CTimeoutCommand(CliHandle_t *handle, void *v)
{
    if (handle->LastRead == '?')
    {
        ReadChar(handle);
        uint24_t timeout = I2C1_GetTimeout();

        WriteNumber(handle, timeout);
    }
    else if (handle->LastRead == ' ')
    {
        uint16_t timeout = ReadInt(handle);

        if (timeout > 0 && timeout < 256)
        {
            I2C1_SetTimeout((uint8_t)timeout);
        }
        else
        {
            QueueErrorCode(I2C_ERROR_INVALID_TIMEOUT);
        }
    }
}

void I2CBaudCommand(CliHandle_t *handle, void *v)
{
    if (handle->LastRead == '?')
    {
        ReadChar(handle);
        uint24_t baudRate = I2C1GetBaudRate();

        WriteNumber(handle, baudRate);
    }
    else if (handle->LastRead == ' ')
    {
        uint24_t baudRate = ReadInt24(handle);

        // BaudRates below 16k cannot be generated with this the system clock
        if (baudRate >= 16000 && baudRate <= 1000000)
        {
            I2C1SetBaudRate(baudRate);
        }
        else
        {
            QueueErrorCode(I2C_ERROR_INVALID_BAUD);
        }
    }
}

void I2CAddressCommand(CliHandle_t *handle, void *v)
{
    if (handle->LastRead == '?')
    {
        ReadChar(handle);
        WriteNumber(handle, i2cTargetAddress);
    }
    else if (handle->LastRead == ' ')
    {
        uint16_t addr = ReadInt(handle);

        if (addr < 128)
        {
            i2cTargetAddress = (uint8_t)addr;
        }
        else
        {
            QueueErrorCode(I2C_ERROR_INVALID_SLAVE_ADDRESS);
        }
    }
}

void I2CWriteCommand(CliHandle_t *handle, void *v)
{
    if (handle->LastRead == ' ')
    {
        ReadChar(handle);
        if (handle->LastRead == '#')
        {
            // Parse the number of bytes to write
            uint16_t writeCount = ReadIEEEHeader(handle);

            // Check for an invalid number
            if (!I2C1_GetEnabled())
            {
                QueueErrorCode(I2C_ERROR_DISABLED_WRITE);
            }
            else if (writeCount > 0 && writeCount <= I2C_N_BUFFER_SIZE)
            {
                uint8_t *nPnt = nBuffer;
                uint8_t i = (uint8_t)writeCount;
                while (i > 0)
                {
                    *nPnt++ = handle->Read();
                    --i;
                }

                I2C1_WriteNBytes(i2cTargetAddress, nBuffer, writeCount);
            }
            else
            {
                // Read off the data that was sent, but can't be used.
                while (writeCount > 0)
                {
                    handle->Read();
                    --writeCount;
                }

                ReadChar(handle);                
                QueueErrorCode(I2C_ERROR_INVALID_WRITE_SIZE);
            }
        }
    }
}

void I2CReadCommand(CliHandle_t *handle, void *v)
{
    if (handle->LastRead == '?')
    {
        ReadChar(handle);

        if (handle->LastRead == ' ')
        {
            // Get the number of bytes to read
            int8_t readCount = (int8_t)ReadInt(handle);

            if (!I2C1_GetEnabled())
            {
                QueueErrorCode(I2C_ERROR_DISABLED_READ);
                WriteString(handle, EmptyIEEEHeader);
            }
            else if (readCount > 0 && readCount <= I2C_N_BUFFER_SIZE)
            {
                WriteIEEEHeader(handle, (uint16_t)readCount);

                // Whip out any left over data in the buffer
                uint8_t clearCount = (uint8_t)readCount;
                uint8_t *nPnt = nBuffer;
                while (clearCount > 0)
                {
                    *nPnt++ = 0x00;
                    --clearCount;
                }

                I2C1_ReadNBytes(i2cTargetAddress, nBuffer, (size_t)readCount);

                nPnt = nBuffer;
                while (readCount > 0)
                {
                    handle->Write(*nPnt++);
                    --readCount;
                }
            }
            else
            {
                QueueErrorCode(I2C_ERROR_INVALID_READ_SIZE);
                WriteString(handle, EmptyIEEEHeader);
            }
        }
        else
        {
            QueueErrorCode(I2C_ERROR_INVALID_READ_SYNTAX);
        }
    }
}

void I2CACKedCommand(CliHandle_t *handle, void *v)
{
    if (handle->LastRead == '?')
    {
        ReadChar(handle);
        WriteNumber(handle, !I2C1_LastOperationNACKed());
    }
}

void I2CRegisterWriteCommand(CliHandle_t *handle, void *v)
{
    if (handle->LastRead == ' ')
    {
        uint16_t data = (uint16_t)ReadInt(handle);

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
                    QueueErrorCode(I2C_ERROR_INVALID_REGISTER_VALUE);
                }
            }
            else
            {
                QueueErrorCode(I2C_ERROR_INVALID_RSIZE);
            }
        }
        else
        {
            QueueErrorCode(I2C_ERROR_DISABLED_WRITE);
        }
    }
}

void I2CRegisterReadCommand(CliHandle_t *handle, void *v)
{
    if (handle->LastRead == '?')
    {
        ReadChar(handle);
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
                QueueErrorCode(I2C_ERROR_INVALID_RSIZE);
            }

            WriteNumber(handle, data);
        }
        else
        {
            QueueErrorCode(I2C_ERROR_DISABLED_READ);
        }
    }
}

void I2CRegisterAddressCommand(CliHandle_t *handle, void *v)
{
    if (handle->LastRead == '?')
    {
        ReadChar(handle);
        WriteNumber(handle, i2cRegisterAddress);
    }
    else if (handle->LastRead == ' ')
    {
        uint16_t address = ReadInt(handle);

        if (address < 256)
        {
            i2cRegisterAddress = (uint8_t)address;
        }
        else
        {
            QueueErrorCode(I2C_ERROR_INVALID_REGISTER_ADDRESS);
        }
    }
}

void I2CRegisterRegisterSizeCommand(CliHandle_t *handle, void *v)
{
    if (handle->LastRead == '?')
    {
        ReadChar(handle);
        WriteNumber(handle, i2cRegisterSize);
    }
    else if (handle->LastRead == ' ')
    {
        uint16_t size = ReadInt(handle);

        if (size >= 1 && size <= 2)
        {
            i2cRegisterSize = (uint8_t)size;
        }
        else
        {
            QueueErrorCode(I2C_ERROR_INVALID_RSIZE);
        }
    }
}

CommandDefinition_t i2cRegisterCommands[] = {
    DEFINE_COMMAND("ADDR", I2CRegisterAddressCommand),
    DEFINE_COMMAND("RSIZ", I2CRegisterRegisterSizeCommand),
    DEFINE_COMMAND("WRIT", I2CRegisterWriteCommand),
    DEFINE_COMMAND("READ", I2CRegisterReadCommand),
    DEFINE_COMMAND("ACK", I2CACKedCommand),
};

CommandDefinition_t i2cCommands[] = {
    DEFINE_BRANCH("REGI", i2cRegisterCommands),
    DEFINE_COMMAND("ADDR", I2CAddressCommand),
    DEFINE_COMMAND("WRIT", I2CWriteCommand),
    DEFINE_COMMAND("READ", I2CReadCommand),
    DEFINE_COMMAND("ACK", I2CACKedCommand),
    DEFINE_COMMAND("MODE", I2CModeCommand),
    DEFINE_COMMAND("BAUD", I2CBaudCommand),
    DEFINE_COMMAND("TIME", I2CTimeoutCommand),
};

CommandDefinition_t IICCommand = DEFINE_BRANCH("IIC", i2cCommands);
CommandDefinition_t I2CCommand = DEFINE_BRANCH("I2C", i2cCommands);
CommandDefinition_t I22CCommand = DEFINE_BRANCH("I²C", i2cCommands);
