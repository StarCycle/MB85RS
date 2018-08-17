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

#ifndef __MB85RS256A_H
#define __MB85RS256A_H

#include <stdint.h>
#include "satellite.h"

void FRAM_init(dev_id id);
void FRAM_write_Enable(dev_id id);
void FRAM_write_Disable(dev_id id);
void FRAM_read_Status(dev_id id, uint8_t *res);
void FRAM_write_Status(dev_id id, uint8_t val);
void FRAM_read(dev_id id, uint16_t address, uint8_t *buf, size_t count);
void FRAM_write(dev_id id, uint16_t address, uint8_t *buf, size_t count);
void FRAM_erase_All(dev_id id);

#endif	// MB85RS256A_H
