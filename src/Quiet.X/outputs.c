
#include <stdint.h>
#include <stdbool.h>
#include <xc.h>

#include "constants.h"
#include "outputs.h"

uint16_t servoValue[10];

#define DIGO_OFFSET 0
#define DIGO_COUNT 8

typedef struct {
    volatile unsigned char *ValueRegister;
    volatile unsigned char *ControlRegister;
    OutputMode_e ActiveMode;
    OutputMode_e AvalibleModes;
    uint16_t ServoValue;
} OutputSetup_t;

#define DEFINE_OUTPUT(controlReg, defaultMode, avalibleModes) {  \
    .ControlRegister = controlReg,                               \
    .ValueRegister = controlReg + 1,                             \
    .ActiveMode = defaultMode,                                   \
    .AvalibleModes = defaultMode | (avalibleModes),              \
    .ServoValue = 0x0200 /* Start servos in the idle position*/  \
}

OutputSetup_t OutputSetups[] = {
    DEFINE_OUTPUT(&CCP3CON, OUT_DISCREET, OUT_SERVO),
    DEFINE_OUTPUT(&CCP3CON, OUT_DISCREET, OUT_SERVO),
    DEFINE_OUTPUT(&CCP3CON, OUT_DISCREET, OUT_SERVO),
    DEFINE_OUTPUT(&CCP3CON, OUT_DISCREET, OUT_SERVO),
    DEFINE_OUTPUT(&CCP4CON, OUT_DISCREET, OUT_PWM | OUT_SERVO | OUT_I2C),
    DEFINE_OUTPUT(&CCP5CON, OUT_DISCREET, OUT_PWM | OUT_SERVO | OUT_I2C),
    DEFINE_OUTPUT(&CCP6CON, OUT_DISCREET, OUT_PWM | OUT_SERVO),
    DEFINE_OUTPUT(&CCP7CON, OUT_DISCREET, OUT_PWM | OUT_SERVO),
    DEFINE_OUTPUT(&CCP1CON, OUT_PWM, OUT_SERVO),
    DEFINE_OUTPUT(&CCP2CON, OUT_PWM, OUT_SERVO),
};

const uint8_t OutputSetupsCount = sizeof(OutputSetups) / sizeof(OutputSetups[0]);

uint8_t OutputMask = 0xFF;

void SetDiscreetOutput(uint8_t output, bool value)
{
    if (output >= DIGO_OFFSET && output <= (DIGO_OFFSET + 7))
    {
        output -= DIGO_OFFSET;
        
        uint8_t v = DOUT;
        
        if (value)
        {
            v |= (0x01 << output);
        }
        else
        {
            v &= ~(0x01 << output);
        }
        
        SetDiscreetOutputs(v);
    }
}

bool GetDiscreetOutput(uint8_t output)
{
    if (output >= DIGO_OFFSET && output <= (DIGO_OFFSET + 7))
    {
        output -= DIGO_OFFSET;
        return (DOUT >> output) & 0x01;
    }
    
    return false;
}

void SetDiscreetOutputs(uint8_t value)
{
    DOUT = value & OutputMask;
}

OutputMode_e GetOutputMode(uint8_t channel)
{
    if (channel < OutputSetupsCount)
    {
        return OutputSetups[channel].ActiveMode;
    }
    else
    {
        return (OutputMode_e)-1;
    }
}

const char* OutputModeToString(OutputMode_e mode)
{
    switch (mode)
    {
        case OUT_DISCREET:
            return DISCREETWord;

        case OUT_PWM:
            return PWMWord;

        case OUT_SERVO:
            return ServoWord;
            
        case OUT_UART:
            return UARTWord;
            
        case OUT_SPI:
            return SPIWord;
            
        case OUT_I2C:
            return I2CWord;
            
        default:
            return "UNKNOWN";
    }
}

void SetOutputMode(uint8_t output, OutputMode_e mode)
{
    if (output >= OutputSetupsCount)
        return;
    
    OutputSetup_t *setup = &OutputSetups[output];
    
    if ((setup->AvalibleModes & mode) == 0)
        return;
    
    setup->ActiveMode = mode;
    
    if (output >= DIGO_OFFSET && output < (DIGO_OFFSET + DIGO_COUNT))
    {
        if (mode == OUT_DISCREET)
        {
            OutputMask |= 0x01 << (output - DIGO_OFFSET);
        }
        else
        {
            OutputMask &= ~(0x01 << (output - DIGO_OFFSET));
        }
        
        // Outputs 4, 5, 6, and 7 need the CCP module turned on if PWM or Servo
        if (output >= 4 && output <= 7)
        {
            // Default to Discreet Mode
            DOUTTRIS &= ~(0x01 << (output - DIGO_OFFSET));
            *setup->ControlRegister = 0x00;
            
            switch (mode)
            {
                case OUT_PWM:
                case OUT_SERVO:
                    *setup->ControlRegister = 0x0C;
                    break;
                    
                case OUT_I2C:
                    DOUTTRIS |= 0x01 << (output - DIGO_OFFSET);
                    break;
                    
                default:
                    // Default is already loaded
                    break;
            }
        }
    }
}

void SetServoValue(uint8_t output, uint16_t value)
{
    if (output < OutputSetupsCount)
    {
        OutputSetups[output].ServoValue = value;
    }
}

uint16_t GetServoValue(uint8_t output)
{
    if (output < OutputSetupsCount)
    {
        return OutputSetups[output].ServoValue;
    }
    else
    {
        return (uint16_t)-1;
    }
}

__inline__ void SetCCPValue(OutputSetup_t *setup, uint16_t value)
{
    *setup->ValueRegister = (uint8_t)(value >> 2);
            
    // Clear the LSBs
    *setup->ControlRegister &= 0xCF;

    // Set the LSBs
    *setup->ControlRegister |= (value & 0x03) << 4;
}

void SetPWMValue(uint8_t output, uint16_t value)
{
    if (output < OutputSetupsCount)
    {
        OutputSetup_t *setup = &OutputSetups[output];
        
        if (setup->ActiveMode == OUT_PWM)
        {
            SetCCPValue(setup, value);
        }
    }
}

uint16_t GetPWMValue(uint8_t output)
{
    if (output < OutputSetupsCount)
    {
        OutputSetup_t *setup = &OutputSetups[output];
        
        if (setup->ActiveMode == OUT_PWM)
        {
            uint16_t result = *setup->ValueRegister;
            result = result << 2;
            
            result |= (*setup->ControlRegister & 0x30) >> 4;
            
            return result;
        }
    }
    
    return (uint16_t)-1;
}

void SetOutputValue(uint8_t output, uint16_t value)
{
    if (output < OutputSetupsCount)
    {
        OutputSetup_t *setup = &OutputSetups[output];
        
        switch (setup->ActiveMode)
        {
            case OUT_DISCREET:
                SetDiscreetOutput(output, value);
                break;
                
            case OUT_PWM:
                SetPWMValue(output, value);
                break;
                
            case OUT_SERVO:
                SetServoValue(output, value);
                break;
                
            case OUT_UART:
            case OUT_SPI:
            case OUT_I2C:
                // Nothing to do here
                break;
        }
    }
}

uint16_t GetOutputValue(uint8_t output)
{
    if (output < OutputSetupsCount)
    {
        OutputSetup_t *setup = &OutputSetups[output];
        
        switch (setup->ActiveMode)
        {
            case OUT_DISCREET:
                return GetDiscreetOutput(output);
                
            case OUT_PWM:
                return GetPWMValue(output);
                
            case OUT_SERVO:
                return GetServoValue(output);
                
            case OUT_UART:
            case OUT_SPI:
            case OUT_I2C:
                return GetDiscreetOutput(output);
        }
    }
    
    return (uint16_t)-1;
}

void ServoTick(void)
{
    static OutputSetup_t *setup = OutputSetups;
    static bool oddEvenTick = true;
    static uint8_t steering = 0x01;
    
    if (oddEvenTick)
    {
        if (setup->ActiveMode == OUT_SERVO)
        {
            *setup->ValueRegister = 0x00;
            *setup->ControlRegister &= 0xCF;
        }

        // Rotate which output is being driven
        PSTR3CON &= 0xF0;
            
        // Increment to the next step
        if (++setup > &OutputSetups[OutputSetupsCount - 1])
        {
            setup = OutputSetups;
            steering= 0x08;
        }
        
        if (setup->ControlRegister == &CCP3CON)
        {
            if (setup->ActiveMode == OUT_SERVO)
                PSTR3CON |= steering & 0x0F;

            steering = (uint8_t)(steering >> 1);
        }

        if (setup->ActiveMode == OUT_SERVO)
        {
            *setup->ValueRegister = 0xFF;
            *setup->ControlRegister |= 0x30;
        }
    }
    else if (setup->ActiveMode == OUT_SERVO)
    {
        SetCCPValue(setup, setup->ServoValue);
    }

    oddEvenTick = !oddEvenTick;
}
