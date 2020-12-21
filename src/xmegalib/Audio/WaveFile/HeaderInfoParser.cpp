/*
 * HeaderInfoParser.cpp
 *
 * Created: 2011/12/30 2:13:58
 *  Author: Seiji Ainoguchi
 */ 

#include <string.h>
#include "Platform.h"
#include "Collections/Vector.h"
#include "Utilities/BasicString.h"
#include "FileSystem/IFile.h"
#include "Audio/Metadata.h"
#include "HeaderInfo.h"
#include "HeaderInfoParser.h"

namespace
{	
}

using namespace FileSystem;
using namespace Audio;
using namespace Audio::WaveFile;

bool HeaderInfoParser::Parse(IFile& file, HeaderInfo& waveFileInfo, Metadata& metadata)
{
	FourCC fourCC;
	uint32_t chunkSize;
	
	if (!readChunk(file, fourCC, chunkSize))
	{
		return false;
	}
	if (fourCC != FourCC('R', 'I', 'F', 'F'))
	{
		return false;
	}

	if (!checkFourCC(file, FourCC('W', 'A', 'V', 'E' )))
	{
		return false;
	}

	while (readChunk(file, fourCC, chunkSize))
	{
		if (fourCC == FourCC('f', 'm', 't', ' '))
		{
			if (!parseFmtChunk(file, chunkSize, waveFileInfo))
			{
				return false;
			}
		}
		else if (fourCC == FourCC('L', 'I', 'S', 'T'))
		{
			if (!parseListChunk(file, chunkSize, metadata))
			{
				return false;
			}
		}
		else if (fourCC == FourCC('d', 'a', 't', 'a'))
		{
			waveFileInfo.dataCount = chunkSize;
			waveFileInfo.dataOffset = file.GetPosition();
			waveFileInfo.blockCount = waveFileInfo.dataCount / waveFileInfo.blockAlign;
			return true;
		}
		else
		{
			//unknown
			file.Seek(IFile::Seek_Cur, chunkSize);
		}
	}
	return false;
}	

bool HeaderInfoParser::parseListChunk(FileSystem::IFile& file, uint32_t chunkSize, Metadata& metadata)
{
	chunkSize -= 4;
	if (!checkFourCC(file, FourCC('I', 'N', 'F', 'O')))
	{
		// seek to next chunk
		file.Seek(IFile::Seek_Cur, chunkSize);
		return true;
	}

	//INFO
	Collections::Vector<char> buffer;
	while (chunkSize != 0)
	{
		FourCC fourCC;
		uint32_t textChunkSize;
		if (!readChunk(file, fourCC, textChunkSize))
		{
			return false;
		}
		chunkSize -= 8;
		textChunkSize = (textChunkSize + 1) & ~1;
		buffer.Resize(textChunkSize);
		if (file.Read(&buffer[0], textChunkSize) != textChunkSize)
		{
			return false;
		}
		chunkSize -= textChunkSize;
		if (fourCC == FourCC('I', 'A', 'R', 'T'))
		{
			metadata.ArtistName = &buffer[0];
		}
		else if (fourCC == FourCC('I', 'G', 'N', 'R'))
		{
			metadata.Genre = &buffer[0];
		}
		else if (fourCC == FourCC('I', 'P', 'R', 'D'))
		{
			metadata.ProductName = &buffer[0];
		}
		else if (fourCC == FourCC('I', 'N', 'A', 'M'))
		{
			metadata.Name = &buffer[0];
		}
	}
	return true;		
}

bool HeaderInfoParser::parseFmtChunk(IFile& file, uint32_t chunkSize, HeaderInfo& waveFileInfo)
{
	uint32_t curPosition = file.GetPosition();
	if (!readWord(file, waveFileInfo.formatTag))
	{
		return false;
	}
	if (!readWord(file, waveFileInfo.channels))
	{
		return false;
	}
	if (!readDoubleWord(file, waveFileInfo.samplesPerSec))
	{
		return false;
	}
	file.Seek(IFile::Seek_Cur, sizeof(uint32_t));	// skip avg bits
	if (!readWord(file, waveFileInfo.blockAlign))
	{
		return false;
	}
	if (!readWord(file, waveFileInfo.bitsPerSamples))
	{
		return false;
	}

	uint32_t readedSize = file.GetPosition() - curPosition;
	file.Seek(IFile::Seek_Cur, chunkSize - readedSize);
	return true;
}

bool HeaderInfoParser::readFourCC(IFile& file, FourCC& fourCC)
{
	return file.Read(fourCC, sizeof(FourCC)) == sizeof(FourCC);
}

bool HeaderInfoParser::checkFourCC(IFile& file, const FourCC& fourCC)
{
	FourCC buffer;
	if (!readFourCC(file, buffer))
	{
		return false;
	}
	return buffer == fourCC;
}

bool HeaderInfoParser::readChunk(IFile& file, FourCC& fourCC, uint32_t& size)
{
	if (!readFourCC(file, fourCC))
	{
		return false;
	}
	return readDoubleWord(file, size);		
}

bool HeaderInfoParser::readWord(FileSystem::IFile& file, uint16_t& result)
{
	LittleUint16 data;
	if (file.Read(&data, sizeof(data)) != sizeof(data))
	{
		return false;
	}
	result = data;
	return true;
}	

bool HeaderInfoParser::readDoubleWord(FileSystem::IFile& file, uint32_t& result)
{
	LittleUint32 data;
	if (file.Read(&data, sizeof(data)) != sizeof(data))
	{
		return false;
	}
	result = data;
	return true;
}	
