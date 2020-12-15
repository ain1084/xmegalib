/*
 * SPDIFFrameEncoder.h
 *
 * Created: 2016/02/10 0:02:08
 *  Author: Seiji Ainoguchi
 */ 

#include "Audio/SPDIFFrame.h"
#include "Audio/SampleData.h"
#include "SPDIFFrameEncoder.h"

using namespace XMEGA;
using namespace Audio;

namespace
{
	const uint8_t MAX_FRAME_NUMBER = 192;
}

extern "C" void SPDIFEncodeSubFrame(uint8_t preamble, int audio, SPDIFSubFrame& subFrame);

SPDIFFrameEncoder::SPDIFFrameEncoder(void)
: _frameNumber(MAX_FRAME_NUMBER)
{
	
}

void SPDIFFrameEncoder::Encode(const Audio::SampleData samples[], SPDIFFrame spdifFrames[], size_t sampleCount)
{
	const auto* pSample = samples;
	const auto* pSampleEnd = samples + sampleCount;
	auto* pSpdifFrame = spdifFrames;

	auto frameNumber = _frameNumber;
	while (pSample != pSampleEnd)
	{
		uint8_t preamble;
		if (frameNumber == MAX_FRAME_NUMBER)
		{
			frameNumber = 0;
			preamble = 0b11101000;
		}
		else
		{
			preamble = 0b11100010;
		}
		::SPDIFEncodeSubFrame(preamble, pSample->L, pSpdifFrame->L);
		::SPDIFEncodeSubFrame(0b11100100, pSample->R, pSpdifFrame->R);
		frameNumber++;
		pSpdifFrame++;
		pSample++;
	}
	_frameNumber = frameNumber;
}
