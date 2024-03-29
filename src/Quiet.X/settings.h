/* 
 * File:   settings.h
 * Author: callwyat
 *
 * Created on November 8, 2021, 7:37 PM
 */

#ifndef SETTINGS_H
#define	SETTINGS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "outputs.h"
#include "uart.h"
#include "mcc_generated_files/i2c1_master.h"
#include "CLI/cli.h"
#include <stdint.h>

    typedef struct {
        uint16_t Value;
        OutputMode_e Mode;
    } OutputSettings_t;
    
#define DEFINE_OUTPUT_SETTING(value, mode) {    \
    .Value = value,                             \
    .Mode = mode,                               \
    }


    typedef struct {
        uint8_t Occupied;
        NumberFormat_e NumberFormat;
        char SerialNumber[16];
        uint8_t SPIBaud;
        uart_settings_t UARTSettings;
        i2c_settings_t I2CSettings;
        OutputSettings_t OutputSettings[10];
    } QuietSettings_t;


    QuietSettings_t GetSettings(void);
    
    void SetSettings(QuietSettings_t value);
    
    void SaveSettings(void);
    
    void RestoreSettings(bool factory);
    
#ifdef	__cplusplus
}
#endif

#endif	/* SETTINGS_H */

