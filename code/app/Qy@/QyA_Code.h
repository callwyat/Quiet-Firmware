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


//Define a type that contains the PWM and Servo Mode of the device
typedef union {
    struct 
    {       
        unsigned PWM7    :1;
        unsigned PWM8    :1;
        unsigned PWM9    :1;
        unsigned PWM10   :1;
        unsigned         :2;
        unsigned Servo1  :1;
        unsigned Servo2  :1;
        unsigned Servo3  :1;
        unsigned Servo4  :1;
        unsigned Servo5  :1;
        unsigned Servo6  :1;
        unsigned Servo7  :1;
        unsigned Servo8  :1;
        unsigned Servo9  :1;
        unsigned Servo10 :1;
    };

    struct 
    {       
        unsigned PWMS    :4;
        unsigned         :2;
        unsigned AServo  :2;    //Analog Output Servo Pins
        unsigned DServo  :8;    //Digital Output Servo Pins
    } ;

    struct 
    {       
        unsigned char Lower;
        unsigned char Higher;
    } ;

    unsigned int All;
} OutModeDefinition ;

typedef union {
    struct 
    {       
        unsigned char In;
        unsigned char Out;
    };
} FIFO_Pointers;


//Define the status byte bits
typedef union {
    struct 
    {   /*These bits indicate what communication channels will accept q_Commands
         * on boot up. After one channel is used, the rest are disabled*/
        unsigned USB_Enable     :1;
        unsigned USART_Slave    :1;
        unsigned SPI_Slave      :1;
        unsigned I2C_Slave      :1;
        
        unsigned                :1;
        
        unsigned USB_NC         :1;     /*USB Not Connected. Reset the board if 
                                        *USB connection is lost*/   
        unsigned Idle_Reset     :1;     /*Flag to reset the board if comms go 
                                         *idle for to long*/
        unsigned Watchdog       :1;     //Flag to enable the watchdog timer
    };
    unsigned char All;
} q_Boot ;

typedef union{
    struct
    {
        unsigned char Byte[64];
    };
    
    struct
    {
        unsigned char UserID[8];                  //0     Null
//        unsigned char UserID;                   //1
//        unsigned char UserID;                   //2
//        unsigned char UserID;                   //3
//        unsigned char UserID;                   //4    Read Status
//        unsigned char UserID;                   //5
//        unsigned char UserID;                   //6
//        unsigned char UserID;                   //7
        unsigned char DigOut;                //8    Digital Outputs
        unsigned char DigIn;                 //9
        OutModeDefinition OutMode;          //10
                                            //11
        unsigned char Servo[10];            //12    Digital Inputs
//        unsigned char Servo;              //13
//        unsigned char Servo;              //14
//        unsigned char Servo;              //15
//        unsigned char Servo;              //16    Analog Outputs
//        unsigned char Servo;              //17
//        unsigned char Servo;              //18
//        unsigned char Servo;              //19
//        unsigned char Servo;              //20    Analog Inputs  
//        unsigned char Servo;              //21
        unsigned char;                  //22
        unsigned char;                  //23
        unsigned char TXSTA;                 //24   USART Write
        unsigned char RCSTA;                 //25
        unsigned char BAUDCON;               //26
        q_int SPBRG;                         //27
        //unsigned char SPBRGH;              //28   USART Read
        unsigned char;                  //29
        unsigned char;                  //30
        unsigned char ;                 //31
        unsigned char SPICON1;                  //32   SPI Write
        unsigned char SPICON2;                  //33
        unsigned char;                  //34
        unsigned char;                  //35
        unsigned char;                  //36   SPI Read Buffer
        unsigned char;                  //37
        unsigned char;                  //38
        unsigned char;                  //39
        unsigned char I2CCON1;                  //40   I2C Write
        unsigned char I2CCON2;                  //41
        unsigned char I2CADR; /*or baud*/       //42    
        unsigned char;                  //43
        unsigned char;                  //44   I2C Read Buffer
        unsigned char;                  //45
        unsigned char;                  //46
        unsigned char;                  //47
        unsigned char;                  //48   Reserved 12
        unsigned char;                  //49
        unsigned char;                  //50
        unsigned char;                  //51
        unsigned char;                  //52   Reserved 13
        unsigned char;                  //53
        unsigned char;                  //54
        q_Boot BOOT;                  //55
        unsigned char SN[8];                  //56    Settings Write 
//        unsigned char SN;                  //57
//        unsigned char SN;                  //58
//        unsigned char SN;                  //59
//        unsigned char SN;                  //60    Settings Read
//        unsigned char SN;                  //61
//        unsigned char SN;                  //62
//        unsigned char SN;                  //63    
    };
} QyA_Settings;

//TAB-Definition Definition
/*  The concept behind tab definition to make an easy to read grouping of
 *  settings.
 * 
 * The top level, or the levels with no tabs, defines the highest grouping, then
 * anything indented indented from there can further describe the group.
 * 
 * For example the first group is UserID=C8 so the first groups name is UserID
 * and the data is contained in 8 Chars.
 * 
 * A deeper example is Discrete=B16. This group is described by 16 Bytes. Inside
 * this group there is more detail. MODEL=B1 defines the 3rd Byte of the Discrete 
 * group as the Low byte of the Mode, and this byte is further defined with its
 * bits such as PWM7=b1
 * 
 * 
 * Current Data Type definitions
 * 
 * C = Char
 * B = Byte
 * b = bit
 * I = Integer (Big Ended)
 * 
 * [ ] = Array (e.g. [B8] is an array of eight bytes
 */


const char *XMLDescription = "\
TAB-Definition\n\
UserID=C8\n\
Discrete=B16\n\
\tDOUT=B1\n\
\tDIN=B1\n\
\tMODEL=B1\n\
\t\tPWM7=b1\n\
\t\tPWM8=b1\n\
\t\tPWM9=b1\n\
\t\tPWM10=b1\n\
\t\t-=b1\n\
\t\t-=b1\n\
\t\tServo1=b1\n\
\t\tServo2=b1\n\
\tMODEH=B1\n\
\t\tServo3=b1\n\
\t\tServo4=b1\n\
\t\tServo5=b1\n\
\t\tServo6=b1\n\
\t\tServo7=b1\n\
\t\tServo8=b1\n\
\t\tServo9=b1\n\
\t\tServo10=b1\n\
\tPWMBOOT=B10\
USART=B8\n\
\tTXSTA=B1\n\
\tRCSTA=B1\n\
\tBAUDCON=B1\n\
\tSPBRG=I1\n\
SPI=B8\n\
\tSPICON1=B1\n\
\tSPICON2=B1\n\
I2C=15\n\
\tI2CCON1=B1\n\
\tI2CCON2=B1\n\
\tI2CADR=B1\n\
BOOT=B1\n\
\tBOOT Settings=B1\n\
\t\tUSB Enable=b1\n\
\t\tUSART Slave=b1\n\
\t\tSPI Slave=b1\n\
\t\tI2C Slave=b1\n\
\t\t-=b1\n\
\t\tUSB Reset=b1\n\
\t\tIdle Reset=b1\n\
\t\tWatchdog=b1\n\
\
\
\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\
\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\
\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\
\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\
\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\
\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\
\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\
\xFF\xFF\xFF\xFF\xFF\xFF\xFF";

//TODO: Figure out how to start this constant string at Address 0xFFC0

/*Press "F" to pay respects. Not sure what is going on here, but the flash read
 routine was only reading 0xFF past the address 0xFF80 in flash, so this insures
 that there should be F's there.*/


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