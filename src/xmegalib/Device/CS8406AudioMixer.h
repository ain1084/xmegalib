/*
 * CS8406AudioMixer.h
 *
 * Created: 2011/11/28 1:11:44
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "Audio/IMixer.h"

namespace Audio
{
	class ISerialSampleRenderer;
}

namespace Device
{
	class CS8406;
	class IOutputPin;

	class CS8406AudioMixer : public Audio::IMixer
	{
	public:
		CS8406AudioMixer(const Audio::ISerialSampleRenderer& serialRenderer, Device::CS8406& cs8406, IOutputPin& resetPin);
		virtual int GetVolume(void) const;
		virtual int SetVolume(int volume);
		virtual ~CS8406AudioMixer(void);

	private:
		CS8406& _cs8406;
		IOutputPin& _resetPin;
	};
}
