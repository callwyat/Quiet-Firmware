/* 
 * File:   Setup.h
 * Author: Wyatt
 *
 * Created on May 21, 2018, 7:59 PM
 */

#define true 1
#define false 0

typedef enum
{
    Default = 0,
    _137High = 1,
    _137Transition = 2,
    _248High = 3,
    _248Transition = 4,
    _59High = 5,
    _59Transistion = 6,
    _60High = 7,
    _60Transistion = 8,
    Idle = 9
} ServoState;

void ServosSetup(void);

unsigned ServoSetup(unsigned char Channel);

void UpdateServo(void);
