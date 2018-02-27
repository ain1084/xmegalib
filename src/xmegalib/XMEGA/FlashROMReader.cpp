/*
 * FlashROMReader.cpp
 *
 * Created: 2012/06/02 23:45:30
 *  Author: Seiji Ainoguchi
 */ 

#include "Platform.h"
#include <string.h>
#include <avr/pgmspace.h>
#include "FlashROMReader.h"

using namespace XMEGA;

uint_fast8_t FlashROMReader::ReadByte(uint_fast32_t offset) const
{
	return pgm_read_byte_far(offset);
}

void FlashROMReader::Read(uint_fast32_t offset, void* dest, unsigned length) const
{
	memcpy_PF(dest, offset, length);
}
