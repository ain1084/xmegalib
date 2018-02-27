/*
 * ISampleGenerator.h
 *
 * Created: 2011/09/03 2:43:24
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <stdint.h>

namespace Audio
{
	struct SampleData;

	class ISampleGenerator
	{
	public:
		virtual ~ISampleGenerator(void) { }

		virtual void Reset(void) = 0;
		virtual bool IsRunning(void) const = 0;
		virtual void Preread(void) = 0;
		virtual unsigned ReadNext(SampleData buffer[], unsigned sampleCount) = 0;
	};
}
