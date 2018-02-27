/*
 * LM49450AudioMixer.cpp
 *
 * Created: 2012/04/30 23:41:23
 *  Author: Seiji Ainoguchi
 */ 

#include "Audio/ISerialSampleRenderer.h"
#include "II2CMaster.h"
#include "LM49450AudioMixer.h"

using namespace Audio;
using namespace Device;

LM49450AudioMixer::LM49450AudioMixer(const ISerialSampleRenderer& serialRenderer, II2CMaster& i2cMaster, uint8_t id)
: _i2cMaster(i2cMaster)
, _id(id)
, _volume(VOLUME_DEFAULT_PERCENT)
{
	const InitializeParameters* pParam = findParameter(serialRenderer.GetSamplingRate(), serialRenderer.GetMasterClockRatio());
	if (pParam != nullptr)
	{
		writeRegister(0x02, pParam->valueRegister02);
		writeRegister(0x01, pParam->valueRegister01);
		writeRegister(0x00, pParam->valueRegister00 | 0b00000001);
	}
	else
	{
		writeRegister(0x02, 0b00000000);
	}
	applyVolume();
}

LM49450AudioMixer::~LM49450AudioMixer(void)
{

}

int LM49450AudioMixer::GetVolume(void) const
{
	return _volume;
}

int LM49450AudioMixer::SetVolume(int volume)
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

void LM49450AudioMixer::applyVolume(void)
{
	uint8_t val = _volume * 31 / VOLUME_MAX_PERCENT;
	writeRegister(0x07, val);
}

void LM49450AudioMixer::writeRegister(uint8_t address, uint8_t data)
{
	uint8_t writeData[2];
	writeData[0] = address;
	writeData[1] = data;
	_i2cMaster.Write(_id, writeData, sizeof(writeData));
	_i2cMaster.Stop();
}

const LM49450AudioMixer::InitializeParameters* LM49450AudioMixer::findParameter(uint32_t samplingRate, unsigned int masterClockRatio)
{
	static const InitializeParameters parameters[] =
	{
		// rate   xFs  DAC_MODE    MCLK ratio CP_DIV
		{  16000, 256, 0b00100000, 0b00000000,  37 },	// MCLK 4.096MHz
		{  16000, 512, 0b00100000, 0b00000011,  37 },	// MLCK 8.192MHz
		{  32000, 256, 0b00100000, 0b00000000,  49 },	// MCLK 8.192MHz
		{  32000, 512, 0b00100000, 0b00000011,  49 },	// MLCK 16.384MHz  (32.768MHz / 2)
		{  44100, 256, 0b00100000, 0b00000000,  68 },	// MCLK 11.2896MHz
		{  44100, 512, 0b00100000, 0b00000011,  68 },	// MCLK 11.2896MHz (22.5792MHz / 2)
		{  48000, 256, 0b00100000, 0b00000000,  75 },	// MCLK 12.288MHz
		{  48000, 512, 0b00100000, 0b00000011,  75 },	// MCLK 12.288MHz (24.576MHz / 2)
		{  88200, 128, 0b01000000, 0b00000000,  68 },	// MCLK 11.2896MHz 
		{  88200, 256, 0b01000000, 0b00000011,  68 },	// MCLK 11.2896MHz (22.5792MHz / 2)
		{  88200, 512, 0b01000000, 0b00000111,  68 },	// MCLK 11.2896MHz (45.1584MHz / 4)
		{  96000, 128, 0b01000000, 0b00000000,  75 },	// MCLK 12.288MHz
		{  96000, 256, 0b01000000, 0b00000011,  75 },	// MCLK 12.288MHz (24.576MHz / 2)
		{  96000, 512, 0b01000000, 0b00000111,  75 },	// MCLK 12.288MHz (49.152MHz / 4)
		{ 176400, 128, 0b01100000, 0b00000000, 138 },	// MCLK 22.5792MHz
		{ 176400, 256, 0b01100000, 0b00000011, 138 },	// MCLK 22.5792MHz (45.1584MHz / 2)
		{ 192000, 128, 0b01100000, 0b00000000, 150 },	// MCLK 24.576MHz
		{ 192000, 256, 0b01100000, 0b00000011, 150 },	// MCLK 24.576MHz (49.152MHz / 2)
	};
	
	for (uint_fast8_t i = 0; i < sizeof(parameters) / sizeof(parameters[0]); ++i)
	{
		const InitializeParameters& param = parameters[i];
		if (samplingRate == param.samplingRate && masterClockRatio == param.masterClockRatio)
		{
			return &param;
		}
	}
	return nullptr;
}
