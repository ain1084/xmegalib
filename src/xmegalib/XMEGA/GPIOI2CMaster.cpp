/*
 * GPIOI2CMaster.cpp
 *
 * Created: 2017/09/07 22:21:27
 *  Author: Seiji Ainoguchi
 */ 

#include "Platform.h"
#include "GPIO.h"
#include "GPIOI2CMaster.h"

using namespace XMEGA;

namespace
{
	void waitT2(SystemClockCounter::ValueType clock)
	{
		SystemClockTimer::WaitForReached(clock);
	}
}

GPIOI2CMaster::GPIOI2CMaster(GPIO& scl, GPIO& sda)
: _scl(scl)
, _sda(sda)
{
	_scl.SetCtrl(PORT_OPC_WIREDAND_gc);
	_scl.SetOutput();
	_scl.SetLow();
	_scl.SetInput();
	_sda.SetCtrl(PORT_OPC_WIREDAND_gc);
	_sda.SetOutput();
	_sda.SetLow();
	_sda.SetInput();
}

GPIOI2CMaster::~GPIOI2CMaster(void)
{
}

bool GPIOI2CMaster::Write(uint8_t address, const uint8_t buffer[], unsigned length)
{
	startCondition();
	writeByte(address << 1 | 0);
	if (!readAcknowledge())
	{
		stopCondition();
		return false;
	}
	
	const uint8_t* pBuffer = buffer;
	while (length-- != 0)
	{
		writeByte(*pBuffer++);
		if (!readAcknowledge())
		{
			return false;
		}
	}
	return true;
}

bool GPIOI2CMaster::Read(uint8_t address, uint8_t buffer[], unsigned length)
{
	startCondition();
	writeByte(address << 1 | 0x1);
	if (!readAcknowledge())
	{
		stopCondition();
		return false;
	}
	
	uint8_t* pBuffer = buffer;
	while (length != 0)
	{
		*pBuffer++ = readByte();
		if (!readAcknowledge())
		{
			break;
		}
		length--;
	}
	return length == 0;
}

bool GPIOI2CMaster::WriteRead(uint8_t address, const uint8_t write[], unsigned writeLength, uint8_t read[], unsigned readLength)
{
	if (!Write(address, write, writeLength))
	{
		return false;
	}
	return Read(address, read, readLength);
}

void GPIOI2CMaster::Stop(void)
{
	stopCondition();
}

void GPIOI2CMaster::startCondition(void)
{
	auto t2Clock = SystemClockCounter::UStoClock(1);
	
	_scl.SetInput();
	_sda.SetInput();
	waitT2(t2Clock);
	_sda.SetOutput();
	waitT2(t2Clock);
}

void GPIOI2CMaster::stopCondition(void)
{
	auto t2Clock = SystemClockCounter::UStoClock(1);
	
	_scl.SetOutput();
	_sda.SetOutput();
	waitT2(t2Clock);
	_scl.SetInput();
	waitT2(t2Clock);
	_sda.SetInput();
	waitT2(t2Clock);
}

void GPIOI2CMaster::writeByte(uint8_t data)
{
	auto t2Clock = SystemClockCounter::UStoClock(1);

	for (uint_fast8_t i = 0; i < 8; ++i)
	{
		_scl.SetOutput();
		if (data & 0x80)
		{
			_sda.SetInput();
		}
		else
		{
			_sda.SetOutput();
		}
		data <<= 1;
		waitT2(t2Clock);
		_scl.SetInput();
		waitT2(t2Clock);
	}
}

uint8_t GPIOI2CMaster::readByte(void)
{
	auto t2Clock = SystemClockCounter::UStoClock(1);

	_sda.SetInput();

	uint8_t r;
	for (uint_fast8_t i = 0; i < 8; ++i)
	{
		r <<= 1;
		_scl.SetOutput();
		waitT2(t2Clock);
		if (_sda.IsHigh())
		{
			r |= 0x1;
		}
		_scl.SetInput();
		waitT2(t2Clock);
	}
}

bool GPIOI2CMaster::readAcknowledge(void)
{
	auto t2Clock = SystemClockCounter::UStoClock(1);

	_scl.SetOutput();
	_sda.SetInput();
	waitT2(t2Clock);
	_scl.SetInput();
	auto r = _sda.IsLow();
	waitT2(t2Clock);
	return r;
}
