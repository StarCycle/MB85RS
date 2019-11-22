/* Code written by Chia Jiun Wei @ 24 Jul 2017
 * <J.W.Chia@tudelft.nl>
 
 * MB85RS: a library to provide high level APIs to interface  
 * with the Fujitsu MB85RS. It is possible to use this library in
 * Energia (the Arduino port for MSP microcontrollers) or in 
 * other toolchains.
 
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License 
 * version 3, both as published by the Free Software Foundation.
  
 */
 
#include "MB85RS.h"
  
/**  MB85RS class creator function
 *
 *   Parameters:
 *   DSPI &spi             SPI object
 *	 unsigned char pin	   Chip select GPIO pin
 *
 */
MB85RS::MB85RS(DSPI &spi, unsigned char pin): line(spi)
{
	CSpin = pin;
}

/**  Initialise MB85RS
 *	 Both memory blocks and status register are initialise to be unprotected
 *
 */
void MB85RS::init()
{	
	pinMode(CSpin, OUTPUT);
	write_Disable();	
}

/**  Enable data writing in MB85RS memory space
 *
 */
void MB85RS::write_Enable()
{	
	digitalWrite(CSpin, LOW);
	line.transfer(WREN); 
	digitalWrite(CSpin, HIGH);
}

/**  Disable data writing in MB85RS memory space
 *
 */
void MB85RS::write_Disable()
{	
	digitalWrite(CSpin, LOW);
	line.transfer(WRDI); 
	digitalWrite(CSpin, HIGH);
}

/**  Returns content of MB85RS status register
 *
 *	 Returns
 * 	 unsigned char         status register value
 */
unsigned char MB85RS::read_Status()
{	
	unsigned char ret;
	digitalWrite(CSpin, LOW);
	line.transfer(RDSR);
	ret = line.transfer(0x00);
	digitalWrite(CSpin, HIGH);
	return ret;
}

/**  writes in MB85RS status register
 *
 *	 Parameter
 * 	 char val         status register value
 */
void MB85RS::write_Status(char val)
{
	write_Enable();
	digitalWrite(CSpin, LOW);
	line.transfer(WRSR);
	line.transfer(val);
	digitalWrite(CSpin, HIGH);
	write_Disable();
}

/**  reads sequential memory locations to buffer
 *
 *	 Parameters
 * 	 unsigned int address       start address
 *   char *buffer				buffer
 *   unsigned int size			total number of bytes
 *
 */
void MB85RS::read(unsigned int address, char *buffer, unsigned int size)
{  
	digitalWrite(CSpin, LOW);
	line.transfer(READ);
	line.transfer((char)(address >> 8));
	line.transfer((char)address);
  
	for (unsigned int i = 0; i < size; i++)
		*(buffer + i) = line.transfer(0x00);

	digitalWrite(CSpin, HIGH);
}

/**  writes to sequential memory locations from buffer
 *
 *	 Parameters
 * 	 unsigned int address       start address
 *   char *buffer				buffer
 *   unsigned int size			total number of bytes
 *
 */
void MB85RS::write(unsigned int address, char *buffer, unsigned int size)
{
	write_Enable();
	digitalWrite(CSpin, LOW);
	line.transfer(WRITE);
	line.transfer((char)(address >> 8));
	line.transfer((char)address);

	for (unsigned int i = 0; i < size; i++)
		line.transfer(*(buffer + i));
	
	digitalWrite(CSpin, HIGH);
	write_Disable();
}


/**  flush whole data memory and resets status reg
 *
 */
void MB85RS::erase_All()
{  
	write_Status(0x00);
	write_Enable();
  
	digitalWrite(CSpin, LOW);
	line.transfer(WRITE);
	line.transfer(0x00);
	line.transfer(0x00);
  
	for(unsigned int i = 0; i <= MEM_SIZE; i++)
		line.transfer(0x00);
	
	digitalWrite(CSpin, HIGH);
}
