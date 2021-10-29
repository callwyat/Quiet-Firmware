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

const char* DISCREETWord = "DISC";
const char* PWMWord = "PWM";
const char* ServoWord = "SERV";

typedef enum {
    OUT_DISCREET = 1,
    OUT_PWM      = 2,
    OUT_SERVO    = 4,
} OutputMode_e;

OutputMode_e GetOutputMode(uint8_t channel);

void SetOutputMode(uint8_t output, OutputMode_e mode);

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

