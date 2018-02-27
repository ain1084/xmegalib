/*
 * HeaderInfo.h
 *
 * Created: 2011/12/30 2:10:19
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

namespace Audio
{
	namespace WaveFile
	{
		struct HeaderInfo
		{
			uint16_t formatTag;
			uint16_t channels;
			uint32_t samplesPerSec;
			uint16_t blockAlign;
			uint16_t bitsPerSamples;
			uint32_t dataOffset;
			uint32_t dataCount;
			uint32_t blockCount;
		};
	}
}
