/*
 * File:   Setup.c
 * Author: Wyatt
 *
 * Created on May 21, 2018, 7:58 PM
 */

#include <xc.h>
#include "../../framework/usb/usb_device_cdc.h"     //Used just for the USB_OUTBuffer size
#include "Setup.h"
#include "Subroutines.h"
#include "QyA_Code.h"
#include "Interrupts.h"
#include "../main.h"

#include "Analog.h"
#include "Communications/qUSART.h"
#include "Communications/qSPI.h"
#include "Communications/qI2C.h"


    //General Memory
//Stores the settings
QyA_Settings Settings;
//Stores the Flash Location of the settings
unsigned int HardSettingsLocation;
//Defines the size of a "Large" data packet
#define LargeSize 24

//Declared in Main.C Stores the current position in the output_buffer
extern char OutPNT;

    //Read Status Byte Memory
q_Status Q_Status;

    //Write Digital Memory

    //Read Digital Memory

    //Write Analog/PWM/Servo Memory
//Stores the current output mode
OutModeDefinition OutputMode;
//Stores the value of the Servo Outputs
q_int Servo_Buffers[10];

    //Read Analog Memory
q_int Analog_Buffers[4];         //These values are filled in the Interrupt routine

    //Write USART Transmit Buffer

    //Read USART Receive Buffer

    //SPI
q_int SPI_Counter;

    //I2C Buffer
unsigned char I2C_Buff[I2C_BufferSize];
FIFO_Pointers I2C_PNTS;

unsigned QyA_Command(unsigned char input)
{   //Stores the currently Executing Command
    static Command CurrentCommand = Null_Command;
    //Stores the active argument of the command
    static q_char Argument;
    //Store the Number of times the current commands has been called
    static unsigned char CommandCount = 0;
    //Set to indicate the current command will take more then 24 bytes of Data
    unsigned LargeData = 0;
    //Switches won't let me declare vars inside them, so this helps with that.
    static unsigned char tempByte;
    
    //Memory for copying receive buffer data
    static unsigned char LengthPNT;    //Store the Length Location
    static q_int Length;               //Store the Length of New Data

    static unsigned char *WAPnt;     //Stores a pointer to were the data will go
    static unsigned ServoMode;       //Stores if the current value is servo data or PWM data
    static unsigned char HighBuffer; //Stores the MSB though loops
      
    //Increment the number of times the current command has be executed
    CommandCount++;
    //If the CommandCount gets out of hand, then something went wrong. Reset the Command
    if(CommandCount > 25 && !LargeData) CurrentCommand = 0;
    
    //Define the start of the command switch (Called from the null case)
    CommandStart:
    
    switch(CurrentCommand)
    {
        case Null_Command:
            //Establish what the new command is
            CurrentCommand = (input >> 4) & 0x0F;
            //Establish the current argument
            Argument.All = input & 0x0F;
            //Clear the command count
            CommandCount = 0;
            //Clear the Large Data bit
            LargeData = 0;
            //If the Null command was not given, then run the loop again for the
            //Argument to be processed
            if(CurrentCommand > 0) goto CommandStart;
            
            switch(Argument.All)
            {
                case 10:            //Clear Watchdog
                    asm("CLRWDT");
                    break;
                     
                case 11:            //Sleep Command?
                    asm("SLEEP");
                    break;
                    
                case 13:        //Run the Setup using the Settings in RAM
                case 14:        //Run the Setup using the Settings in Flash
                    Setup(!Argument.B0);   
                    break;

                case 15:            //Soft Reset
                    asm("RESET");
                    break;
            }
            
            Argument.All = 0;       //Clear the argument
            break;   
            
        case ReadStatus:  
            switch(Argument.All)       //Read Status
            {
                default:
                    //Read in the active bits
                    Q_Status.SPI_Active = SSP2STATbits.BF | DMACON1bits.DMAEN;
                    Q_Status.I2C_Active = SSP1STATbits.BF;
                    
                    //Send the status
                    Send(Q_Status.All);
                    break;
                
                case 13:    //Read User ID
                    for(unsigned char count = 0; count < 8; count++)
                    {
                        Send(Settings.UserID[count]);
                    }
                    break;
                    
                case 14:    //Read Serial Number
                    for(unsigned char count = 0; count < 8; count++)
                    {
                        Send(Settings.SN[count]);
                    }
                    break;
                    
                case 15:    //Read Firmware Revision                +++++++++++++Firmware Identification+++++++++++
                    Send('Q');
                    Send('y');
                    Send('@');
                    Send(' ');
                    Send('3');
                    Send('.');
                    Send('0');
                    Send('.');
                    Send('0');
                    Send('\n');
                    break;
            }

            CurrentCommand = Null_Command;
            break;
            
        case WriteDigital:                                              //Write Digital
            if(CommandCount == 0)
            {
                //Process the Argument
            }
            else
            {   //Round 2, grab the data byte and process it 
                //(Mask off any data that would conflict with a servo)
                DigitalOutput = input & (OutputMode.DServo ^ 0xFF);
                CurrentCommand = Null_Command;                
            }
            break;
            
        case ReadDigital:                                               //Read Digital
            if(CommandCount == 0)
            {   //Process the Argument
                Send(DigitalInput);
                CurrentCommand = Null_Command;
            }
            else
            {   //In case we get here...
                CurrentCommand = Null_Command;
            }
            break;
            
        case WriteAnalog:                                               //Write Analog
            
            switch (CommandCount)
            {   
                case 0:     //Process the address the user is targeting
                    if(Argument.All > 10)   //Test if the channel number is not valid
                    {   //Invalid channel set the pointer to 0
                        WAPnt = 0;
                        break;
                    }

                    //Test if accessing a Digital Output in Servo Mode
                    if(Argument.All > 1 && GetBit(OutputMode.DServo, (Argument.All - 2)))
                    {   //If yes, assign the pointer to a register in the Servo_Buffer
                        WAPnt = &Servo_Buffers[Argument.All].Lower;
                        ServoMode = 1;
                        break;
                    }
                    
                    //Test if using a valid PWM address
                    if(Argument.All < 2)    //Lower PWM numbers
                    {   
                        if(GetBit(OutputMode.AServo, Argument.All))
                        {
                            WAPnt = &Servo_Buffers[Argument.All].Lower;
                            ServoMode = 1;   
                        }
                        else
                        {
                        //Move the pointer to the upper byte
                        WAPnt = (unsigned char*)((unsigned int)&PWM1L - Argument.All * (unsigned int)(&PWM1L - &PWM2L));
                        }
                        break;
                    }
                    
                    if(Argument.All >= 6)   //Upper PWM Numbers
                    {   //Move the pointer to the first upper byte
                        WAPnt = (unsigned char*)((unsigned int)&PWM7L - (Argument.All - 6) * (unsigned int)(&PWM7L - &PWM8L));
                        break;
                    }
                    
                    //Invalid option, set the pointer to Zero to signal a error
                    WAPnt = 0;
                    break;
                case 1:     //Store the High Byte
                    HighBuffer = input;
                    break;
                    
                case 2:     //Store the Low Byte and apply new values
                    *WAPnt = ((input & 0xC0) >> 2) | (*WAPnt & 0xCF);   //Store the LSB
                    
                    if(ServoMode)
                    {   //If in servo mode, move the high byte
                        WAPnt++;
                    }
                    else
                    {   //If in PWM mode, move to the PWMxH register
                        WAPnt = WAPnt + (&PWM1H - &PWM1L);
                    }
                    
                    *WAPnt = HighBuffer;            //Store the MSB
                    
                    CurrentCommand = Null_Command;  //All Done with this command
                    break;
                    
                default:
                //In case we get here...
                CurrentCommand = Null_Command;
                    break;
            }
            break;
            
        case ReadAnalog:                                                //Read Analog
            if(CommandCount == 0)
            {   //Process the Argument
                char count;
                for(count = 0; count < 4; count++)
                {   //Rotate the Argument then check if the LSB was set
                    Argument.All = Argument.All >> 1;
                    if(STATUSbits.C == 1)       //if the LSB was set, then grab the analog data
                    {
                        Send(Analog_Buffers[count].Higher);
                        Send(Analog_Buffers[count].Lower);
                    }
                }

                CurrentCommand = Null_Command;
            }
            else
            {   //In case we get here...
                CurrentCommand = Null_Command;
            }
            break;

    case WriteUSART:
        if(CommandCount == 0)
        {
            //No Data to process on the first run though here
        }
        else
        {  
            //Write the data, check if the buffer is full
            USARTWrite(input, true);
            
            //Test if the last byte has been received
            if(CommandCount > Argument.All)
            {   //If so, then this command is done. 
                CurrentCommand = Null_Command;
            }
        }
        break;
        
    case ReadUSART:     //Read the USART Receive Buffer 

        //Get the amount of data that can be read
        Length.Lower = USARTDataAvalible();

        //Report the number of bytes that were read
        Send(Length.Lower);
        
        //While there is data to be read, send it.
        while (Length.Lower-- > 0)
        {
            USARTRead(&tempByte, 1);
            Send(tempByte);
        }
        
        CurrentCommand = Null_Command;
        
        break;
        
    //TODO: Maybe add a flag to send all 0xFF or 0x00 with B3 and B3 of Argument
    case WriteSPI:
        if(CommandCount < 2)
        {
            switch(CommandCount)
            {
                case 0:
                    SPI_Counter.Higher = Argument.All;  //Set the counter Higher
                    break;
                    
                default:
                    SPI_Counter.Lower = input;      //Set the counter Lower
                    
                    SPI_Counter.All++;          //0 argument == 1 byte
                    
                    CommandCount = 2;
                    break;
            }
        }
        else
        {   
            SPIWrite(input);
            
            //Test if all the data has been received
            if(SPI_Counter.All-- == 0) CurrentCommand = Null_Command;
        }
        
     break;
            
    case ReadSPI:
        //Get the amount of data that can be read
        Length.All = SPIDataAvalible();
        
        //Report the number of bytes that were read
        Send(Length.Higher);
        Send(Length.Lower);
        
        //While there is data to be read, send it.
        while (Length.All-- > 0)
        {
            SPIRead(&tempByte, 1);
            Send(tempByte);
        }
        
        CurrentCommand = Null_Command;
            
    case WriteI2C:
        break;
            
    case ReadI2C:
        break;
        
    case Reserve12:
        break;
            
    case Reserve13:
        break;         
            
    case WriteSettings:
        //If this is the first time though, then there isn't anything to do
        //Unless it's a commit command
        if(CommandCount <= 0 && (Argument.All < 13 || Argument.All == 0))  break;
        
        switch(Argument.All)
        {
            case 0:     //Store the current output values into the settings
                Settings.DigOut = DigitalOutput;
                
                //Save digital output settings
                Settings.OutMode = OutputMode;
                Settings.Servo[0] = (OutputMode.Servo1 ? Servo_Buffers[0].Higher : PWM1H);
                Settings.Servo[1] = (OutputMode.Servo2 ? Servo_Buffers[1].Higher : PWM2H);
                Settings.Servo[2] = Servo_Buffers[2].Higher;
                Settings.Servo[3] = Servo_Buffers[3].Higher;
                Settings.Servo[4] = Servo_Buffers[4].Higher;
                Settings.Servo[5] = Servo_Buffers[5].Higher;
                Settings.Servo[6] = (OutputMode.Servo7 ? Servo_Buffers[6].Higher : PWM7H);
                Settings.Servo[7] = (OutputMode.Servo8 ? Servo_Buffers[7].Higher : PWM8H);
                Settings.Servo[8] = (OutputMode.Servo9 ? Servo_Buffers[8].Higher : PWM9H);
                Settings.Servo[9] = (OutputMode.Servo10 ? Servo_Buffers[9].Higher : PWM10H);
                
                //Save USART Settings
                Settings.BAUDCON = BAUDCON;
                Settings.SPBRG.Higher = SPBRGH;
                Settings.SPBRG.Lower = SPBRG;
                Settings.TXSTA = TXSTA1;
                Settings.RCSTA = RCSTA2;
               
                break;
                
            case 1:    //Write the next 8 bytes to User ID
                if(CommandCount > 0) Settings.UserID[CommandCount - 1] = input;
                if(CommandCount >= 8) CurrentCommand = Null_Command;
                break;
                
            case 6:     //Write to a specific Settings Location
                //If the Length and Start Index have been filled jump to 
                //storing the data
                if(CommandCount > 2) goto Write_Settings;

                if(CommandCount <= 1)   //Save the Start Index
                {   
                    LengthPNT = input;
                }
                else                    //Save the Length of the Data Packet
                {   //Return if the user will send a zero length data packet?
                    if(input == 0) CurrentCommand = Null_Command;
                    
                    Length.Lower = input + 7;
                    //Suppress the Data Reset if needed
                    if(Length.Lower >= LargeSize + 7) LargeData = 1; 
                    CommandCount = 7;   //Jump to 8 (7 + 1) to account for USER ID
                }
                break;

            case 7:     //Write to all the Settings
                if(CommandCount <= 1)
                {
                    LengthPNT = 0;
                    Length.Lower = SettingsLength + 7;
                    LargeData = 1;      //Suppress the Data Reset
                    CommandCount = 7;   //Jump to 8 (7 + 1) to account for USER ID
                }
                
            Write_Settings:
                //If the user tries to access memory outside of the Settings
                //bounds, don't allow it.
                if(CommandCount + LengthPNT < SettingsLength)
                {
                    Settings.Byte[CommandCount + LengthPNT] = input;
                }
                
                //Wait for the users data packet to end
                if(CommandCount >= Length.Lower) CurrentCommand = Null_Command;
                break;
                
            case 13:    //Commit the new settings with Digital Output and Serial ack
            case 14:    //Commit the new settings with Serial ack
            case 15:    //Commit the new settings
                //Test if this is the first time the user is saving settings
                if(HardSettingsLocation < BaseSettingsLocation)
                {   //Yes, Load the base location for saving settings
                    HardSettingsLocation = BaseSettingsLocation;  
                } 
                //Test if the Settings block will overflow on this save
                else if((HardSettingsLocation + 64) >= (BaseSettingsLocation + 1024))
                {   //Erase the whole block
                    Flash_Erase(BaseSettingsLocation);
                    //Reset to the base settings location
                    HardSettingsLocation = BaseSettingsLocation;      
                }
                else
                {   //Otherwise increment to the next clear location
                    HardSettingsLocation += 64;
                }
                
                //Write the settings into flash
                if(Flash_Write(HardSettingsLocation, &Settings.Byte[0], 64))
                {   //Test if the user wanted an ack
                    if(Argument.All <= 13) DigitalOutput = 0xFF;
                    if(Argument.All <= 14) Send(0xFF);
                }
                else
                {
                    if(Argument.All <= 13) DigitalOutput = 0x81;
                    if(Argument.All <= 14) Send(0x00);
                }
                
                CurrentCommand = Null_Command;
                break;
                
            default:
                CurrentCommand = Null_Command;
                break;
        }
        break;
            
    case ReadSettings:
        //Test if the description of the setting is wanted
        if(Argument.All == 0)
        {
            ReadSettingsDescription();
        }
        else
        {
            if(!Argument.B3) //Test if a read all command
            {
                if(CommandCount <= 1) 
                {//Make sure the pointer stays in the array
                    LengthPNT = (input <= SettingsLength ? input : SettingsLength);
                    break;
                }
                else
                {   //Make sure there isn't an overflow
                    Length.Lower = (input <= SettingsLength - LengthPNT ? input : SettingsLength - LengthPNT);
                    //                    ^           This is a conditional statement              ^

                    LengthPNT += 8;  //Offset to account for User ID
                }      
            }
            else
            {
                if(Argument.All != 12)      //Check that this isn't the ALL command
                {
                    LengthPNT = 0;              //Set the to start of settings array
                    Length.Lower = SettingsLength;    //Set to read all of the settings
                    Argument.B3 = 0;            //Clear the read all command
                }
            }

            switch(Argument.All)
            {
                case 1:     //Read the settings from RAM
                    Read_RAM_Settings:
                    for(unsigned char count = 0; count < Length.Lower; count++)
                    {
                        Send(Settings.Byte[count + LengthPNT]);
                    }
                    break;
                case 2:     //Read the settings from Flash
                    if((HardSettingsLocation - 1) < BaseSettingsLocation) goto Read_RAM_Settings;

                    SendFromFlash((HardSettingsLocation + LengthPNT), Length.Lower);

                    OutPNT += Length.Lower;
                    break;

                case 12:     //Read all settings including Device ID and Serial Number
                             //Regardless of if Settings have been saved or not.
                    SendFromFlash(HardSettingsLocation, 64);

                    OutPNT += 64;
                    break;

                default:    //Send back a generic read value
                    Send(0xFF);
                    break;
            }

            //End of command
            CurrentCommand = Null_Command;
        }
        break;
        
    default:
        //This Should never happen...
        CurrentCommand = Null_Command;
        break;
    }
    
    //Return reporting no extra data to read
    return false;
}

void SendFromFlash(unsigned int Address, unsigned int Length)
{
    unsigned char readBuffer[64];
    unsigned char* buffPNT = &readBuffer[0];
    
    Flash_Read(Address, &readBuffer[0], Length);
    
    unsigned char count;
    for (count = 0; count < Length; count++)
        Send(*buffPNT++);
    
}

void ReadSettingsDescription(void)
{
    unsigned char readBuffer[64];
    unsigned char* buffPNT;
    
    unsigned End = 0;
    unsigned char FilePNT = 0;
    
    unsigned int count;
    while (!End)
    {
        Flash_Read((int)(&XMLDescription + FilePNT), &readBuffer[0], 64);  
        
        buffPNT = &readBuffer[0];
        
        for (count = 0; count < 64; count++)
        {
            if (*buffPNT++ == 0xFF)
            {
                End = 1;
                break;
            }
        }
    }
}


//If this routine is hit, that means that an error happened, which is bad.
void Q_Error(unsigned int ErrorCode)
{
    while(true);
    
}



