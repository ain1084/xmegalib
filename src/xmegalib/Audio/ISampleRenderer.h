/*
 * ISampleRenderer.h
 *
 * Created: 2011/09/03 2:44:20
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <stdint.h>

namespace Audio
{
	class ISampleGenerator;

	class ISampleRenderer
	{
	public:
		virtual ~ISampleRenderer(void) { }	

		virtual void Start(ISampleGenerator& generator) = 0;
		virtual void Stop(void) = 0;
		virtual bool IsRunning(void) const = 0;
		virtual void Pause(void) = 0;
		virtual bool IsPaused(void) const = 0;
		virtual uint32_t GetSampleCount(void) const = 0;
		virtual uint32_t GetSamplingRate(void) const = 0;
		virtual void ResetPeakLevel(unsigned& left, unsigned& right) = 0;
	};
}
