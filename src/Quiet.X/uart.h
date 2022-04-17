/* 
 * File:   uart.h
 * Author: callwyat
 *
 * Created on April 1, 2022, 4:13 PM
 */

#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
    
typedef enum {
    UMODE_USBUART,
    UMODE_SCPI,
    UMODE_MODBus
} UART_Modes_e;

typedef struct {
    uint16_t Baud;
    UART_Modes_e Mode;
} uart_settings_t;

bool UART_is_tx_ready(void);

uint16_t UART_get_period(void);

void UART_set_period(uint16_t rate);

uint24_t UART_get_baud_rate(void);

void UART_set_baud_rate(uint24_t rate);

UART_Modes_e UART_get_mode(void);

void UART_set_mode(UART_Modes_e mode);

uint8_t inline UART_get_rx_count(void);

uint8_t inline UART_Read(void);

void inline UART_Write(uint8_t txData);

unsigned UART_RxBufferOverflow(void);

void UART_ClearRxBufferOverflow(void);

#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

