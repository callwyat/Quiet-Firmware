/*
 * File:   Setup.c
 * Author: Wyatt
 *
 * Created on May 21, 2018, 7:58 PM
 */

#include <xc.h>
#include "../Setup.h"
#include "../QyA_Code.h"
#include "qSPI.h"

void SPISetup(void)
{
    
}

    //SPI Buffer
unsigned char SPI_Buff[SPI_BufferSize];
SPI_Pointers SPI_PNTS;      //Stores the location of the last read
eSPISTAT SPISTAT;

enum
{
    DMAComplete     = 0,    //All DMA transfers are completed
    Idle            = 1,    //DEFAULT STATE //No transactions are active
    Single          = 2,    //Sending data one byte at a time
    DMABuffering    = 3,    //Data is being buffered for DMA Transfer
    DMAActive       = 4,    /*Data is being transmitted via DMA. (Data can be
                            buffering while DMA is active)*/
} SPIState = Idle;

//Reports the amount of data that is ready to be read
unsigned int SPIDataAvalible(void)
{   //Test if there is more data to be had, and if it has all been received
    if(SPIState < Single && SSP2STATbits.BF) 
    {   //Read in the last byte of data
        SPI_Buff[SPI_PNTS.Resp++] = SSP2BUF;
        SPI_PNTS.Resp &= SPI_BufferSize - 1;    //Handle rollover
        
        SPIState = Idle;        //Mark that all transfers are completed
    }
    
    //Subtract the Response pointer from the Unload pointer to see how much data
    //can be read. Place into a signed int for ease of a later checked
    int count = SPI_PNTS.Resp - SPI_PNTS.Unload;
    
    //If there was a rollover, account for it.
    return (count < 0 ? count + SPI_BufferSize : count);
}

//  Summary:    Copies data from the SPI Buffer to the Users Buffer
//  BuffPNT:    Pointer to the buffer that the user wants filled
//  Length:     The max amount of data the user can accept
//  Returns:    Amount of data that was read
unsigned int SPIRead(unsigned char* BuffPNT, unsigned int Length)
{   //Get the amount of data that can be read from the SPI Buffer
    unsigned int rLen = SPIDataAvalible();
    
    if(rLen > Length)
    {
        rLen = Length;
    }
    
    int count;
    for(count = 0; count < rLen; count++)
    {   //Copy the data to the users buffer
        *BuffPNT = SPI_Buff[SPI_PNTS.Unload++];
        //Limit the pointer
        SPI_PNTS.Unload &= SPI_BufferSize - 1;
        //Move the users pointer to the next location
        BuffPNT++;
    }  
    
    //Return the number of bytes that were copied
    return  rLen;
}

void SPIWrite(unsigned char data)
{          
    //Test if we are in the single or idle mode
    if(SPIState < DMABuffering && SSP2STATbits.BF)     
    {   //Test if moving from Idle to Single
        if(SPIState == Single) 
        {   //Grab the read from the last write
            SPI_Buff[SPI_PNTS.Resp++] = SSP2BUF;
        }      

        SSP2BUF = data;        //Write the new data
        SPI_PNTS.Load++;        //Increment the send pointer
        
        SPI_PNTS.Load &= SPI_BufferSize - 1;      //Limit the SPI PNTs
        SPI_PNTS.Resp &= SPI_BufferSize - 1;      //Limit the SPI PNTs
        
        SPIState = Single;      //Mark that Single Mode is active
    }
    else
    {   
        if(SPIState == Single)
        {   

            //Setup the pointers
            q_int SendCount;
            SendCount.All = (unsigned int)&SPI_Buff[SPI_PNTS.Load];
            
            //Update to the buffering state
            SPIState = DMABuffering;
        }
        
        //Store the new data
        SPI_Buff[SPI_PNTS.Load++] = data;

        //Limit the pointer and mark if a rollover happened
        if(SPI_PNTS.Load == SPI_BufferSize)
        {
            SPI_PNTS.Load = 0;
            SPISTAT.DMARollover = 1;
        }

        //Enable the interrupts, DMA will start after the current transfer is done
        PIE3bits.SSP2IE = 1;    
    }
}

void SPIINT(void)
{
    //Test if this is the first run though since switching to DMA Mode
    if(SPIState <= DMABuffering)
    {   //Take care of the byte that was last sent as one byte
        SPI_Buff[SPI_PNTS.Resp++] = SSP2BUF;
        SPI_PNTS.Resp &= SPI_BufferSize - 1;
        goto LoadDMA;
    }
    else
    {   //Triggered because a DMA transfer completed. Update the response pointer
        SPI_PNTS.Resp = SPI_PNTS.DMALoad.All;
    }
    
    //Test if the pointers are equal, meaning all transfers are complete
    if(SPI_PNTS.DMALoad.All != SPI_PNTS.Load)
    {   //Nope there is more data to send
        
        LoadDMA:
        //Load the transfer pointers
        TXADDRH = SPI_PNTS.DMALoad.Higher;
        RXADDRH = TXADDRH;
        TXADDRL = SPI_PNTS.DMALoad.Lower;
        RXADDRL = TXADDRL;
        
        //Calculate how many bytes need to be sent
        q_int sendCount;
        sendCount.All = (SPISTAT.DMARollover ? SPI_BufferSize : SPI_PNTS.Load);
        sendCount.All -= SPI_PNTS.DMALoad.All;
               
        //Clear the rollover mark
        SPISTAT.DMARollover = 0;

        //Load the Count
        DMABCH = sendCount.Higher;
        DMABCL = sendCount.Lower;

        //Start the DMA transfer
        DMACON1bits.DMAEN = 1;

        //Increment the load location
        SPI_PNTS.DMALoad.All += sendCount.All;
        SPI_PNTS.DMALoad.All &= SPI_BufferSize - 1;
        
        SPIState = DMAActive;   //Mark that the DMA Module is active
    }
    else
    {   //All Transfers are complete
        SPIState = DMAComplete;
    }
}


