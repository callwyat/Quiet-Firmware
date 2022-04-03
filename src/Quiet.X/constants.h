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

#ifdef	__cplusplus
}
#endif

#endif	/* CONSTANTS_H */

