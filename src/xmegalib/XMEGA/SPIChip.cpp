/*
 * SPIChip.cpp
 *
 * Created: 2011/12/04 23:51:49
 *  Author: Seiji Ainoguchi
 */ 

#include "Platform.h"
#include "Device/LEDPin.h"

using namespace XMEGA;
using namespace Device;

SPIChip::SPIChip(SPIMaster& spi, OutputPin& chipSelect, uint32_t frequency)
 : _spi(spi)
 , _chipSelect(chipSelect)
 , _pLEDPin(nullptr)
 , _baudrateValue(0xFFFF)
{
	 if (frequency != 0)
	 {
		 SetFrequency(frequency);
	 }
	 Inactive();
}

SPIChip::SPIChip(SPIMaster& spi, OutputPin& chipSelect, LEDPin& ledPin, uint32_t frequency /* = 0 */)
 : _spi(spi)
 , _chipSelect(chipSelect)
 , _pLEDPin(&ledPin)
 , _baudrateValue(0xFFFF)
{
	 if (frequency != 0)
	 {
		 SetFrequency(frequency);
	 }
	 Inactive();
}

void SPIChip::Active(void)
{
	if (_pLEDPin != nullptr)
	{
		_pLEDPin->On();
	}
	if (_baudrateValue != 0xFFFF)
	{
		_spi.SetBaudrateValue(_baudrateValue);
	}
	_chipSelect.Low();
}

void SPIChip::Inactive(void)
{
	_chipSelect.High();
	if (_pLEDPin != nullptr)
	{
		_pLEDPin->Off();
	}
}

void SPIChip::SendClock(unsigned clockByteCount)
{
	_spi.SendClock(clockByteCount);
}

uint_fast8_t SPIChip::Read(void)
{
	return _spi.Read();
}

void SPIChip::Read(void* dest, unsigned length)
{
	_spi.Read(dest, length);
}

void SPIChip::Write(uint_fast8_t data)
{
	_spi.Write(data);
}

void SPIChip::Write(const void* src, unsigned length)
{
	_spi.Write(src, length);
}

void SPIChip::SetFrequency(uint32_t frequency)
{
	_baudrateValue = _spi.CalcBaudrateValue(frequency);
	_spi.SetBaudrateValue(_baudrateValue);
}
