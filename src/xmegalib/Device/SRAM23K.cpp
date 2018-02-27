/*
 * SRAM23K.cpp
 *
 * Created: 2011/12/14 2:43:38
 *  Author: Seiji Ainoguchi
 */ 

#include "SRAM23K.h"
#include "ISPIChip.h"

using namespace Device;

SRAM23K::SRAM23K(ISPIChip& spiChip)
 : _spiChip(spiChip)
{
	_spiChip.SetFrequency(20000000);

	//set sequential read/write mode
	_spiChip.Active();
	_spiChip.Write(0x01);
	_spiChip.Write(0x40);
	_spiChip.Inactive();
}

uint_fast32_t SRAM23K::GetCapacity(void) const
{
	return 32768;
}

void SRAM23K::beginTransfer(uint_fast8_t command, uint_fast16_t address) const
{
	_spiChip.Active();
	uint8_t buf[] = { command, static_cast<uint8_t>(address >> 8), static_cast<uint8_t>(address) };
	_spiChip.Write(buf, sizeof(buf));
}

uint_fast8_t SRAM23K::ReadByte(uint_fast32_t address) const
{
	beginTransfer(0x03, address);
	uint_fast8_t result = _spiChip.Read();
	_spiChip.Inactive();
	return result;
}	

void SRAM23K::Read(uint_fast32_t address, void* dest, unsigned length) const
{
	beginTransfer(0x03, address);
	_spiChip.Read(dest, length);
	_spiChip.Inactive();
}	

void SRAM23K::Write(uint_fast16_t address, const void* src, unsigned length)
{
	beginTransfer(0x02, address);
	_spiChip.Write(src, length);
	_spiChip.Inactive();
}	
