//spi.c
#include "types.h"
#include <LPC21xx.h>
#include "pin_function_defines.h"
#include "pin_connect_block.h"
#include "spi_defines.h"

void Init_SPI0(void) 
{ 
   //cfg p0.4 pin as SCK pin  
   CfgPortPinFunc(0,4,SCK_0_4); 
   //cfg p0.5 pin as MIS0 pin
   CfgPortPinFunc(0,5,MISO_0_5); 
   //cfg p0.6 pin as MOSI pin  
   CfgPortPinFunc(0,6,MOSI_0_6); 
   //make p0.7(cs) as high using IOSET0/IOPIN0 
   IOSET0=1<<CS_0_7;  
   //cfg p0.7 as gpio output pin using IODIR0 
   IODIR0|=1<<CS_0_7; 
   //cfg spi clock speed using S0SPCCR 
	 if(SPI_DIVIDER>=8)
	 {
     if(SPI_DIVIDER%2==0)
     {			 
       S0SPCCR=SPI_DIVIDER; 
		 }
     else
     {			 
       S0SPCCR=SPI_DIVIDER+1; 
		 }
   }		 
   //cfg spi peripheral for Master Mode,Mode3,MSBF(def)
    S0SPCR=1<<MSTR_BIT|1<<CPOL_BIT|1<<CPHA_BIT; 
} 

u8 spi(u8 sByte)
{
	u8 rByte;
	//write to spi data buffer,initiates transfer
	S0SPDR=sByte;
	//wait until tranfer (tx/rx)completion status
	while(((S0SPSR>>SPIF_BIT)&1)==0);
	//read recvd byte
	rByte=S0SPDR;
	//return recv byte
	return rByte;
}

