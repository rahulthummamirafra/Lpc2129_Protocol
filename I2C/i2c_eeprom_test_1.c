//i2c_eeprom_test.c
#include "i2c_eeprom.h"
#include "i2c_peripheral.h"
#include "i2c_eeprom_defines.h"
#include "delay.h"
u8 t  __attribute__((at(0x40000000))); 
s8 rbuff[18] __attribute__((at(0x40000040)))=""; 
main() 
{ 
  s8 wbuff[]="0123456789ABCDEF"; 
  init_i2c(); 
  i2c_eeprom_bytewrite(I2C_EEPROM_SA,0x00,'a'); 
  delay_ms(2000); 
  t=i2c_eeprom_randomread(I2C_EEPROM_SA,0x00); 
  delay_ms(2000); 
  i2c_eeprom_pagewrite(I2C_EEPROM_SA,0x10,wbuff,16); 
  delay_ms(1000); 
 	i2c_eeprom_seqread(I2C_EEPROM_SA,0x10,rbuff,16); 
  delay_ms(1000); 
  while(1); 
}
