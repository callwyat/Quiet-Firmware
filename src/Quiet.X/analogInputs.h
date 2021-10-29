/* 
 * File:   analogInputs.h
 * Author: callwyat
 *
 * Created on October 28, 2021, 8:07 PM
 */

#ifndef ANALOGINPUTS_H
#define	ANALOGINPUTS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
    
    uint16_t GetADCValue(uint8_t channel);

    void StartADCUpdate(void);
    
    void ADCTick(void);


#ifdef	__cplusplus
}
#endif

#endif	/* ANALOGINPUTS_H */

