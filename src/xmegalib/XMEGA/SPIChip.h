/*
 * SPIChip.h
 *
 * Created: 2011/12/04 23:50:15
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <Device/ISPIChip.h>

namespace Device
{
	class LEDPin;
}

namespace XMEGA
{
	class SPIMaster;
	class OutputPin;

	class SPIChip : public Device::ISPIChip
	{
	public:
		SPIChip(SPIMaster& spi, OutputPin& chipSelect, uint32_t frequency = 0);

		SPIChip(SPIMaster& spi, OutputPin& chipSelect, Device::LEDPin& ledPin, uint32_t frequency = 0);

		virtual void SetFrequency(uint32_t frequency);

		virtual void Active(void);
		virtual void Inactive(void);

		virtual uint8_t Read(void);
		virtual void SendClock(unsigned clockByteCount);
		virtual void Read(void* dest, unsigned length);
		virtual void Write(uint_fast8_t data);
		virtual void Write(const void* src, unsigned length);

	private:
		SPIChip(const SPIChip& rhs);
		SPIChip& operator=(const SPIChip& rhs);

		SPIMaster& _spi;
		OutputPin& _chipSelect;
		Device::LEDPin* const _pLEDPin;

		uint16_t _baudrateValue;
	};
}
