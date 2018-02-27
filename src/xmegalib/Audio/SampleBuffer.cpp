/*
 * SampleBuffer.cpp
 *
 * Created: 2011/12/24 23:28:28
 *  Author: Seiji Ainoguchi
 */ 

#include <string.h>
#include "Platform.h"
#include "SampleData.h"
#include "SampleBuffer.h"

using namespace Audio;

SampleBuffer::SampleBuffer(unsigned size)
: _size(size)
, _pBuffer(new SampleData[size])
, _pBufferEnd(_pBuffer + size)
, _pReadNext(_pBuffer)
, _pWriteNext(_pBuffer)
, _used(0)
{

}

SampleBuffer::~SampleBuffer(void)
{
	delete[] _pBuffer;
}

bool SampleBuffer::IsRunning(void) const
{
	CriticalSection cs;
	return _used != 0;
}

void SampleBuffer::Reset(void)
{
	CriticalSection cs;
	_pReadNext = _pBuffer;
	_pWriteNext = _pBuffer;
	_used = 0;
}

void SampleBuffer::Reset(ISampleGenerator& sourceGenerator)
{
	CriticalSection cs;
	sourceGenerator.Reset();
	Reset();
}

unsigned SampleBuffer::ReadNext(SampleData* pBuffer, unsigned count)
{
	unsigned used;
	{
		CriticalSection cs;	
		used = _used;
	}
	unsigned remainCount = count;
	if (used < remainCount)
	{
		remainCount = used;
		count = used;
	}
	const SampleData* pReadNext = _pReadNext;
	while (remainCount != 0)
	{
		unsigned countTemp;
		const SampleData* pReadNextTemp = pReadNext;
		if (pReadNext + remainCount >= _pBufferEnd)
		{
			countTemp = _pBufferEnd - pReadNext;
			pReadNext = _pBuffer;
		}
		else
		{
			countTemp = remainCount;
			pReadNext += remainCount;
		}
		memcpy(pBuffer, pReadNextTemp, countTemp * sizeof(SampleData));
		pBuffer += countTemp;
		remainCount -= countTemp;
	}
	_pReadNext = pReadNext;
	{
		CriticalSection cs;
		_used -= count;
	}
	return count;
}


void SampleBuffer::Fill(ISampleGenerator& sourceGenerator)
{
	unsigned remainCount;
	{
		CriticalSection cs;
		remainCount = _size - _used;
	}
			
	SampleData* pWriteNext = _pWriteNext;
	unsigned freeCount = remainCount;
	while (remainCount != 0)
	{
		unsigned readedCount;
		if (pWriteNext + remainCount >= _pBufferEnd)
		{
			unsigned readCount = _pBufferEnd - pWriteNext;
			readedCount = sourceGenerator.ReadNext(pWriteNext, readCount);
			if (readedCount == readCount)
			{
				pWriteNext = _pBuffer;
			}
		}
		else
		{
			readedCount = sourceGenerator.ReadNext(pWriteNext, remainCount);
			pWriteNext += readedCount;
		}
		if (readedCount == 0)
		{
			break;
		}
		remainCount -= readedCount;
	}

	_pWriteNext = pWriteNext;
	{
		CriticalSection cs;
		_used += freeCount - remainCount;
	}
}
