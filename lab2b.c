
//Lab 2- General Purpose I/O
//Question 2- LED Matrix 8x8 is driven by MAX7219
//Display the students names

// pin connection
// VCC : to Vcc +5V
// GND : to Gnd
// DIN : to GPA0
// CS  : to GPA1
// CLK : to GPA2

#include <stdio.h>                                                                                                           
#include "NUC1xx.h"
#include "Driver\DrvSYS.h"
#include "Driver\DrvGPIO.h"

#define  CLK_HI  DrvGPIO_SetBit(E_GPA, 2)
#define  CLK_LO  DrvGPIO_ClrBit(E_GPA, 2)
#define  CS_HI   DrvGPIO_SetBit(E_GPA, 1)
#define  CS_LO   DrvGPIO_ClrBit(E_GPA, 1)
#define  DIN_HI  DrvGPIO_SetBit(E_GPA, 0)
#define  DIN_LO  DrvGPIO_ClrBit(E_GPA, 0)

uint8_t Font[6][8]={  // two demantioanl array for storing the symbols
                        // avery row contain what led in each row need to turn on in hex base
{0x09,0x09,0x09,0x09,0x09,0x09,0x09,0xff},// eyin
{0xff,0x04,0x04,0x04,0x04,0x04,0x04,0x04},// dalet
{0x70,0x10,0x10,0x10,0x10,0x10,0x10,0x10},// final non
{0x00,0x80,0x50,0x28,0x44,0x82,0x01,0x7f},//mam 
{0x00,0x3f,0x21,0x21,0x21,0x21,0x21,0xe1},// taf
{0x70,0x10,0x10,0x10,0x10,0x10,0x10,0x10},// final non
};

void Init_GPIO_pins(void)
{
    DrvGPIO_InitFunction(E_FUNC_GPIO);
    DrvGPIO_Open(E_GPA, 0,  E_IO_OUTPUT);   //DIN
    DrvGPIO_Open(E_GPA, 1,  E_IO_OUTPUT);       //CS
    DrvGPIO_Open(E_GPA, 2,  E_IO_OUTPUT);       //CLK
    // set to default
    DrvGPIO_ClrBit(E_GPA, 0);
    DrvGPIO_ClrBit(E_GPA, 1);
    DrvGPIO_ClrBit(E_GPA, 2);
}

void Write_Max7219_byte(uint8_t DATA)         
{
    uint8_t i;    
    CS_LO;      
    for(i=8;i>=1;i--) {       //cheking the row/coulom to be turn on/off
     CLK_LO;
         if (DATA&0x80) DIN_HI; // logic and between DATA and 0x80 (=10000000) if true insert '1' to DIN
         else           DIN_LO;  // in case false '0' to DIN
     DATA=DATA<<1;           // shifting the DATA left for checking the next bit
     CLK_HI;                                    // insert the bit in to register
         DrvSYS_Delay(10);
  }                                 
}

void Write_Max7219(uint8_t address,uint8_t dat)
{ 
   CS_LO;                         // open the register for reading data
     Write_Max7219_byte(address);           
   Write_Max7219_byte(dat);     
     CS_HI;                        // close the register for reading data
}

void Init_MAX7219(void)         // inital the 8X8 on the first run
{
 Write_Max7219(0x09, 0x00);
 Write_Max7219(0x0a, 0x03);
 Write_Max7219(0x0b, 0x07);
 Write_Max7219(0x0c, 0x01);
 Write_Max7219(0x0f, 0x00);
}

int main(void)
{
  uint8_t i,j;    // integers for the for loops to run the letter
    UNLOCKREG();
  DrvSYS_Open(48000000);
  LOCKREG();
    
    Init_GPIO_pins();   // initial GPA 0,1,2 pins that control the 8X8 leds
  Init_MAX7219();       
    
  while(1)
  {
   for(j=0;j<6;j++)   // control what symbol to show
   {
    for(i=1;i<9;i++)               // control what leds need to turn on/off for display
                                                                    // the j symbol
         Write_Max7219(i,Font[j][i-1]);   // sending the adress and the data for checking what led to turn on/off
    DrvSYS_Delay(1000000); // 100ms delay between symbols
   }  
        DrvSYS_Delay(1000000);  // delay beofre showing the symbols again
  } 
}               





