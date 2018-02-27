/*
 * LM49450AudioMixer.h
 *
 * Created: 2012/04/30 23:39:17
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
	class II2CMaster;
	
	class LM49450AudioMixer : public Audio::IMixer
	{
	public:
		LM49450AudioMixer(const Audio::ISerialSampleRenderer& serialRenderer, II2CMaster& i2cMaster, uint8_t id);
		virtual int GetVolume(void) const;
		virtual int SetVolume(int volume);
		virtual ~LM49450AudioMixer(void);

	private:
		II2CMaster& _i2cMaster;
		const uint8_t _id;
		int _volume;

		struct InitializeParameters
		{
			uint32_t samplingRate;
			uint16_t masterClockRatio;
			uint8_t valueRegister00;
			uint8_t valueRegister01;
			uint8_t valueRegister02;
		};

		void writeRegister(uint8_t address, uint8_t data);
		void applyVolume(void);

		static const InitializeParameters* findParameter(uint32_t samplingRate, unsigned int masterClockRatio);
	};
}
