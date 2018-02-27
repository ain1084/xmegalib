/*
 * MAX9850AudioMixer.cpp
 *
 * Created: 2012/04/30 23:41:23
 *  Author: Seiji Ainoguchi
 */ 

#include "Device/II2CMaster.h"
#include "Audio/ISerialSampleRenderer.h"
#include "MAX9850AudioMixer.h"

using namespace Audio;
using namespace Device;

MAX9850AudioMixer::MAX9850AudioMixer(const ISerialSampleRenderer& serialRenderer, II2CMaster& i2cMaster, uint8_t id)
 : _i2cMaster(i2cMaster)
 , _id(id)
 , _volume(VOLUME_DEFAULT_PERCENT)
{
	//charge-Pump clock
	//11.2896MHz / (2 * NCP) = 667Khz +- 20%
	writeRegister(0x06, 0b00000000);
	writeRegister(0x07, 8);
	
	//digital audio register
	if (serialRenderer.GetFormat() == ISerialSampleRenderer::Format_LeftJustified)
	{
		writeRegister(0x0a, 0b01000000);
	}
	else
	{
		writeRegister(0x0a, 0b00001000);
	}		

	applyVolume();

	//enable	
	writeRegister(0x05, 0b11111101);
}

MAX9850AudioMixer::~MAX9850AudioMixer(void)
{

}

int MAX9850AudioMixer::GetVolume(void) const
{
	return _volume;
}

int MAX9850AudioMixer::SetVolume(int volume)
{
	if (volume < VOLUME_MIN_PERCENT)
	{
		volume = VOLUME_MIN_PERCENT;
	}
	else if (volume > VOLUME_MAX_PERCENT)
	{
		volume = VOLUME_MAX_PERCENT;
	}
	if (_volume != volume)
	{
		_volume = volume;
		applyVolume();
	}	
	return _volume;
}

void MAX9850AudioMixer::applyVolume(void)
{
	uint8_t val = (100 - _volume) * 63 / VOLUME_MAX_PERCENT;
	writeRegister(0x02, val);
}

void MAX9850AudioMixer::writeRegister(uint8_t address, uint8_t data)
{
	uint8_t writeData[2];
	writeData[0] = address;
	writeData[1] = data;
	_i2cMaster.Write(_id, writeData, sizeof(writeData));
	_i2cMaster.Stop();
}
