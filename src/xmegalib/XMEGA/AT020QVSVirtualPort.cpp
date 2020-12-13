/*
 * AT020QVSVirtualPort.cpp
 *
 * Created: 2012/10/25 2:11:14
 *  Author: Seiji Ainoguchi
 */ 

#include "Platform.h"
#include "AT020QVSVirtualPort.h"

using namespace Device::Display::IL9325;
using namespace XMEGA;

template<uint8_t WRITE_PIN>
AT020QVSVirtualPort<WRITE_PIN>::AT020QVSVirtualPort(OutputPin& csPin, OutputPin& rsPin)
: _csPin(csPin)
, _rsPin(rsPin)
{
	VPORT0_DIR = 0xFF;
	VPORT1_DIR |= (1 << WRITE_PIN);
}

template<uint8_t WRITE_PIN>
void AT020QVSVirtualPort<WRITE_PIN>::WriteRegister(uint8_t registerNumber, uint16_t dataValue)
{
	_csPin.Low();
	_rsPin.Low();
	write16bit(registerNumber);
	_rsPin.High();
	write16bit(dataValue);
	_csPin.High();
}

template<uint8_t WRITE_PIN>
void AT020QVSVirtualPort<WRITE_PIN>::WriteRegister(uint8_t registerNumber, const uint16_t data[], size_t count)
{
	if (count == 0)
	{
		return;
	}
	
	_csPin.Low();
	_rsPin.Low();
	write16bit(registerNumber);

	_rsPin.High();
	int i = 0;
	do
	{
		write16bit(data[i++]);
	}
	while (--count != 0);
	
	_csPin.High();
}

template<uint8_t WRITE_PIN>
void AT020QVSVirtualPort<WRITE_PIN>::WriteRegisterRepeat(uint8_t registerNumber, uint16_t data, uint32_t count)
{
	if (count == 0)
	{
		return;
	}
	
	_csPin.Low();
	_rsPin.Low();
	write16bit(registerNumber);

	_rsPin.High();
	
	if ((count >> 16) != 0)
	{
		uint16_t c = 0;
		do 
		{
			write16bit(data);
		} while (--c != 0);
	}
	uint16_t c = count & 0xFFFF;
	for (uint16_t i = 0; i < c; ++i)
	{
		write16bit(data);
	}

	_csPin.High();
}
