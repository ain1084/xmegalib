/*
 * CS8406AudioSampleOutput.cpp
 *
 * Created: 2011/11/28 1:11:58
 *  Author: Seiji Ainoguchi
 */ 

#include "Platform.h"
#include "CS8406.h"
#include "Audio/ISerialSampleRenderer.h"
#include "IOutputPin.h"
#include "CS8406AudioMixer.h"

using namespace Audio;
using namespace Device;

CS8406AudioMixer::CS8406AudioMixer(const ISerialSampleRenderer& serialRenderer, CS8406& cs8406, IOutputPin& resetPin)
 : _cs8406(cs8406)
 , _resetPin(resetPin)
{
	SystemClockCounter::ValueType clock20us = SystemClockCounter::UStoClock(20);

	//CS8406 reset
	resetPin.High();
	resetPin.Low();
	SystemClockTimer::WaitForReached(clock20us);
	resetPin.High();

	uint8_t data;

	//Set Serial Audio Input Port Data Format(0x05)
	switch (serialRenderer.GetFormat())
	{
		case Audio::ISerialSampleRenderer::Format_LeftJustified:
			//Left Justified format: Slave Mode / 16-bit resolution
			data = CS8406::Format_SIRES1;
			break;
		case Audio::ISerialSampleRenderer::Format_I2S:
		default:
			//I2S format: Slave Mode / 16-bit resolution / SIDEL / SIRPOL
			data = CS8406::Format_SIRES1|CS8406::Format_SIDEL|CS8406::Format_SILRPOL;
			break;
	}		
	_cs8406.SetSerialInputFormat(data);
			
	//Set Clock Source Control (0x04)
	//RUN / OMCK frequency is 256*Fs
	data = CS8406::Clock_RUN;
	switch (serialRenderer.GetMasterClockRatio())
	{
		case 128:
			data = CS8406::Clock_RUN|CS8406::Clock_CLK1|CS8406::Clock_CLK0;
			break;
		case 384:
			data = CS8406::Clock_RUN|CS8406::Clock_CLK0;
			break;
		case 512:
			data = CS8406::Clock_RUN|CS8406::Clock_CLK1;
			break;
		case 256:
			data = CS8406::Clock_RUN;
			break;
	}
	_cs8406.SetClockSourceControl(data);
	
	//set channel status
	if (serialRenderer.GetSamplingRate() == 32000)
	{
		static const uint8_t channelStatus32K[] =
		{
			0b00100000, 0b01000000, 0b00000000, 0b11000000
		};
		_cs8406.SetChannelStatus(channelStatus32K, 4);
	}
	else if (serialRenderer.GetSamplingRate() == 44100)			
	{
		static const uint8_t channelStatus44K[] =
		{
			0b00100000, 0b01000000, 0b00000000, 0b00000000
		};
		_cs8406.SetChannelStatus(channelStatus44K, 4);				
	}
	else if (serialRenderer.GetSamplingRate() == 48000)
	{
		static const uint8_t channelStatus48K[] =
		{
			0b00100000, 0b01000000, 0b00000000, 0b01000000
		};
		_cs8406.SetChannelStatus(channelStatus48K, 4);				
	}
}

CS8406AudioMixer::~CS8406AudioMixer(void)
{
	//Stop clock
	_cs8406.SetClockSourceControl(0x00);
}

int CS8406AudioMixer::GetVolume(void) const
{
	return VOLUME_MAX_PERCENT;
}

int CS8406AudioMixer::SetVolume(int volume)
{
	return VOLUME_MAX_PERCENT;
}
