/*
 * CS8406.cpp
 *
 * Created: 2012/08/07 1:38:38
 *  Author: Seiji Ainoguchi
 */ 

#include "II2CMaster.h"
#include "ISPIChip.h"
#include "CS8406.h"

using namespace Device;

CS8406::CS8406(II2CMaster& i2cMaster, uint8_t address)
 : _pPort(new I2C_CS8406Port(i2cMaster, address))
{

}

CS8406::CS8406(ISPIChip& spiChip)
 : _pPort(new SPI_CS8406Port(spiChip))
{
	 
}

CS8406::~CS8406(void)
{
	
}

bool CS8406::SetClockSourceControl(uint8_t data)
{
	return _pPort->Write(0x04, data);
}

bool CS8406::SetChannelStatus(const uint8_t data[], size_t count)
{
	uint8_t registerNumber = 0x20;
	for (size_t i = 0; i < count; ++i)
	{
		if (!_pPort->Write(registerNumber++, data[i]))
		{
			return false;
		}
	}
	while (registerNumber <= 0x37)
	{
		if (!_pPort->Write(registerNumber++, 0x00))
		{
			
			return false;
		}
	}
	return _pPort->Write(0x12, 0b00000000);
}

bool CS8406::GetClockSourceControl(uint8_t& data)
{
	return _pPort->Read(0x04, data);
}

bool CS8406::SetSerialInputFormat(uint8_t data)
{
	return _pPort->Write(0x05, data);
}

bool CS8406::GetSerialInputFormat(uint8_t& data)
{
	return _pPort->Read(0x05, data);
}

bool CS8406::GetIdAndVersion(uint8_t& data)
{
	return _pPort->Read(0x7F, data);
}

CS8406::I2C_CS8406Port::I2C_CS8406Port(II2CMaster& i2cMaster, uint8_t address)
 : _i2cMaster(i2cMaster)
 , _address(address)
{
	
}

CS8406::I2C_CS8406Port::~I2C_CS8406Port(void)
{
	
}

bool CS8406::I2C_CS8406Port::Write(uint8_t address, uint8_t data)
{
	uint8_t buffer[2];
	buffer[0] = address;
	buffer[1] = data;

	return _i2cMaster.WriteRegister(_address, address, data);
}

bool CS8406::I2C_CS8406Port::Read(uint8_t registerAddress, uint8_t& data)
{
	return _i2cMaster.WriteRead(_address, &registerAddress, 1, &data, 1);
}

CS8406::SPI_CS8406Port::SPI_CS8406Port(ISPIChip& spiChip)
 : _spiChip(spiChip)
{
	
}

CS8406::SPI_CS8406Port::~SPI_CS8406Port(void)
{
	
}

bool CS8406::SPI_CS8406Port::Write(uint8_t address, uint8_t data)
{
	uint8_t transmitData[3];
	transmitData[0] = 0b00100000;
	transmitData[1] = address;
	transmitData[2] = data;

	_spiChip.Active();
	_spiChip.Write(transmitData, sizeof(transmitData));
	_spiChip.Inactive();
	return true;
}

bool CS8406::SPI_CS8406Port::Read(uint8_t address, uint8_t& data)
{
	Write(address, data);

	_spiChip.Active();
	_spiChip.Write(0b00100001);
	data = _spiChip.Read();
	_spiChip.Inactive();
	return true;
}
