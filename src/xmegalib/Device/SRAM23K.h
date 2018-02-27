/*
 * SRAM23K.h
 *
 * Created: 2011/12/14 2:43:16
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "ISRAM.h"

namespace Device
{
	class ISPIChip;
	
	class SRAM23K : public ISRAM
	{
	public:
		SRAM23K(ISPIChip& chip);

		uint_fast32_t GetCapacity(void) const;
		virtual uint_fast8_t ReadByte(uint_fast32_t address) const;
		virtual void Read(uint_fast32_t address, void* dest, unsigned length) const;

		virtual void Write(uint_fast16_t address, const void* src, unsigned length);

	private:
		ISPIChip& _spiChip;

		void beginTransfer(uint_fast8_t command, uint_fast16_t address) const;
	};
}
