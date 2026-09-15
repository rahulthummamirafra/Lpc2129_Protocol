//spi_eeprom_test.c
#include "spi.h"
#include "spi_eeprom.h"
#include "delay.h"
u8 rDat[8] __attribute__((at(0x40000010)));

main(void) 
{ 
 u32 i;	
 Init_SPI0(); 
 for(i=0;i<=1000;i++)	
  spi_eeprom_bytewrite(i,'A'); 
	
 delay_ms(1000); 

 for(i=0;i<=7;i++)	
  rDat[i]=spi_eeprom_byteread(i);
	
 while(1); 
}
