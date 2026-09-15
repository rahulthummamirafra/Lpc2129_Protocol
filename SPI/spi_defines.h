//spi_defines.h

//SxSPCR Bits Setting  
#define CPHA_BIT  3  // 0,1,0,1
#define CPOL_BIT  4  // 0,0,1,1
#define MSTR_BIT  5  // 1=Master,0=Slave  
#define LSBF_BIT  6  // 0=MSB first,1=LSB first 

//SxSPSR Status Register bits 
#define SPIF_BIT   7   // Data Transfer Completion Flag 

#define FOSC 12000000
#define CCLK (FOSC*5)
#define PCLK (CCLK/4)
#define SPI_CLK 1000000
#define SPI_DIVIDER (PCLK/SPI_CLK)
