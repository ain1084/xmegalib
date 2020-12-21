/*
 * I2CMaster.h
 *
 * Created: 2012/04/30 0:24:30
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <avr/io.h>

#include <Device/II2CMaster.h>
#include "ISystemClockNotify.h"

namespace XMEGA
{
	class I2CMaster : public Device::II2CMaster, protected ISystemClockNotify
	{
	public:
		I2CMaster(TWI_t& twi, uint32_t busFrequency = 0);
		virtual ~I2CMaster(void);
		
		virtual bool Write(uint8_t address, const uint8_t buffer[], unsigned length);
		virtual bool Read(uint8_t address, uint8_t buffer[], unsigned length);
		virtual bool WriteRead(uint8_t address, const uint8_t write[], unsigned writeLength, uint8_t read[], unsigned readLength);
		virtual bool WriteRegister(uint8_t address, uint8_t registerAddress, uint8_t value);
		
	protected:
		virtual void OnPeripheralClockChanged(uint32_t newClock);
		
	private:
		volatile TWI_MASTER_t& _twiMaster;
		const uint32_t _busFrequency;

		void applyBaudrate(void);
		void stop(void);
	};
}