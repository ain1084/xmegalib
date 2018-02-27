/*
 * PCM1770AudioMixier.cpp
 *
 * Created: 2011/11/28 2:01:42
 *  Author: Seiji Ainoguchi
 */ 

#include "Platform.h"
#include "ISPIChip.h"
#include "IOutputPin.h"
#include "PCM1770AudioMixer.h"

namespace
{
	const uint8_t REG03_OVER = 0x80;		// 0: 128fs / 1: 192fs, 256fs, 384fs oversampling
}

using namespace Audio;
using namespace Device;

PCM1770AudioMixer::PCM1770AudioMixer(ISPIChip& spiChip, IOutputPin& resetPin,  ISerialSampleRenderer::Format format)
 : _spiChip(spiChip)
 , _resetPin(resetPin)
 , _volume(50)
{
	
	SystemClockCounter::ValueType clock2ms = SystemClockCounter::MStoClock(2);

	resetPin.Low();
	SystemClockTimer::WaitForReached(clock2ms);
	resetPin.High();
	SystemClockTimer::WaitForReached(clock2ms);


	if (format == ISerialSampleRenderer::Format_LeftJustified)
	{
		//16-24bit left-justified format
		static const uint8_t reg03[] = { 0x03, 0x80 };
		sendCommand(reg03, sizeof(reg03));
	}
	else
	{
		//16-24bit i2s format
		static const uint8_t reg03[] = { 0x03, 0x81 };
		sendCommand(reg03, sizeof(reg03));
	}		

	applyVolume();
}

PCM1770AudioMixer::~PCM1770AudioMixer(void)
{

}

int PCM1770AudioMixer::GetVolume(void) const
{
	return _volume;
}

int PCM1770AudioMixer::SetVolume(int volume)
{
	if (volume < 0)
	{
		volume = 0;
	}
	else if (volume > 100)
	{
		volume = 100;
	}
	if (_volume != volume)
	{
		_volume = volume;
		applyVolume();
	}	
	return _volume;
}

void PCM1770AudioMixer::applyVolume(void)
{
	uint8_t vol = (100 - _volume) * 63 / 100;
	
	uint8_t reg01[] = { 0x01, vol };
	sendCommand(reg01, sizeof(reg01));

	SystemClockTimer::WaitForReached(SystemClockCounter::UStoClock(200));

	uint8_t reg02[] = { 0x02, vol };
	sendCommand(reg02, sizeof(reg02));
}

void PCM1770AudioMixer::sendCommand(const uint8_t* pData, uint8_t length)
{
	_spiChip.Active();
	_spiChip.Write(pData, length);	
	_spiChip.Inactive();
}
