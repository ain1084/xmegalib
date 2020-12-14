/*
 * XRA1200.cpp
 *
 * Created: 2012/10/26 0:58:21
 *  Author: Seiji Ainoguchi
 */ 

#include "II2CMaster.h"
#include "IInputPin.h"
#include "XRA1200.h"

using namespace Device;

XRA1200::XRA1200(II2CMaster& i2cMaster, uint8_t address, IInputPin& intPin)
 : _i2cMaster(i2cMaster)
 , _address(address)
, _lastData(0xFF)
 , _pIntPin(&intPin)
{
	 
}

XRA1200::XRA1200(II2CMaster& i2cMaster, uint8_t address)
: _i2cMaster(i2cMaster)
, _address(address)
, _lastData(0xFF)
, _pIntPin(nullptr)
{
	
}


void XRA1200::SetDirection(uint8_t inputBitMasks)
{
	write(GPIOConfiguration, inputBitMasks);

	//enable rising/falling edge interrupt
	write(InputRisingEdgeInterruptEnable, inputBitMasks);
	write(InputFallingEdgeInterruptEnable, inputBitMasks);
	write(InputInterruptEnable, inputBitMasks);
}	

uint8_t XRA1200::GetDirection(void) const
{
	return read(GPIOConfiguration);
}

void XRA1200::SetPullup(uint8_t bitMasks)
{
	write(InputInternalPullupRegister, bitMasks);	
}

uint8_t XRA1200::GetPullup(void) const
{
	return read(InputInternalPullupRegister);
}

void XRA1200::SetData(uint8_t data)
{
	write(GPIOStatus, data);
	_lastData = data;
}

uint8_t XRA1200::GetData(void) const
{
	if (_pIntPin != nullptr && _pIntPin->IsHigh())
	{
		return _lastData;
	}
	_lastData = read(GPIOStatus);
	return _lastData;
}

uint8_t XRA1200::read(Register regNumber) const
{
	uint8_t data;
	uint8_t reg = static_cast<uint8_t>(regNumber);
	_i2cMaster.WriteRead(_address, &reg, 1, &data, 1);
	return data;
}

void XRA1200::write(Register regNumber, uint8_t value)
{
	_i2cMaster.WriteRegister(_address, regNumber, value);
}