/*
 * ISPIChip.h
 *
 * Created: 2017/09/06 21:36:23
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <stdint.h>

namespace Device
{
	class ISPIChip
	{
	public:
		virtual ~ISPIChip(void) {}

		virtual void SetFrequency(uint32_t frequency) = 0;
		virtual void Active(void) = 0;
		virtual void Inactive(void) = 0;

		virtual uint8_t Read(void) = 0;
		virtual void SendClock(unsigned clockByteCount) = 0;
		virtual void Read(void* dest, unsigned length) = 0;
		virtual void Write(uint_fast8_t data) = 0;
		virtual void Write(const void* src, unsigned length) = 0;
	};
}
