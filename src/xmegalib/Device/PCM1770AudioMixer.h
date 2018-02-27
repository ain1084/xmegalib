/*
 * PCM1770AudioMixer.h
 *
 * Created: 2011/11/28 2:01:28
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "Audio/IMixer.h"
#include "Audio/ISerialSampleRenderer.h"

namespace Device
{
	class ISPIChip;
	class IOutputPin;
	
	class PCM1770AudioMixer : public Audio::IMixer
	{
	public:
		PCM1770AudioMixer(ISPIChip& spiChip, IOutputPin& resetPin, Audio::ISerialSampleRenderer::Format format = Audio::ISerialSampleRenderer::Format_I2S);
		virtual int GetVolume(void) const;
		virtual int SetVolume(int volume);
		virtual ~PCM1770AudioMixer(void);

	private:
		ISPIChip& _spiChip;
		IOutputPin& _resetPin;
		int8_t _volume;

		void sendCommand(const uint8_t* pData, uint8_t length);
		void applyVolume(void);
	};
}
