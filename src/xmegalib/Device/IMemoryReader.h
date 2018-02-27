/*
 * IMemoryReader.h
 *
 * Created: 2012/05/07 1:19:16
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <stdint.h>

namespace Device
{
	class IMemoryReader
	{
	public:
		virtual ~IMemoryReader(void) { }
		
		virtual uint_fast8_t  ReadByte(uint_fast32_t address) const = 0;
		virtual void Read(uint_fast32_t address, void* dest, unsigned length) const = 0;
	};
}