/*
 * SampleGenerator.h
 *
 * Created: 2011/12/31 2:38:11
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "Audio/ISampleGenerator.h"
#include "HeaderInfo.h"

namespace FileSystem
{
	class IFile;
}

namespace Audio
{
	namespace WaveFile
	{
		class SampleGenerator : public ISampleGenerator
		{
		public:
			SampleGenerator(FileSystem::IFile& file, const HeaderInfo& headerInfo);

			virtual void Reset(void);
			virtual bool IsRunning(void) const;
			virtual void Preread(void);
			virtual unsigned ReadNext(SampleData buffer[], unsigned sampleCount);
			
			uint32_t GetTotalSampleCount(void) const;
			
			void IsDiskErrorOccurred(void) const;

		private:
			enum
			{
				SECTOR_SIZE = 512
			};
			FileSystem::IFile& _file;
			const HeaderInfo _headerInfo;
			uint_fast32_t _dataSizeCountdown;
			uint8_t* _pReadNext;
			uint_fast16_t _nextReadSize;
			uint_fast16_t _bufferRemain;
			uint8_t _buffer[SECTOR_SIZE];
			bool _isDiskError;

			bool readNextSector(uint16_t offsetInSector, uint_fast32_t dataSizeCount);
		};
	}
}
