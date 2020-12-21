/*
 * WaveSampleReader.cpp
 *
 * Created: 2011/12/31 2:38:23
 *  Author: Seiji Ainoguchi
 */ 

#include <string.h>
#include "Platform.h"
#include "FileSystem/IFile.h"
#include "Audio/SampleData.h"
#include "SampleGenerator.h"

using namespace FileSystem;
using namespace Audio;
using namespace Audio::WaveFile;

SampleGenerator::SampleGenerator(IFile& file, const HeaderInfo& headerInfo)
: _file(file)
, _headerInfo(headerInfo)
, _dataSizeCountdown(0)
, _isDiskError(false)
{

}

uint32_t SampleGenerator::GetTotalSampleCount(void) const
{
	return _headerInfo.blockCount;
}

void SampleGenerator::Reset(void)
{
	_file.Seek(IFile::Seek_Top, _headerInfo.dataOffset);
	uint16_t offsetInSector = static_cast<uint16_t>(_headerInfo.dataOffset & (SECTOR_SIZE - 1));
	if (readNextSector(offsetInSector, _headerInfo.dataCount))
	{
		CriticalSection cs;
		_dataSizeCountdown = _headerInfo.dataCount - _bufferRemain;
	}		
}

void SampleGenerator::Preread(void)
{

}

bool SampleGenerator::IsRunning(void) const
{
	CriticalSection cs;
	return _dataSizeCountdown != 0;
}

unsigned SampleGenerator::ReadNext(SampleData* pBuffer, unsigned requestSampleCount)
{
	//WAVE �f�[�^�� alignment �� word �P�ʁB
	//SampleData (4byte) �̂܂܂ł� SECTOR_SIZE ���E���ׂ��ł��܂�����
	//int16_t �̒P�ʂŏ�������B
	int16_t* pInt16Buffer = reinterpret_cast<int16_t*>(pBuffer);
	requestSampleCount *= 2;
	
	uint_fast32_t dataSizeCountdown;
	{
		CriticalSection cs;
		dataSizeCountdown = _dataSizeCountdown;
	}

	do 
	{
		if (dataSizeCountdown == 0)
		{
			//�f�[�^�I��
			break;
		}
		if (_bufferRemain == 0)
		{
			//�o�b�t�@����ɂȂ����̂ōő�� dataSizeCountdown 
			//(PCM�f�[�^�T�C�Y)���܂œǂݍ���
			if (!readNextSector(0, dataSizeCountdown))
			{
				break;
			}
		}
		uint16_t transferSamples;
		uint16_t remainSamples = _bufferRemain / sizeof(int16_t);
		if (requestSampleCount > remainSamples)
		{
			transferSamples = remainSamples;
		}
		else
		{
			transferSamples = requestSampleCount;
		}

		uint_fast16_t transferSize = transferSamples * sizeof(int16_t);
		memcpy(pInt16Buffer, _pReadNext, transferSize);
		_pReadNext += transferSize;
		_bufferRemain -= transferSize;
		dataSizeCountdown -= transferSize;
		pInt16Buffer += transferSamples;
		requestSampleCount -= transferSamples;
	} while (requestSampleCount != 0);

	{
		CriticalSection cs;
		_dataSizeCountdown = dataSizeCountdown;
	}
	//�������ݏI���ʒu - �������݊J�n�ʒu��߂�l�Ƃ��ĕԂ�
	return reinterpret_cast<SampleData*>(pInt16Buffer) - pBuffer;
}

bool SampleGenerator::readNextSector(uint_fast16_t offsetInSector, uint_fast32_t maxSize)
{
	auto readSize = sizeof(_buffer) - offsetInSector;
	if (readSize > maxSize)
	{
		readSize = maxSize;
	}
	auto readed = _file.Read(_buffer, readSize);
	_bufferRemain = readed;
	_pReadNext = _buffer;
	return readed != 0;
}
