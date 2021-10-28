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

const char* MakeString = "Y@ Technologies";
const char* ModelString = "Qy@ Board";
const char* SerialNumberString = "{Serial Number}";
const char* VersionString = "2.0";


#define DIN PORTD
#define DOUT LATB


#ifdef	__cplusplus
}
#endif

#endif	/* CONSTANTS_H */
