/*
 * FlashROMReader.h
 *
 * Created: 2012/06/02 23:45:18
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "Device/IMemoryReader.h"

namespace XMEGA
{
	class FlashROMReader : public Device::IMemoryReader
	{
	public:
		virtual uint_fast8_t  ReadByte(uint_fast32_t offset) const;
		virtual void Read(uint_fast32_t offset, void* dest, unsigned length) const;
	};
}
