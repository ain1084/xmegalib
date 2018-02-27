/*
 * ISerialSampleRenderer.h
 *
 * Created: 2012/01/04 5:25:30
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "ISampleRenderer.h"

namespace Audio
{
	class ISerialSampleRenderer : public ISampleRenderer
	{
	public:
		enum Format
		{
			Format_LeftJustified,
			Format_I2S
		};

		virtual Format GetFormat(void) const = 0;
		virtual uint16_t GetMasterClockRatio(void) const = 0;
	};
}
