/*
 * AT020QVS164ControlPort.cpp
 *
 * Created: 2012/09/28 10:20:53
 *  Author: Seiji Ainoguchi
 */ 

#include "Platform.h"
#include "AT020QVS164ControlPort.h"

using namespace Device;
using namespace Device::Display::IL9325;


AT020QVS164ControlPort::AT020QVS164ControlPort(SPIChip& chip, OutputPin& rsPin, OutputPin& wrPin, OutputPin& backlightPin)
: _chip(chip)
, _rsPin(rsPin)
, _wrPin(wrPin)
, _pBacklightPin(&backlightPin)
{

}

AT020QVS164ControlPort::AT020QVS164ControlPort(SPIChip& chip, OutputPin& rsPin, OutputPin& wrPin)
: _chip(chip)
, _rsPin(rsPin)
, _wrPin(wrPin)
, _pBacklightPin(nullptr)
{

}

void AT020QVS164ControlPort::WriteRegister(uint8_t registerNumber, uint16_t dataValue)
{
	_chip.Active();
	_rsPin.Low();
	write16bit(registerNumber);
	_rsPin.High();
	write16bit(dataValue);
	_chip.Inactive();
}

void AT020QVS164ControlPort::WriteRegister(uint8_t registerNumber, const uint16_t data[], size_t count)
{
	if (count == 0)
	{
		return;
	}
	
	_chip.Active();
	_rsPin.Low();
	write16bit(registerNumber);

	_rsPin.High();
	int i = 0;
	do
	{
		write16bit(data[i++]);
	}
	while (--count != 0);
	
	_chip.Inactive();
}

void AT020QVS164ControlPort::WriteRegisterRepeat(uint8_t registerNumber, uint16_t data, uint32_t count)
{
	if (count == 0)
	{
		return;
	}
	
	_chip.Active();
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

	_chip.Inactive();
}
