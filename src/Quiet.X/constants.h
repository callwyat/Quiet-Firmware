/* 
 * File:   constants.h
 * Author: callwyat
 *
 * Created on October 17, 2021, 9:51 PM
 */

#ifndef CONSTANTS_H
#define	CONSTANTS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <xc.h>

extern const char* MakeString;
extern const char* ModelString;
extern const char* VersionString;

extern const char* EmptyIEEEHeader;

#define DIN PORTD
#define DOUT LATB
#define DOUTTRIS TRISB

#define USBLED LATEbits.LATE1

#define SPICS LATEbits.LATE2
#define SDDetect PORTEbits.RE2
#define SDDetectEnable TRISEbits.TRISE2

#define OUTPUT_COUNT 10

// System Errors
#define ERROR_CODE_NO_ERROR                 0x0000
#define ERROR_CODE_ERROR_BUFFER_OVERFLOW    0x0001

// CLI Errors
#define CLI_ERROR_INVALID_COMMAND           0x0101
#define CLI_ERROR_INVALID_BRANCH            0x0102
#define CLI_ERROR_INVALID_NUMBER            0x0110
#define CLI_ERROR_INVALID_IEEE_HEADER       0x0111

// DIGI Errors
#define DIGI_ERROR_INVALID_CHANNEL          0x0201

// ANAI Errors
#define ANAI_ERROR_INVALID_CHANNEL          0x0301

// OUTPUT Errors
#define OUTPUT_ERROR_NO_ERROR               0x00
#define OUTPUT_ERROR_INVALID_CHANNEL        0x01
#define OUPTUT_ERROR_INVALID_MODE_SELECTION 0x02
#define OUTPUT_ERROR_INVALID_VALUE          0x03

// DIGO Errors
#define DIGO_ERROR_GROUP                    0x04

// ANAO Errors
#define ANAO_ERROR_GROUP                    0x05

// PWM Errors
#define PWM_ERROR_GROUP                     0x06

// SERV Errors
#define SERV_ERROR_GROUP                    0x07

// UART Errors
#define UART_ERROR_INVALID_BAUD             0x0910
#define UART_ERROR_INVALID_MODE             0x0911
#define UART_ERROR_INVALID_WRITE            0x0912

#define UART_ERROR_RECEIVE_OVERFLOW         0x0920

#define UART_ERROR_WRITE_MODE_ERROR         0x0930
#define UART_ERROR_READ_MODE_ERROR          0x0931

// SPI Errors

// IIC Errors
#define I2C_ERROR_NONE                      0x0B00
#define I2C_ERROR_INVALID_BAUD              0x0B01
#define I2C_ERROR_INVALID_TIMEOUT           0x0B02
#define I2C_ERROR_INVALID_SLAVE_ADDRESS     0x0B03
#define I2C_ERROR_INVALID_MODE              0x0B04

#define I2C_ERROR_DISABLED_WRITE            0x0B10
#define I2C_ERROR_DISABLED_READ             0x0B11
#define I2C_ERROR_NO_ACKNOWLEDGE            0x0B12

#define I2C_ERROR_INVALID_RSIZE             0x0B20
#define I2C_ERROR_INVALID_REGISTER_ADDRESS  0x0B21
#define I2C_ERROR_INVALID_REGISTER_VALUE    0x0B22

#define I2C_ERROR_BUFFER_OVERFLOW           0x0B30
#define I2C_ERROR_INVALID_WRITE_SIZE        0x0B31
#define I2C_ERROR_INVALID_READ_SIZE         0x0B32

#ifdef	__cplusplus
}
#endif

#endif	/* CONSTANTS_H */

