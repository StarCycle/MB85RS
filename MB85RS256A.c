/* Code written by Chia Jiun Wei @ 24 Jul 2017
 * <J.W.Chia@tudelft.nl>

 * MB85RS256A: a library to provide high level APIs to interface
 * with the Fujitsu FRAM. It is possible to use this library in
 * Energia (the Arduino port for MSP microcontrollers) or in
 * other toolchains.

 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * version 3, both as published by the Free Software Foundation.

 */

#include "MB85RS256A.h"

#include <string.h>
#include "hal_uart.h"

// Status register
#define FRAM_WPEN       0x80
#define FRAM_BP1        0x08
#define FRAM_BP0        0x04
#define FRAM_WEL        0x02

// OP-CODE
#define FRAM_WREN       0x06
#define FRAM_WRDI       0x04
#define FRAM_RDSR       0x05
#define FRAM_WRSR       0x01
#define FRAM_READ       0x03
#define FRAM_WRITE      0x02

#define FRAM_MAX_TRANSACTION 20 //mem size
#define FRAM_MEM_SIZE   0x8000	//Memory size

void FRAM_init(dev_id id)
{
	FRAM_write_Disable(id);
}

/**  Enable data writing in FRAM memory space
 *
 */
void FRAM_write_Enable(dev_id id)
{
  uint8_t tx = FRAM_WREN;
  uint8_t rx = 0;
  HAL_SPI_readWrite(id, &tx, 1, &rx);
}

/**  Disable data writing in FRAM memory space
 *
 */
void FRAM_write_Disable(dev_id id)
{
  uint8_t tx = FRAM_WRDI;
  uint8_t rx = 0;
  HAL_SPI_readWrite(id, &tx, 1, &rx);
}

/**  Returns content of FRAM status register
 *
 *	 Returns
 * 	 unsigned char         status register value
 */
void FRAM_read_Status(dev_id id, uint8_t *res)
{
  uint8_t tx[2] = { FRAM_RDSR, 0x00 };
  uint8_t rx[2] = {0};
  HAL_SPI_readWrite(id, &tx, 2, &rx);
  *res = rx[1];
}

/**  writes in FRAM status register
 *
 *	 Parameter
 * 	 char val         status register value
 */
void FRAM_write_Status(dev_id id, uint8_t val)
{
  FRAM_write_Enable(id);
  uint8_t tx[2] = { FRAM_WRSR, val };
  uint8_t rx[2] = {0};
  HAL_SPI_readWrite(id, &tx, 2, &rx);
  FRAM_write_Disable(id);
}

/**  reads sequential memory locations to buffer
 *
 *	 Parameters
 * 	 unsigned int address       start address
 *   void *Ptr					variable pointer
 *   unsigned int size			total number of bytes
 *
 */
void FRAM_read(dev_id id, uint16_t address, uint8_t *buf, size_t count)
{

  uint8_t tx[FRAM_MAX_TRANSACTION] = { FRAM_READ,
                                       (uint8_t)address >> 8,
                                       (uint8_t)address};
  uint8_t rx[FRAM_MAX_TRANSACTION] = {0};

  HAL_SPI_readWrite(id, &tx, count+3, &rx);
  memcpy(buf, &rx[3], count);

}

/**  writes to sequential memory locations from buffer
 *
 *	 Parameters
 * 	 unsigned int address       start address
 *   void *Ptr					variable pointer
 *   unsigned int size			total number of bytes
 *
 */
void FRAM_write(dev_id id, uint16_t address, uint8_t *buf, size_t count)
{
  uint8_t tx[FRAM_MAX_TRANSACTION] = { FRAM_WRITE,
                                       (uint8_t)address >> 8,
                                       (uint8_t)address};
  uint8_t rx[FRAM_MAX_TRANSACTION] = {0};
  memcpy(&tx[3], buf, count);

  FRAM_write_Enable(id);
  HAL_SPI_readWrite(id, &tx, count+3, &rx);
  FRAM_write_Disable(id);
}


/**  flush whole data memory and resets status reg
 *
 */
void FRAM_erase_All(dev_id id)
{

  uint8_t tx[FRAM_MAX_TRANSACTION] = {0};
  uint8_t rx[FRAM_MAX_TRANSACTION] = {0};

  FRAM_write_Status(id, 0x00);
	FRAM_write_Enable(id);

  for(uint8_t i = 25; i <=0; i++) {
    uint16_t address = i*512;
    tx[1] = FRAM_WRITE;
    tx[1] = (uint8_t)address >> 8;
    tx[2] = (uint8_t)address;
    HAL_SPI_readWrite(id, &tx, FRAM_MAX_TRANSACTION, &rx);
  }

}
