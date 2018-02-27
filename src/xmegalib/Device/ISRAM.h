/*
 * ISRAM.h
 *
 * Created: 2016/02/14 16:42:03
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "IMemoryReader.h"

namespace Device
{
	class ISRAM : public IMemoryReader
	{
	public:
		virtual void Write(uint_fast16_t address, const void* src, unsigned length) = 0;
	};
}

