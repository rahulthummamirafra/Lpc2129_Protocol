#include <LPC21xx.h>     // Header file for LPC214x microcontrollers
   
#include "pin_connect_block.h"	 
#include "types.h"
#include "uart0.h"


/* UART Pin Configuration Definitions */
#define PIN_FUN1 0
#define PIN_FUN2 1
#define PIN_FUN3 2
#define PIN_FUN4 3

#define TXD0_PIN_0_0 PIN_FUN2
#define RXD0_PIN_0_1 PIN_FUN2

/*
  UART0 Baud Rate Formula:
  -------------------------
  Baud Rate = PCLK / (16 * DIVISOR)
  where,
  DIVISOR = (U0DLM * 256) + U0DLL

  To get 9600 baud rate:
  - We calculate DIVISOR based on peripheral clock (PCLK)
*/

/* Clock and Baud Rate Definitions */
#define FOSC  12000000      // External crystal frequency = 12 MHz
#define CCLK  (FOSC*5)	    // CPU Clock (CCLK) = 60 MHz (using PLL multiplier = 5)
#define PCLK  (CCLK/4)	    // Peripheral Clock (PCLK) = 15 MHz (default setting)
#define BAUD  9600	    // Desired baud rate
#define DIVISOR (PCLK/(16*BAUD))     // Divisor value for baud rate generation

/* U0LCR Register Bit Definitions */
#define _8BIT 3               // Define a symbolic constant "_8BIT" with value 3
                             // In UART Line Control Register (U0LCR), bits 1:0 
                             // determine the word length:
                             //   00 -> 5-bit data
                             //   01 -> 6-bit data
                             //   10 -> 7-bit data
                             //   11 -> 8-bit data
                             // So, 3 (binary 11) selects 8-bit data mode.

#define WORD_LEN _8BIT       // Define "WORD_LEN" as alias for "_8BIT"
                             // This improves code readability and allows changing 
                             // the data word length easily in one place if needed.

#define DLAB_BIT 7     	     // Bit 7 of U0LCR register enables access to Divisor Latch registers

/* U0LSR Register Bit Definitions */
#define RDR_BIT   0  // Bit 0 of U0LSR — Receiver Data Ready
#define THRE_BIT  5  // Bit 5 of U0LSR — Transmit Holding Register Empty
#define TEMT_BIT  6  // Bit 6 of U0LSR — Transmitter Empty (shift register empty)

/*
Function: Init_UART0()
Purpose : Initialize UART0 peripheral for serial communication
*/
void UART0_Init(void)
{
	// Configure P0.0 as TXD0 and P0.1 as RXD0 (alternate function selection)
	CfgPortPinFunc(0,0,TXD0_PIN_0_0);
	CfgPortPinFunc(0,1,RXD0_PIN_0_1);
	// Configure UART0 line control for:
	// - 8 data bits
	// - 1 stop bit
	// - No parity
	// - DLAB = 1 (to access baud rate divisor registers)
	U0LCR = WORD_LEN| (1<<DLAB_BIT); 
	// Load divisor value for 9600 baud rate
	// Lower 8 bits of divisor
	// Upper 8 bits of divisor
	U0DLL = DIVISOR;
	U0DLM = DIVISOR>>8;

	// Disable DLAB (so that THR and RBR can be accessed)
	U0LCR &=~(1<<DLAB_BIT);
}

/*
Function: UART0_Txchar()
Purpose : Transmit one byte of data using UART0
*/
void UART0_TxChar(unsigned char TxByte)
{
	// Load the data byte into UART0 Transmit Holding Register
	U0THR = TxByte;
	// Wait until Transmitter is completely empty (TEMT = 1 ? both THR and shift register are empty)
	while(((U0LSR>>TEMT_BIT)&1) == 0);
}

/*
Function: UART0_RxChar()
Purpose : Receive one byte of data from UART0
*/
unsigned char UART0_RxChar(void)
{
	// Wait until Receiver Data Ready (RDR = 1)
	while(((U0LSR>>RDR_BIT)&1) == 0);
	// Return the received data byte from Receiver Buffer Register
	return U0RBR;
} 


/*
Function: UART0_TxString()
Purpose : Transmit one string using UART0
*/
void UART0_TxString(char *str) 
{
	while(*str)
		UART0_TxChar(*str++);

}

/*
Function: UART0_TxUInt()
Purpose : Transmit one +ve integer value using UART0
*/
void UART0_TxUInt(u32 n)
{
	s32 i=0;
	u8 a[10];
	
	if(n==0)
	{
		UART0_TxChar('0');
	}
	else
	{
		while(n>0)
		{
		  a[i++]=(n%10)+48;
      n/=10;			
		}
		for(--i;i>=0;i--)
		  UART0_TxChar(a[i]);
	}
}

/*
Function: UART0_TxSInt()
Purpose : Transmit one -ve integer value using UART0
*/
void UART0_TxSInt(s32 n)
{
  if(n<0)
  {
		UART0_TxChar('-');
		n=-n;
	}		
	UART0_TxUInt(n);
}	

/*
Function: UART0_TxFloat()
Purpose : Transmit one float value using UART0
*/
void UART0_TxFloat(f32 fn,u8 nDP)
{
	u32 n,i;
	if(fn<0.0)
	{
		UART0_TxChar('-');
		fn=-fn;
	}
  n=fn;
  UART0_TxUInt(n);
  UART0_TxChar('.');
  for(i=0;i<nDP;i++)
  {
		fn=(fn-n)*10;
		n=fn;
		UART0_TxChar(n+48);
  }		
}

void UART0_RxString(u8 *str,u32 MaxLen)
{
	u32 i=0;
	u8 ch;
	
	while(1)
	{
		ch = UART0_RxChar();
		if(ch == '\r' || ch == '\n')
		{
			str[i] = '\0';
			break;			
		}
		else if(i>=MaxLen-1)
		{
			str[i] = '\0';
			break;
		}
		else
			str[i++] = ch;		
	}
}



/*
Function: main()
Purpose : Main application — sends A–Z repeatedly and echoes received data
*/
int main()
{
	// Declare local variables for loop and received character
	unsigned char ch,buffer[10];
	// Initialize UART0 for 9600 baud, 8N1 format
	UART0_Init();
	UART0_TxString("UART0 Testing\r\n");
	while(1)                // Infinite loop
	{
		UART0_TxString("Character:");
		UART0_TxChar('A');
		UART0_TxString("\r\n+ve Number:");
		UART0_TxUInt(1234);
		UART0_TxString("\r\n-ve Number:");
		UART0_TxSInt(-5845);
		UART0_TxString("\r\nFloat Number:");
		UART0_TxFloat(123.456,3);
		UART0_TxString("\r\n");
		//ch = UART0_RxChar();
		//UART0_TxChar(ch);
		UART0_TxString("Waiting for string");
		UART0_TxString("\r\n");
		UART0_RxString(buffer,10);
		UART0_TxString("Received string is:");
		UART0_TxString(buffer);
		UART0_TxString("\r\n");
	}	
}
