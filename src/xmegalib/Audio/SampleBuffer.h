/*
 * SampleBuffer.h
 *
 * Created: 2011/12/24 23:22:51
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "ISampleGenerator.h"

namespace Audio
{
	struct SampleData;

	class SampleBuffer
	{
	public:
		SampleBuffer(unsigned size);
		~SampleBuffer(void);

		void Reset(void);
		void Reset(ISampleGenerator& sourceGenerator);
		bool IsRunning(void) const;
		void Fill(ISampleGenerator& sourceGenerator);
		unsigned ReadNext(SampleData buffer[], unsigned count);

	private:
		const unsigned _size;
		SampleData* _pBuffer;
		SampleData* _pBufferEnd;
		const SampleData* _pReadNext;
		SampleData* _pWriteNext;
		unsigned _used;
	};
}
