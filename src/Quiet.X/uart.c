
#include <xc.h>
#include <stdint.h>
#include "uart.h"
#include "mcc_generated_files/device_config.h"

#include "mcc_generated_files/eusart1.h"
#include "mcc_generated_files/eusart2.h"


UART_Modes_e ActiveMode = UMODE_USBUART;

void UART_set_period(uint16_t period)
{
    uint8_t storage = TXSTA1;
    TXSTA1 = 0x00;
    SPBRG1 = (uint8_t)period & 0xFF;
    SPBRGH1 = (uint8_t)(period >> 8);
    TXSTA1 = storage;
    
    period *= 3;
    
    storage = RCSTA2;
    RCSTA2 = 0x00;
    SPBRG2 = (uint8_t)period & 0xFF;
    SPBRGH2 = (uint8_t)(period >> 8);
    RCSTA2 = storage;
}

void UART_set_baud_rate(uint24_t rate)
{
    uint16_t period = (uint16_t)(_XTAL_FREQ / (4 * ((uint24_t)rate + 1)));
    
    UART_set_period(period);
}

uint16_t UART_get_period(void)
{
    return (uint16_t)((SPBRGH1 << 8) + SPBRG1);
}

uint24_t UART_get_baud_rate(void)
{
    uint24_t period = UART_get_period();
    
    return (_XTAL_FREQ / (4 * period)) - 1;   
}

UART_Modes_e UART_get_mode(void)
{
    return ActiveMode;
}

void UART_set_mode(UART_Modes_e mode)
{
    ActiveMode = mode;
}

uint8_t inline UART_get_rx_count(void)
{
    return EUSART2_get_rx_count();
}

uint8_t inline UART_Read(void)
{
    return EUSART2_Read();
}

void inline UART_Write(uint8_t txData)
{
    EUSART1_Write(txData);
}

