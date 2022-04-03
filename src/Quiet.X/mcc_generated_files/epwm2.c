/**
  EPWM2 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    epwm2.c

  @Summary
    This is the generated driver implementation file for the EPWM2 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This source file provides implementations for driver APIs for EPWM2.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.78
        Device            :  PIC18F46J53
        Driver Version    :  2.01
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.05 and above
         MPLAB 	          :  MPLAB X 5.20
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

/**
  Section: Included Files
*/

#include <xc.h>
#include "epwm2.h"

/**
  Section: Macro Declarations
*/

#define PWM2_INITIALIZE_DUTY_VALUE    499

/**
  Section: EPWM Module APIs
*/

void EPWM2_Initialize(void)
{
    // Set the EPWM2 to the options selected in the User Interface
	
	// CCP2M P2A,P2C: active high; P2B,P2D: active high; DC2B 0; P2M single; 
	CCP2CON = 0x0C;    
	
	// ECCP2ASE operating; PSS2BD low; PSS2AC low; ECCP2AS disabled; 
	ECCP2AS = 0x00;    
	
	// STRD P2D_to_port; STRC P2C_to_port; STRB P2B_to_port; STRA P2A_to_CCP2M; CMPL1 Steering Outputs depend on the values of STRD-A settings; STRSYNC start_at_next; 
	PSTR2CON = 0x11;    
	
	// P2RSEN automatic_restart; P2DC 0; 
	ECCP2DEL = 0x80;    
	
	// CCPR2H 0; 
	CCPR2H = 0x00;    
	
	// CCPR2L 0; 
	CCPR2L = 0x00;    

	// Selecting Timer2
	CCPTMRS0bits.C2TSEL = 0x0;
}

/**
void EPWM2_LoadDutyValue(uint16_t dutyValue)
{
   // Writing to 8 MSBs of pwm duty cycle in CCPRL register
    CCPR2L = ((dutyValue & 0x03FC)>>2);
    
   // Writing to 2 LSBs of pwm duty cycle in CCPCON register
    CCP2CON = ((uint8_t)(CCP2CON & 0xCF) | ((dutyValue & 0x0003)<<4));
}

End of File
*/

