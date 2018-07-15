

#include <xc.h>
#include "Subroutines.h"
#include "QyA_Code.h"


#define FlashMemory 0x00F9FF

void Flash_Read(unsigned int Address, unsigned char* DataPnt, unsigned int Length)
{
    q_int address;
    address.All = Address;
    TBLPTRL = address.Lower;     //Write the Flash Memory Location
    TBLPTRH = address.Higher;
    TBLPTRU = 0x00;     
    
    for(unsigned int count = 0; count < Length; count++)
    {
        asm("TBLRD*+");    //Load the Low Byte into the TABLAT

        *DataPnt = TABLAT;      //Save Byte
        DataPnt++;
    }
    
    return;
}

/*This routine will write into Program Memory Space. (Data will need to be erased
 * before this operation will work.*/
unsigned Flash_Write(unsigned int Address, unsigned char* DataPnt, unsigned int Length)
{ 
    q_int address;
    address.All = Address & 0xFFC0;
    TBLPTRL = address.Lower;     //Write the Flash Memory Location
    TBLPTRH = address.Higher;
    TBLPTRU = 0x00;     
    
    //Copy the lower part of the block into the Table.
    while(address.All < Address)
    {
        asm("TBLRD*");    //Load the Low Byte into the TABLAT
        asm("TBLWT*+");
        address.All++;
    }
        
    unsigned char tableCounter = 0;
    unsigned result = 1;        //Indicates if the write was successful
    for(unsigned char count = 0; count < Length; count++)
    {
        TABLAT = *DataPnt;      
        asm("TBLWT*+");    //Load TABLAT into the Holding Register
        DataPnt++;
                
        tableCounter++;
        if(tableCounter >= 64)
        {
            unsigned char tempByte = TBLPTRL;
            TBLPTRL = (TBLPTRL - 1) & 0xC0;
            
            result &= _flashWrite();
            tableCounter = 0;   //Reset the Table Counter
            
            TBLPTRL = tempByte;
        }
    }
    
    if(tableCounter > 0) result &= _flashWrite();
    return result;
}

unsigned _flashWrite()
{       
    EECON1bits.WREN = true;     //Setup the write
    INTCONbits.GIEH = false;
    INTCONbits.GIEL = false;
    asm("CLRWDT");
    
    EECON2 = 0x55;              //Pump Charge!!
    EECON2 = 0xAA;
    EECON1bits.WR = true;       //Start Write
    
    INTCONbits.GIEH = true;
    INTCONbits.GIEL = true;
    EECON1bits.WREN = false;
            
    return !EECON1bits.WRERR;
}  

unsigned Flash_Erase(unsigned int Address)
{
    q_int address;
    address.All = Address;
    TBLPTRL = address.Lower;     //Write the Flash Memory Location
    TBLPTRH = address.Higher;
    TBLPTRU = 0x00;     
    
    EECON1bits.WREN = true;     //Prepare to erase the memory
    EECON1bits.FREE = true;
    INTCONbits.GIEH = false;
    INTCONbits.GIEL = false;
    asm("CLRWDT");
    
    EECON2 = 0x55;              //Pump Charge
    EECON2 = 0xAA;
    EECON1bits.WR = true;       //Enable the Erase
    
    INTCONbits.GIEH = true;      //Re-enable interrupts
    INTCONbits.GIEL = true;
   
    return 1;
}