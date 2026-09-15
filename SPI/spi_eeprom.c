//spi_eeprom.c
#include "pin_function_defines.h"
#include "spi_eeprom_defines.h"
#include <LPC21xx.h>
#include "types.h"
#include "delay.h"
#include "spi.h"

void WriteEnable(void) 
{ 
  //make chip select low 
  IOCLR0=1<<CS_0_7; 
  //issue WREN via spi tranfer 
  spi(WREN); 
 //make chip select high 
  IOSET0=1<<CS_0_7; 
} 

void WriteDisable(void) 
{ 
  //make chip select low 
  IOCLR0=1<<CS_0_7; 
  //issue WRDI via spi tranfer 
  spi(WRDI); 
 //make chip select high 
  IOSET0=1<<CS_0_7; 
} 

void spi_eeprom_bytewrite(u16 wBuffAddr,u8 sDat) 
{ 
   //Enable Write Enable Latch 
   WriteEnable(); 
   //make chip select low 
   IOCLR0=1<<CS_0_7; 
   //issue WRITE instruction via spi transfer 
   spi(WRITE);  
   //msbyte of 16-bit address loc via spi transfer 
   spi(wBuffAddr>>8); 
   //lsbyte of 16-bit address loc via spi transfer 
   spi(wBuffAddr); 
   //data for that address loc 
   spi(sDat); 
   //make chip select high 
   IOSET0=1<<CS_0_7; 
   //delay for internal write cycle 
   delay_ms(5); 
   //Disable Write Enable Latch 
   WriteDisable(); 
}  

u8 spi_eeprom_byteread(u16 rBuffAddr) 
{ 
  u8 rDat; 
  //make chip select low 
  IOCLR0=1<<CS_0_7; 
  //issue READ instruction 
  spi(READ);    
 //msbyte of 16-bit address loc to be read via spi 
  spi(rBuffAddr>>8); 
  //lsbyte of 16-bit address loc to be read via spi 
  spi(rBuffAddr);  
  //sending garbage & reading data at loc   
  rDat=spi(0x00); 
  //make chip select high 
  IOSET0=1<<CS_0_7; 
  return rDat;    
}
