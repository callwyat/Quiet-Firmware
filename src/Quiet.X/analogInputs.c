
#include <stdbool.h>
#include <stdint.h>
#include <xc.h>

volatile uint16_t ADCCache[4];

const uint8_t adcCacheSize = sizeof(ADCCache) / sizeof(ADCCache[0]);

uint16_t GetADCValue(uint8_t channel)
{
    return channel < adcCacheSize ? ADCCache[channel] : (uint16_t)-1;
}

void StartADCUpdate(void)
{
    ADCON0bits.GO_nDONE = 1;
}

void ADCTick(void)
{
    static uint8_t step = 0;
    
    // Turn off the ADC for changing channels
    ADCON0bits.ADON = false;

    ADCCache[step] = (uint16_t)((ADRESH << 8) | (ADRESL));
    
    if (++step >= adcCacheSize)
    {
        step = 0;
    }
    
    switch (step)
    {
        case 0:
            ADCON0 = 0x09;
            break;
            
        case 1:
            ADCON0 = 0x0D;
            break;
            
        case 2:
            ADCON0 = 0x11;
            break;
            
        case 3:
            ADCON0 = 0x15;
            break;
        
        default:
            // Developer error: What is this channel you give me?
            while (true);
    }
    
    if (step != 0)
    {
        // Start the next conversion
        ADCON0bits.GODONE = true;
    }
}
