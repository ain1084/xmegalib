/*
 * II2CMaster.h
 *
 * Created: 2017/09/06 21:05:18
 *  Author: ain
 */ 

#pragma once

#include <stdint.h>

namespace Device
{
	class II2CMaster
	{
	public:
		virtual ~II2CMaster(void) {}

		virtual bool Write(uint8_t address, const uint8_t buffer[], unsigned length) = 0;
		virtual bool Read(uint8_t address, uint8_t buffer[], unsigned length) = 0;
		virtual bool WriteRead(uint8_t address, const uint8_t write[], unsigned writeLength, uint8_t read[], unsigned readLength) = 0;
		virtual bool WriteRegister(uint8_t address, uint8_t registerAddress, uint8_t value) = 0;
	};	
}
