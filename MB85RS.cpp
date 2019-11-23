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
MB85RS::MB85RS(DSPI &spi, unsigned long csPort, unsigned long csPin):
        line(spi), CSPort(csPort), CSPin(csPin) {}

/**  Initialize MB85RS
 *	 Both memory blocks and status register are initialize to be unprotected
 *
 */
void MB85RS::init()
{	
    MAP_GPIO_setOutputHighOnPin( CSPort, CSPin );
    MAP_GPIO_setAsOutputPin( CSPort, CSPin );
	write_Disable();	
}

/**  Enable data writing in MB85RS memory space
 *
 */
void MB85RS::write_Enable()
{	
	MAP_GPIO_setOutputLowOnPin( CSPort, CSPin );
	line.transfer(WREN); 
    MAP_GPIO_setOutputHighOnPin( CSPort, CSPin );
}

/**  Disable data writing in MB85RS memory space
 *
 */
void MB85RS::write_Disable()
{	
	MAP_GPIO_setOutputLowOnPin( CSPort, CSPin );
	line.transfer(WRDI); 
	MAP_GPIO_setOutputHighOnPin( CSPort, CSPin );
}

/**  Returns content of MB85RS status register
 *
 *	 Returns
 * 	 unsigned char         status register value
 */
unsigned char MB85RS::read_Status()
{	
	unsigned char ret;
	MAP_GPIO_setOutputLowOnPin( CSPort, CSPin );
	line.transfer(RDSR);
	ret = line.transfer(0x00);
	MAP_GPIO_setOutputHighOnPin( CSPort, CSPin );
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
	MAP_GPIO_setOutputLowOnPin( CSPort, CSPin );
	line.transfer(WRSR);
	line.transfer(val);
	MAP_GPIO_setOutputHighOnPin( CSPort, CSPin );
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
void MB85RS::read(unsigned int address, unsigned char *buffer, unsigned int size)
{  
	MAP_GPIO_setOutputLowOnPin( CSPort, CSPin );
	line.transfer(READ);
	line.transfer((char)(address >> 8));
	line.transfer((char)address);
  
	for (unsigned int i = 0; i < size; i++)
	{
		*(buffer + i) = line.transfer(0x00);
	}

	MAP_GPIO_setOutputHighOnPin( CSPort, CSPin );
}

/**  writes to sequential memory locations from buffer
 *
 *	 Parameters
 * 	 unsigned int address       start address
 *   char *buffer				buffer
 *   unsigned int size			total number of bytes
 *
 */
void MB85RS::write(unsigned int address, unsigned char *buffer, unsigned int size)
{
	write_Enable();
	MAP_GPIO_setOutputLowOnPin( CSPort, CSPin );
	line.transfer(WRITE);
	line.transfer((char)(address >> 8));
	line.transfer((char)address);

	for (unsigned int i = 0; i < size; i++)
	{
		line.transfer(*(buffer + i));
	}
	
	MAP_GPIO_setOutputHighOnPin( CSPort, CSPin );
	write_Disable();
}


/**  flush whole data memory and resets status register
 *
 */
void MB85RS::erase_All()
{  
	write_Status(0x00);
	write_Enable();
  
	MAP_GPIO_setOutputLowOnPin( CSPort, CSPin );
	line.transfer(WRITE);
	line.transfer(0x00);
	line.transfer(0x00);
  
	for(unsigned int i = 0; i <= MEM_SIZE; i++)
	{
		line.transfer(0x00);
	}
	
	MAP_GPIO_setOutputHighOnPin( CSPort, CSPin );
}
