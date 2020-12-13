/*
 * SPDIFFrameEncoder.h
 *
 * Created: 2016/02/10 0:01:53
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <stddef.h>
#include "Platform.h"

namespace Audio
{
	struct SampleData;
}

namespace XMEGA
{
	struct SPDIFSubFrame;
	class SPDIFFrameEncoder
	{
	public:
		SPDIFFrameEncoder(void);
		void Encode(const Audio::SampleData samples[], SPDIFFrame bmcFrames[], size_t count);

	private:
		uint_fast8_t _frameNumber;
		static const uint8_t _lowNibbleCodedTable[16];
		static const uint8_t _codedTable[256];

		static void encodeSubFrame(uint8_t preamble, int audio, SPDIFSubFrame& bmcSubFrame);
	};
}
