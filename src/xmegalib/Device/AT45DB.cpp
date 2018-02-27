/*
 * AT45DB.cpp
 *
 * Created: 2012/04/22 23:24:46
 *  Author: Seiji Ainoguchi
 */ 

#include "Platform.h"
#include "ISPIChip.h"
#include "AT45DB.h"

using namespace Device;


const AT45DB::SupportedDevice AT45DB::_supportedDevices[] =
{
	{
		0b00100111, 0b00000001,
		{
			0x400000, 512, 4096, "Atmel AT45DB321D"
		}
	},
	{
		0b00100110, 0b00000000,
		{
			0x200000, 512, 4096, "Atmel AT45DB161D"
		}
	},
	{
		0b00100101, 0b00000000,
		{
			0x100000, 256, 2048, "Atmel AT45DB081D"
		}
	},
	{
		0b00100100, 0b00000000,
		{
			0x080000, 256, 2048, "Atmel AT45DB041D"
		}
	},
	{
		0b00100100, 0b00000000,
		{
			0x040000, 256, 2048, "Atmel AT45DB021D"
		}
	},
	{
		0, 0,
		{
			0, 0, 0
		}
	}
};

IEEPROM* AT45DB::Create(ISPIChip& chip)
{
	uint8_t deviceId1;
	uint8_t deviceId2;
	
	uint8_t manufacturer = readDeviceID(chip, deviceId1, deviceId2);

	static const uint8_t MANUFACTURER_ATMEL = 0x1F;
	if (manufacturer != MANUFACTURER_ATMEL)
	{
		return 0;
	}
	
	const SupportedDevice* psd = _supportedDevices;
	while (psd->deviceId1 != 0)
	{
		if (psd->deviceId1 == deviceId1 && psd->deviceId2 == deviceId2)
		{
			return new AT45DB(chip, psd->deviceInfo);
		}
		psd++;
	}
	return 0;
}

uint8_t AT45DB::readDeviceID(ISPIChip& chip, uint8_t& deviceId1, uint8_t& deviceId2)
{
	chip.Active();
	chip.Write(0x9F);
	uint8_t manufacturer = chip.Read();
	deviceId1 = chip.Read();
	deviceId2 = chip.Read();
	chip.Inactive();
	return manufacturer;
}

void AT45DB::writeCommand(const uint8_t data[], unsigned length)
{
	_chip.Active();
	_chip.Write(data, length);
	_chip.Inactive();
}

AT45DB::AT45DB(ISPIChip& chip, const DeviceInfo& deviceInfo)
: _chip(chip)
, _deviceInfo(deviceInfo)
{
	static const uint8_t powerOf2PageSize[] = { 0x3D, 0x2A, 0x80, 0xA6 };
	writeCommand(powerOf2PageSize, sizeof(powerOf2PageSize));
}

const char* AT45DB::GetName(void) const
{
	return _deviceInfo.pName;
}

uint32_t AT45DB::GetCapacity(void) const
{
	return _deviceInfo.Capacity;
}

uint16_t AT45DB::GetPageSize(void) const
{
	return _deviceInfo.PageSize;
}

uint32_t AT45DB::GetBlockSize(void) const
{
	return _deviceInfo.BlockSize;
}

void AT45DB::EraseChip(void)
{
	for (uint32_t offset = 0; offset < _deviceInfo.Capacity; offset += _deviceInfo.BlockSize)
	{
		EraseBlock(offset);
	}
}

void AT45DB::EraseBlock(uint32_t address)
{
	address &= ~static_cast<uint32_t>(_deviceInfo.BlockSize - 1);
	
	uint8_t eraseBlockCommand[4];
	eraseBlockCommand[0] = 0x50;
	eraseBlockCommand[1] = address >> 16;
	eraseBlockCommand[2] = address >>  8;
	eraseBlockCommand[3] = address;
	writeCommand(eraseBlockCommand, sizeof(eraseBlockCommand));
	WaitReady();
}

bool AT45DB::IsReady(void) const
{
	_chip.Active();
	_chip.Write(0xD7);
	uint8_t status = _chip.Read();
	_chip.Inactive();
	return (status & 0x80) ? true : false;
}

void AT45DB::WaitReady(void) const
{
	while (!IsReady())
		;
}

void AT45DB::beginRead(uint32_t address) const
{
	uint8_t command[] = { 0x03, static_cast<uint8_t>(address >> 16), static_cast<uint8_t>(address >> 8), static_cast<uint8_t>(address) };

	_chip.Active();
	_chip.Write(command, sizeof(command));
}

void AT45DB::Read(uint32_t address, void* buffer, unsigned length) const
{
	CriticalSection cs;

	beginRead(address);
	_chip.Read(buffer, length);
	_chip.Inactive();
}

uint_fast8_t AT45DB::ReadByte(uint_fast32_t address) const
{
	CriticalSection cs;

	beginRead(address);
	uint_fast8_t result = _chip.Read();
	_chip.Inactive();
	return result;
}

void AT45DB::WritePage(uint32_t address)
{
	address &= ~static_cast<uint32_t>(_deviceInfo.PageSize - 1);

	//Buffer to Main Memory Page Program without Built-in Erase
	uint8_t writePageCommand[4];
	writePageCommand[0] = 0x88;
	writePageCommand[1] = address >> 16;
	writePageCommand[2] = address >>  8;
	writePageCommand[3] = address;
	writeCommand(writePageCommand, sizeof(writePageCommand));
	WaitReady();
}

void AT45DB::WritePageBuffer(uint16_t offset, const void* buffer, unsigned length)
{
	//Buffer Write
	uint8_t writeBufferCommand[] = { 0x84, 0x00, static_cast<uint8_t>(offset >> 8), static_cast<uint8_t>(offset) };
	_chip.Active();
	_chip.Write(writeBufferCommand, sizeof(writeBufferCommand));
	_chip.Write(buffer, length);
	_chip.Inactive();
}
