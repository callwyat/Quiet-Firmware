
#include <stdint.h>
#include <stdbool.h>
#include <xc.h>

#include "constants.h"
#include "outputs.h"

uint16_t servoValue[10];

typedef struct {
    volatile unsigned char *ValueRegister;
    volatile unsigned char *ControlRegister;
    OutputMode_e ActiveMode;
    OutputMode_e AvalibleModes;
    uint16_t ServoValue;
} OutputSetup_t;

#define DEFINE_OUTPUT(controlReg, avalibleModes) {  \
    .ControlRegister = controlReg,                  \
    .ValueRegister = controlReg + 1,                \
    .ActiveMode = OUT_DISCREET,                     \
    .AvalibleModes = avalibleModes,                 \
}

OutputSetup_t OutputSetups[] = {
    DEFINE_OUTPUT(&CCP1CON, OUT_PWM | OUT_SERVO),
    DEFINE_OUTPUT(&CCP2CON, OUT_PWM | OUT_SERVO),
    DEFINE_OUTPUT(&CCP4CON, OUT_DISCREET | OUT_PWM | OUT_SERVO),
    DEFINE_OUTPUT(&CCP5CON, OUT_DISCREET | OUT_PWM | OUT_SERVO),
    DEFINE_OUTPUT(&CCP6CON, OUT_DISCREET | OUT_PWM | OUT_SERVO),
    DEFINE_OUTPUT(&CCP7CON, OUT_DISCREET | OUT_PWM | OUT_SERVO),
    DEFINE_OUTPUT(&CCP3CON, OUT_DISCREET | OUT_SERVO),
    DEFINE_OUTPUT(&CCP3CON, OUT_DISCREET | OUT_SERVO),
    DEFINE_OUTPUT(&CCP3CON, OUT_DISCREET | OUT_SERVO),
    DEFINE_OUTPUT(&CCP3CON, OUT_DISCREET | OUT_SERVO),
};

const uint8_t OutputSetupsCount = sizeof(OutputSetups) / sizeof(OutputSetups[0]);

uint8_t OutputMask = 0xFF;

void SetDiscreetOutput(uint8_t output, bool value)
{
    if (output > 1 && output < 9)
    {
        output -= 2;
        
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
    if (output > 1 && output < 9)
    {
        output -= 2;
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
        return -1;
    }
}

void SetOutputMode(uint8_t output, OutputMode_e mode)
{
    if (output > 1)
    {
        if (mode == OUT_DISCREET)
        {
            OutputMask |= 0x01 << (mode - 2);
        }
        else
        {
            OutputMask &= ~(0x01 << (mode - 2));
        }
    }
    
    OutputSetup_t *setup = &OutputSetups[output];
    if (setup->AvalibleModes & mode)
    {
        setup->ActiveMode = mode;
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
                GetDiscreetOutput(output);
                break;
                
            case OUT_PWM:
                return GetPWMValue(output);
                
            case OUT_SERVO:
                return GetServoValue(output);
        }
    }
    
    return (uint16_t)-1;
}

void ServoTick(void)
{
    static OutputSetup_t *setup = &OutputSetups[0];
    static bool oddEvenTick = true;
    static uint8_t steering = 0x01;
    
    if (setup->ActiveMode == OUT_SERVO)
    {
        if (oddEvenTick)
        {
            *setup->ValueRegister = 0x00;
            *setup->ControlRegister &= 0xCF;
            
            // Increment to the next step
            if (++setup > &OutputSetups[OutputSetupsCount - 1])
            {
                setup = &OutputSetups[0];
                steering= 0x01;
            }
            else if (setup->ControlRegister == &CCP3CON)
            {
                // Rotate which output is being driven
                PSTR3CON &= 0xF0;
                if (setup->ActiveMode == OUT_SERVO)
                    PSTR3CON |= steering & 0x0F;
                
                steering = steering << 1;
            }
            
            if (setup->ActiveMode == OUT_SERVO)
            {
                *setup->ValueRegister = 0xFF;
                *setup->ControlRegister |= 0x30;
            }
        }
        else
        {
            SetCCPValue(setup, setup->ServoValue);
        }
        
        oddEvenTick = !oddEvenTick;
    }
}
