/*
 * IEEPROM.h
 *
 * Created: 2012/04/22 23:21:06
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "IMemoryReader.h"

namespace Device
{
	class IEEPROM : public IMemoryReader
	{
	public:
		virtual ~IEEPROM(void) { }

		virtual uint_fast32_t GetCapacity(void) const = 0;
		virtual const char* GetName(void) const = 0;
		virtual uint16_t GetPageSize(void) const = 0;
		virtual uint32_t GetBlockSize(void) const = 0;
		virtual bool IsReady(void) const = 0;
		virtual void WaitReady(void) const = 0;
		virtual void EraseChip(void) = 0;
		virtual void EraseBlock(uint32_t address) = 0;
		virtual void WritePageBuffer(uint_fast16_t offset, const void* buffer, unsigned length) = 0;
		virtual void WritePage(uint32_t address) = 0;
	};
}
