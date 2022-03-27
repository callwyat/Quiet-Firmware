/*
 * File:   settings.c
 * Author: callwyat
 *
 * Created on November 8, 2021, 7:37 PM
 */


#include <xc.h>
#include <stdint.h>
#include "settings.h"
#include "outputs.h"
#include "constants.h"
#include "CLI/cli.h"
#include "mcc_generated_files/memory.h"
#include "mcc_generated_files/i2c1_master.h"

#define FACTORY_SETTINGS {                                          \
    .Occupied = 1,                                                  \
    .NumberFormat = DecimalFormat,                                  \
    .SerialNumber = { '{', 'S', 'e', 'r', 'i', 'a', 'l', ' ',       \
    'N', 'u', 'm', 'b', 'e', 'r', '}', '\x00'},                     \
    .UARTBaud = 0x01A0,         /* 9600 */                          \
    .SPIBaud = 0x00,            /* 4 Mhz */                         \
    .I2CBaud = 0x27,            /* 100 kHz */                       \
    .I2CTimeout = 0x80,                                             \
    .OutputSettings = {                                             \
        DEFINE_OUTPUT_SETTING(0x000, OUT_DISCREET),                 \
        DEFINE_OUTPUT_SETTING(0x000, OUT_DISCREET),                 \
        DEFINE_OUTPUT_SETTING(0x000, OUT_DISCREET),                 \
        DEFINE_OUTPUT_SETTING(0x000, OUT_DISCREET),                 \
        DEFINE_OUTPUT_SETTING(0x000, OUT_DISCREET),                 \
        DEFINE_OUTPUT_SETTING(0x000, OUT_DISCREET),                 \
        DEFINE_OUTPUT_SETTING(0x000, OUT_DISCREET),                 \
        DEFINE_OUTPUT_SETTING(0x000, OUT_DISCREET),                 \
        DEFINE_OUTPUT_SETTING(0x0FF, OUT_PWM),                      \
        DEFINE_OUTPUT_SETTING(0x0FF, OUT_PWM),                      \
    }                                                               \
} 

const QuietSettings_t settings __at(0xF800) = FACTORY_SETTINGS;

QuietSettings_t GetSettings(){
    return settings;
}

void SetSettings(QuietSettings_t value){
    
    FLASH_WriteBlock((uint16_t)&settings, (uint8_t*)&value);
}

void SaveSettings()
{
    QuietSettings_t settings = GetSettings();

    for (uint8_t i = 0; i < OUTPUT_COUNT; ++i)
    {
        OutputSettings_t* outputSetting = &settings.OutputSettings[i];
        outputSetting->Mode = GetOutputMode(i);
        outputSetting->Value = GetOutputValue(i);
    }
    
    settings.NumberFormat = GetNumberFormat();

    settings.UARTBaud = (SPBRGH1 << 8) + SPBRG1;
    settings.SPIBaud = SSP2CON1bits.SSPM;
    settings.I2CBaud = SSP1ADD;
    settings.I2CTimeout = I2C1_GetTimeout();
    
    SetSettings(settings);
}

void RestoreSettings(bool factory)
{
    QuietSettings_t settings = FACTORY_SETTINGS;
    
    if (factory)
    {
        SetSettings(settings);
    }
    else
    {
        settings = GetSettings();
    }

    for (uint8_t i = 0; i < OUTPUT_COUNT; ++i)
    {
        OutputSettings_t outputSetting = settings.OutputSettings[i];
        SetOutputMode(i, outputSetting.Mode);
        SetOutputValue(i, outputSetting.Value);
    }
    
    SPBRG1 = (uint8_t)settings.UARTBaud;
    SPBRGH1 = settings.UARTBaud >> 8;

    SSP2CON1bits.SSPM = settings.SPIBaud;
    SSP1ADD = settings.I2CBaud;
    I2C1_SetTimeout(settings.I2CTimeout);

    SetNumberFormat(settings.NumberFormat);
}
