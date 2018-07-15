/* 
 * File:   Setup.h
 * Author: Wyatt
 *
 * Created on May 21, 2018, 7:59 PM
 */

#define SPI_BufferSize 512

typedef union {
    struct 
    {          
        unsigned Active             :1;     //Indicates a transfer is active
        unsigned Used               :1;     //Indicates the module has been used
        unsigned                    :1;
        unsigned                    :1;

        unsigned DMALoad            :1;     //Indicates DMA space is being loaded
        unsigned DMAStart           :1;
        unsigned DMAActive          :1;
        unsigned DMARollover        :1;     //Indicates the buffer data buffer rolled over
    };
    unsigned char All;
} eSPISTAT ;

typedef union {
    struct 
    {       
        unsigned int Load;      //Location of the next byte to put into the buffer
        q_int        DMALoad;   //Location where the DMA transfer will start
        unsigned int Resp;      //Location to store the next response into the buffer
        unsigned int Unload;    //Location of next byte to send to User
    };
    
} SPI_Pointers;

void SPISetup(void);

void SPIWrite(unsigned char data);

unsigned int SPIDataAvalible(void);

unsigned int SPIRead(unsigned char* BuffPNT, unsigned int Length);

void SPIINT(void);