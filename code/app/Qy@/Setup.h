/* 
 * File:   Setup.h
 * Author: Wyatt
 *
 * Created on May 21, 2018, 7:59 PM
 */

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

#define USBLED PORTEbits.RE1

#define true 1
#define false 0

#define DigitalOutput LATB
#define DigitalInput PORTD

#define PWM1H CCPR1L    //Analog Outputs
#define PWM1L CCP1CON
#define PWM2H CCPR2L
#define PWM2L CCP2CON

#define PWM3H CCPR3L    //Analog Outputs
#define PWM3L CCP3CON

#define PWM7H CCPR4L    //Analog Outputs
#define PWM7L CCP4CON
#define PWM8H CCPR5L
#define PWM8L CCP5CON
#define PWM9H CCPR6L    //Analog Outputs
#define PWM9L CCP6CON
#define PWM10H CCPR7L
#define PWM10L CCP7CON


#define   firstByte   flashRAM[0]


#define   outStreamByte   flashRAM[3]
#define   outStreamTimerH   flashRAM[4]
#define   outStreamTimerL   flashRAM[5]


#define   inStreamByte   flashRAM[8]
#define   inStreamTimerH   flashRAM[9]
#define   inStreamTimerL   flashRAM[10]


#define   analogInByte   flashRAM[13]


#define   analogOutByte   flashRAM[16]
#define   analogOut1   flashRAM[17]
#define   analogOut2   flashRAM[18]
#define   analogOut3   flashRAM[19]
#define   analogOut4   flashRAM[20]
#define   analogOut5   flashRAM[21]
#define   analogOut6   flashRAM[22]
#define   analogOut7   flashRAM[23]
#define   analogOut8   flashRAM[24]

#define   digitalOutByte0   flashRAM[26]
#define   digitalOutByte1   flashRAM[27]
#define   digitalOutByte2   flashRAM[28]


#define   USART1buadH   flashRAM[31]
#define   USART1buadL   flashRAM[32]


#define   USART2buadH   flashRAM[35]
#define   USART2buadL   flashRAM[36]
#define   USART2RCpin   flashRAM[37]
#define   USART2TXpin   flashRAM[38]


#define   I2Cset   flashRAM[41]
#define   I2Cbaud   flashRAM[42]


#define   SPIBuadH   flashRAM[45]
#define   SPIBuadL   flashRAM[46]


void Setup(unsigned ReadHardSettings);

void SetupPort(unsigned char *PortPnt, unsigned char Value, unsigned char TriState);

void FirstRun(void);



