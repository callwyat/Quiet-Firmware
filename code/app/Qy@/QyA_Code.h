/* 
 * File:   Setup.h
 * Author: Wyatt
 *
 * Created on May 21, 2018, 7:59 PM
 */

#define CoreConfigLocation 0xFFF8
#define BaseSettingsLocation 0xF900
#define SettingsLength 48

#define I2C_BufferSize 16

unsigned QyA_Command(unsigned char input);

void ReadSettingsDescription(void);

void SendFromFlash(unsigned int Address, unsigned int Length);


//Define the status byte bits
typedef union {
    struct 
    {          
        unsigned URC_DataAvailable  :1;
        unsigned URC_OverFill       :1;
        unsigned UTX_Empty          :1;
        unsigned UTX_Overfill       :1;
        
        unsigned SPI_Active         :1;     //Indicates a transfer is active
        unsigned SPI_Overfill       :1;
        unsigned I2C_Active         :1;
        unsigned I2C_Overfill       :1;
        //                           ^Defines one bit letting complier use Bit Operations
        //                            Higher numbers can be used.
        //          ^Defines the name of the Bit
        //  ^Unsigned means one bit
    };
    unsigned char All;
} q_Status ;

//Define an int that can be access in bytes
typedef union {
    struct 
    {       
        unsigned char Lower;
        unsigned char Higher;
    } ;
  unsigned int All;
} q_int ;

//Define a char that can be access by the bits
typedef union {
    struct 
    {       
        unsigned B0 :1;
        unsigned B1 :1;
        unsigned B2 :1;
        unsigned B3 :1;
        unsigned B4 :1;
        unsigned B5 :1;
        unsigned B6 :1;
        unsigned B7 :1;
    };
    unsigned char All;
} q_char ;

typedef union {
    struct 
    {       
        unsigned char In;
        unsigned char Out;
    };
} FIFO_Pointers;

typedef enum
{
    Null_Command = 0,
    ReadStatus = 1,
    WriteDigital = 2,
    ReadDigital = 3,
    WriteAnalog = 4,
    ReadAnalog = 5,
    WriteUSART = 6,
    ReadUSART = 7,
    WriteSPI = 8,
    ReadSPI = 9,
    WriteI2C = 10,
    ReadI2C = 11,
    Reserve12 = 12,
    Reserve13 = 13,
    WriteSettings = 14,
    ReadSettings = 15
} Command;