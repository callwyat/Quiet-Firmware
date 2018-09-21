/*
 * File:   Settings.c
 * Author: Wyatt
 *
 * Created on September 20, 2018, 8:43 PM
 */


#include <xc.h>
#include "../main.h"
#include "Settings.h"
#include "QyA_Code.h"
#include "Subroutines.h"

void ReadSettingsDescription(void)
{
    unsigned char readBuffer[64];
    unsigned char* buffPNT;
    
    unsigned End = 0;
    unsigned int FilePNT = 0;
    
    unsigned int count;
    while (!End)
    {
        //Since XMLDescription is a pointer the & is not needed
        Flash_Read((int)(XMLDescription + FilePNT), &readBuffer[0], 64);  
        
        buffPNT = &readBuffer[0];
        
        for (count = 0; count < 64; count++)
        {
            Send(*buffPNT);
            if (*buffPNT++ == 0xFF)
            {
                End = 1;
                break;
            }
        }
        
        FilePNT += 64;
    }
}