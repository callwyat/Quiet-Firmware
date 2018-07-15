/* 
 * File:   Subroutines.h
 * Author: Wyatt
 *
 * Created on May 21, 2018, 9:13 PM
 */

#define true 1
#define false 0

#define GetBit(var, pos) (var & (1<<pos))

void Flash_Read(unsigned int Address, unsigned char* DataPnt, unsigned int Length);

unsigned Flash_Write(unsigned int Address, unsigned char* DataPnt, unsigned int Length);

unsigned _flashWrite();

unsigned Flash_Erase(unsigned int Address);

