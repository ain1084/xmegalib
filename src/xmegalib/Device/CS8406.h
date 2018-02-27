/*
 * ICS8406.h
 *
 * Created: 2012/08/07 0:42:38
 *  Author: Seiji Ainoguchi
 */ 


#pragma once

#include <stddef.h>
#include "Utilities/ScopedPtr.h"

namespace Device
{
	class II2CMaster;
	class ISPIChip;
	
	class CS8406
	{
	public:
		enum Clock
		{
			Clock_RUN = 0x40,
			Clock_CLK1 = 0x20,
			Clock_CLK0 = 0x10
		};
		
		enum Format
		{
			Format_SIMS = 0x80,
			Format_SISF = 0x40,
			Format_SIRES1 = 0x20,
			Format_SIRES0 = 0x10,
			Format_SIJUST = 0x08,
			Format_SIDEL = 0x04,
			Format_SISPOL = 0x02,
			Format_SILRPOL = 0x01,
			
		};

		CS8406(ISPIChip& spiChip);
		CS8406(II2CMaster& i2cMaster, uint8_t address);

		bool SetClockSourceControl(uint8_t data);
		bool GetClockSourceControl(uint8_t& data);
		bool SetSerialInputFormat(uint8_t data);
		bool GetSerialInputFormat(uint8_t& data);
		bool GetIdAndVersion(uint8_t& data);
		bool SetChannelStatus(const uint8_t data[], size_t count);

		~CS8406(void);

	private:
		class CS8406Port
		{
		public:
			virtual ~CS8406Port(void) { }
		
			virtual bool Write(uint8_t address, uint8_t data) = 0;
			virtual bool Read(uint8_t address, uint8_t& data) = 0;
		};
		
		class I2C_CS8406Port : public CS8406Port
		{
		public:
			I2C_CS8406Port(II2CMaster& i2cMaster, uint8_t address);
			virtual ~I2C_CS8406Port(void);

			virtual bool Write(uint8_t address, uint8_t data);
			virtual bool Read(uint8_t address, uint8_t& data);

		private:
			II2CMaster& _i2cMaster;
			const uint8_t _address;
		};

		class SPI_CS8406Port : public CS8406Port
		{
		public:
			SPI_CS8406Port(ISPIChip& spiChip);
			virtual ~SPI_CS8406Port(void);

			virtual bool Write(uint8_t address, uint8_t data);
			virtual bool Read(uint8_t address, uint8_t& data);

		private:
			ISPIChip& _spiChip;
		};
		ScopedPtr<CS8406Port> _pPort;
	};
}