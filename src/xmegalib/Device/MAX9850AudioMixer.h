/*
 * MAX9850AudioMixer.h
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
	
	class MAX9850AudioMixer : public Audio::IMixer
	{
	public:
		MAX9850AudioMixer(const Audio::ISerialSampleRenderer& serialRenderer, II2CMaster& i2cMaster, uint8_t id);
		virtual int GetVolume(void) const;
		virtual int SetVolume(int volume);
		virtual ~MAX9850AudioMixer(void);

	private:
		II2CMaster& _i2cMaster;
		const uint8_t _id;
		int _volume;

		void writeRegister(uint8_t address, uint8_t data);
		void applyVolume(void);
	};
}
