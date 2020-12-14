/*
 * CS2200_I2C.cpp
 *
 * Created: 2012/05/19 1:41:00
 *  Author: Seiji Ainoguchi
 */ 

#include <string.h>
#include "II2CMaster.h"
#include "ISPIChip.h"
#include "CS2200.h"
#include "Collections/Vector.h"

using namespace Device;
using namespace Collections;

CS2200::CS2200(II2CMaster& i2cMaster, uint8_t address, uint32_t inputClock)
 : _pPort(new I2CCS2200Port(i2cMaster, address))
 , _inputClock(inputClock)
 , _outputClock(inputClock)
{
}

CS2200::CS2200(ISPIChip& spiChip, uint32_t inputClock)
: _pPort(new SPICS2200Port(spiChip))
, _inputClock(inputClock)
, _outputClock(inputClock)
{
}

CS2200::~CS2200(void)
{
}

void CS2200::SetOutputClock(uint32_t clock)
{
	uint32_t ratio = static_cast<uint32_t>((static_cast<uint64_t>(clock) << 20) / _inputClock);
	_outputClock = clock;
	
	uint8_t writeData[4];

	writeData[0] = 0b0000101;
	_pPort->Write(0x03, writeData, 1);

	writeData[0] = static_cast<uint8_t>(ratio >> 24);
	writeData[1] = static_cast<uint8_t>(ratio >> 16);
	writeData[2] = static_cast<uint8_t>(ratio >>  8);
	writeData[3] = static_cast<uint8_t>(ratio >>  0);
	_pPort->Write(0x86, writeData, 4);

	writeData[0] = 0b00010000;
	_pPort->Write(0x16, writeData, 1);

	writeData[0] = 0b00000000;
	_pPort->Write(0x17, writeData, 1);

	//unleash chip
	writeData[0] = 0b00000001;
	_pPort->Write(0x05, writeData, 1);
}

uint32_t CS2200::GetOutputClock(void) const
{
	return _outputClock;
}

CS2200::I2CCS2200Port::I2CCS2200Port(II2CMaster& i2cMaster, uint8_t address)
 : _i2cMaster(i2cMaster)
 , _address(address)
{
	
}

bool CS2200::I2CCS2200Port::Write(uint8_t map, const uint8_t data[], unsigned count)
{
	Vector<uint8_t> buffer(count + 1);
	buffer[0] = map;
	memcpy(&buffer[1], data, count);
	_i2cMaster.Write(_address, &buffer[0], buffer.GetCount());
	return true;
}

bool CS2200::I2CCS2200Port::Read(uint8_t map, uint8_t data[], unsigned count)
{
	return _i2cMaster.WriteRead(_address, &map, 1, data, count);
}

CS2200::SPICS2200Port::SPICS2200Port(ISPIChip& spiChip)
 : _spiChip(spiChip)
{
	 
}

bool CS2200::SPICS2200Port::Write(uint8_t map, const uint8_t data[], unsigned count)
{
	_spiChip.Active();
	_spiChip.Write(0b10011110);
	_spiChip.Write(data, count);
	_spiChip.Inactive();
	return true;
}

bool CS2200::SPICS2200Port::Read(uint8_t map, uint8_t data[], unsigned count)
{
	return false;
}
