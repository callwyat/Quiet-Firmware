///*
// * File:   Setup.c
// * Author: Wyatt
// *
// * Created on May 21, 2018, 7:58 PM
// */
//
//#include <xc.h>
//
//
//
//
//unsigned char SD_Rout(void)
//{
//    while(!SSP2STATbits.BF);
//    
//    PORTEbits.RE2 = false;
//    
//    SD_temp = 0;
//    
//    while(SD_temp < 6)          //Send the Command
//    {
//        SSP2BUF = SD_COM[SD_temp++];
//        while(!SSP2STATbits.BF);
//    }
//    
//    SSP2BUF = 0xFF;  
//    
//    SD_stat = 0x20;       //Set the Pre-Response bit
//    
//    SD_temp = 0;
// 
//    while(CHECK_BIT(SD_stat,5) | SSP2BUF != 0xFF)
//    { 
//    while(!SSP2STATbits.BF);
//    
//    if(SSP2BUF == 0xFF)
//    {
//        SSP2BUF = 0xFF;
//        continue;      
//    }
//
//    SD_resp[SD_temp++] = SSP2BUF;
//    SSP2BUF = 0xFF;
//    
//    SD_stat = SD_stat - 0x20;
//    
//    }
//
//    switch  (SD_COM[0])
//    {
//        case    0x51:       //Read Sector from SD Card
//
//            while(!SSP2STATbits.BF);
//
//            do{
//                SSP2BUF = 0xFF;
//                while(!SSP2STATbits.BF);
//
//            }while(SSP2BUF == 0xFF);  //Wait for the Data token or an error to be received
//
//            if(SSP2BUF != 0xFE)      //Check that the data token was received
//            {
//                return  0x02;       //If not, return with an error token
//            }
//            SSP2BUF = 0xFF;         //Push out the next byte of data
//
//            while(SD_temp < 512)
//            {
//                while(!SSP2STATbits.BF);
//                SPI_buffer[SD_temp++] = SSP2BUF; //Save the received data
//                SSP2BUF = 0xFF;         //Push out the next byte of data
//            }
//
//            SD_temp = 0;
//
//            while(SD_temp < 2)  //Push out the CRC
//            {
//                while(!SSP2STATbits.BF);
//                SSP2BUF = 0xFF;
//                SD_temp++;
//            }
//            
//        break;
//        
//        case 0x52:          //Setup Read Multiple Sector
//            
//            
//            
//        break;
//        
//        case 0x58:         //Write sector
//            
//        break;
//    }
//    
//    PORTEbits.RE2 = true;
//    return 0x00;
//    
//}
//
//int SD_Initialize (void)
//{
//    //Insure SPI settings are proper for the SD Card Initialization.
//    
//    PR2 = 0x16;           //Setup Timer 2 to use with SPI 
//    
//    SSP2STAT = 0x00;    //Sample Data Middle of clock, transmit form idle to active
//    SSP2CON1 = 0x33;
//    
//    //SD Card Setup-------------------------------------
//    
//    PORTEbits.RE2 = true;
//    
//    SSP2BUF = 255;
//    
//    tempbyte = 12;
//    
//    while(tempbyte > 0)
//    {
//        while(!SSP2STATbits.BF);
//        
//        SSP2BUF = 255;
//        tempbyte--;
//        
//    }
//    
//    PORTEbits.RE2 = false;
//
//    SSP2BUF = 0xFF;
//    
//    tempbyte = 3;
//    
//    while (tempbyte > 0)
//    {
//        while(!SSP2STATbits.BF);
//        
//        SSP2BUF = 255;
//        tempbyte--;           
//    }
//    
//    do{
//    
//        SD_COM[0] = 0x40;
//        SD_COM[1] = 0;
//        SD_COM[2] = 0;
//        SD_COM[3] = 0;
//        SD_COM[4] = 0;
//        SD_COM[5] = 0x95;
//
//        SD_Rout();
//    
//    }while(SD_resp[0] != 0x01);
//    
//    do{
//
//        SD_COM[0] = 0x48;
//        SD_COM[3] = 0x01;
//        SD_COM[4] = 0xAA;
//        SD_COM[5] = 0x86;
//
//        SD_Rout();
//    
//    }while(SD_resp[0] != 0x01);
//        
//    SD_resp[3] = 0xFF;
//    
//    SD_COM[0] = 0x7A;
//    SD_COM[3] = 0;
//    SD_COM[4] = 0;
//    SD_COM[5] = 0x95;
//
//    SD_Rout();      
//        
//    if(!CHECK_BIT(SD_resp[3],4))
//    {
//        return 0xAA;    //SD Card can not run at 3.3V don't leave Idle Mode
//    }
//    
//    do{
//        SD_COM[0] = 0x77;
//        SD_COM[1] = 0x00;
//        SD_COM[5] = 0x7A;
//        
//        SD_Rout();
//        
//        if (SD_resp[0] != 0x01) continue;
//        
//        SD_COM[0] = 0x69;
//        SD_COM[1] = 0x40;
//        SD_COM[2] = 0x00;
//        
//        SD_Rout();
//             
//    }while(SD_resp[0] != 0x00);
//    
//    SSP2CON1 = 0x30;    //Kick the SPI up to a 12Mhz Clock
//    
//    return 0x00;
//}