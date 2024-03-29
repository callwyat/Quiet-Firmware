/* 
 * File:   outputs.h
 * Author: callwyat
 *
 * Created on October 26, 2021, 11:36 PM
 */

#ifndef OUTPUTS_H
#define	OUTPUTS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
    
#define I2C_DATA_OUTPUT 4
#define I2C_CLOCK_OUTPUT 5

extern const char* DISCREETWord;
extern const char* PWMWord;
extern const char* ServoWord;
extern const char* UARTWord;
extern const char* SPIWord;
extern const char* I2CWord;

typedef enum {
    OUT_DISCREET = 0x01,
    OUT_PWM      = 0x02,
    OUT_SERVO    = 0x04,
    OUT_UART     = 0x08,
    OUT_SPI      = 0x10,
    OUT_I2C      = 0x20,
} OutputMode_e;

OutputMode_e GetOutputMode(uint8_t channel);

const char* OutputModeToString(OutputMode_e mode);

uint8_t SetOutputMode(uint8_t output, OutputMode_e mode);

void SetDiscreetOutput(uint8_t output, bool value);

bool GetDiscreetOutput(uint8_t output);

void SetDiscreetOutputs(uint8_t value);

void SetServoValue(uint8_t output, uint16_t value);

uint16_t GetServoValue(uint8_t output);

void SetPWMValue(uint8_t output, uint16_t value);

uint16_t GetPWMValue(uint8_t output);

void SetOutputValue(uint8_t output, uint16_t value);

uint16_t GetOutputValue(uint8_t output);

void ServoTick(void);

#ifdef	__cplusplus
}
#endif

#endif	/* OUTPUTS_H */

