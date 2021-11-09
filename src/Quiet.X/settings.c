/*
 * File:   settings.c
 * Author: callwyat
 *
 * Created on November 8, 2021, 7:37 PM
 */


#include <xc.h>
#include "settings.h"
#include "outputs.h"
#include "constants.h"
#include "mcc_generated_files/memory.h"

const QuietSettings_t settings __at(0xFF80) = {
    .Occupied = 1,
    .SerialNumber = { '{', 'S', 'e', 'r', 'i', 'a', 'l', ' ', 
    'N', 'u', 'm', 'b', 'e', 'r', '}', '\x00'},
    .OutputSettings = {
        DEFINE_OUTPUT_SETTING(0x0FF, OUT_PWM),       
        DEFINE_OUTPUT_SETTING(0x0FF, OUT_PWM),
        DEFINE_OUTPUT_SETTING(0x000, OUT_DISCREET),
        DEFINE_OUTPUT_SETTING(0x000, OUT_DISCREET),
        DEFINE_OUTPUT_SETTING(0x000, OUT_DISCREET),
        DEFINE_OUTPUT_SETTING(0x000, OUT_DISCREET),
        DEFINE_OUTPUT_SETTING(0x000, OUT_DISCREET),
        DEFINE_OUTPUT_SETTING(0x000, OUT_DISCREET),
        DEFINE_OUTPUT_SETTING(0x000, OUT_DISCREET),
        DEFINE_OUTPUT_SETTING(0x000, OUT_DISCREET),
    }
};

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
    
    SetSettings(settings);
}

void RestoreSettings()
{
    QuietSettings_t settings = GetSettings();

    for (uint8_t i = 0; i < OUTPUT_COUNT; ++i)
    {
        OutputSettings_t outputSetting = settings.OutputSettings[i];
        SetOutputMode(i, outputSetting.Mode);
        SetOutputValue(i, outputSetting.Value);
    }
}
