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
#include "uart.h"
#include "constants.h"
#include "CLI/cli.h"
#include "mcc_generated_files/memory.h"
#include "mcc_generated_files/i2c1_master.h"

#define FACTORY_SETTINGS {                                          \
    .Occupied = 1,                                                  \
    .NumberFormat = DecimalFormat,                                  \
    .SerialNumber = { '{', 'S', 'e', 'r', 'i', 'a', 'l', ' ',       \
    'N', 'u', 'm', 'b', 'e', 'r', '}', '\x00'},                     \
    .UARTSettings = {                                               \
        .Baud = 0x01A0,         /* 9600 */                          \
        .Mode = UMODE_USBUART                                             \
    },                                                              \
    .SPIBaud = 0x00,            /* 4 Mhz */                         \
    .I2CSettings = {                                                \
        .Enabled = false,       /* Disabled */                      \
        .Baud = 0x27,           /* 100 kHz */                       \
        .Timeout = 0x80,        /* Unsure */                        \
    },                                                              \
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

    settings.UARTSettings.Baud = UART_get_period();    
    settings.UARTSettings.Mode = UART_get_mode();
    
    settings.SPIBaud = SSP2CON1bits.SSPM;
    settings.I2CSettings = I2C1_GetSettings();
    
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
    
    UART_set_period(settings.UARTSettings.Baud);    
    UART_set_mode(settings.UARTSettings.Mode);

    SSP2CON1bits.SSPM = settings.SPIBaud;
    I2C1_SetSettings(settings.I2CSettings);

    SetNumberFormat(settings.NumberFormat);
}
