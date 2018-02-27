/*
 * CS2200_I2C.h
 *
 * Created: 2012/05/19 1:37:19
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <stdint.h>
#include "Utilities/ScopedPtr.h"

namespace Device
{
	class II2CMaster;
	class ISPIChip;

	class CS2200
	{
	public:
		enum Address
		{
			DefaultAD0 = 0x4E,
			DefaultAD1 = 0x4F,
		};
	
		CS2200(II2CMaster& i2cMaster, uint8_t address, uint32_t inputClock);
		CS2200(ISPIChip& spiChip, uint32_t inputClock);

		void SetOutputClock(uint32_t clock);
		uint32_t GetOutputClock(void) const;

		~CS2200(void);

	private:
		class ICS2200Port
		{
		public:
			virtual ~ICS2200Port(void) { }
			virtual bool Write(uint8_t map, const uint8_t data[], unsigned count) = 0;
			virtual bool Read(uint8_t map, uint8_t data[], unsigned count) = 0;
		};
	
		class I2CCS2200Port : public ICS2200Port
		{
		public:
			I2CCS2200Port(II2CMaster& i2cMaster, uint8_t address);
			virtual bool Write(uint8_t map, const uint8_t data[], unsigned count);
			virtual bool Read(uint8_t map, uint8_t data[], unsigned count);
		private:
			II2CMaster& _i2cMaster;
			const uint8_t _address;
		};

		class SPICS2200Port : public ICS2200Port
		{
		public:
			SPICS2200Port(ISPIChip& spiChip);
			virtual bool Write(uint8_t map, const uint8_t data[], unsigned count);
			virtual bool Read(uint8_t map, uint8_t data[], unsigned count);
		private:
			ISPIChip& _spiChip;
		};

		ScopedPtr<ICS2200Port> _pPort;

		const uint32_t _inputClock;
		uint32_t _outputClock;
	};
}
