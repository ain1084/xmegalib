/*
 * File.cpp
 *
 * Created: 2012/05/07 1:02:47
 *  Author: Seiji Ainoguchi
 */ 

#include <string.h>
#include <ctype.h>
#include "Device/IMemoryReader.h"
#include "DirectoryInfo.h"
#include "Utilities/ScopedPtr.h"
#include "FileInfo.h"
#include "File.h"


namespace
{
	int Stricmp(const char* r1, const char* r2)
	{
		while (toupper(*r1) == toupper(*r2))
		{
			if (*r1++ == '\0')
			{
				return 0;
			}
			r2++;
		}
		return toupper(*r1) - toupper(*r2);
	}
}

using namespace Device;
using namespace FileSystem::IndexTable;

File::File(const IMemoryReader& reader, uint32_t address, uint32_t length)
 : _reader(reader)
 , _address(address)
 , _length(length)
 , _position(0)
 , _opened(true)
{
	 
}

File::File(const Device::IMemoryReader& reader, const char* pFileName)
 : _reader(reader)
 , _position(0)
 , _opened(false)
{
	ScopedPtr<DirectoryInfo> pDirectoryInfo(new DirectoryInfo(reader));

	for (;;)
	{
		ScopedPtr<IFileSystemInfo> pFileSystemInfo(pDirectoryInfo->Next());
		if (!pFileSystemInfo)
		{
			break;
		}
		if (Stricmp(pFileSystemInfo->GetName().GetBuffer(), pFileName) == 0)
		{
			FileInfo* pFileInfo = static_cast<FileInfo*>(pFileSystemInfo.Get());
			_address = pFileInfo->GetOffset();
			_length = pFileInfo->GetSize();
			_opened = true;
			break;
		}
	}
}

BasicString File::GetName(void) const
{
	return GetFullName();
}

BasicString File::GetFullName(void) const
{
	char buf[8 + 1];
	for (uint_fast8_t i = 0; i < 8; ++i)
	{
		uint_fast8_t nibble = static_cast<uint_fast8_t>(_address >> (i * 4));
		if (nibble >= 10)
		{
			buf[i] = 'A' + (nibble - 10);
		}
		else
		{
			buf[i] = '0' + nibble;
		}
	}
	buf[8] = '\0';
	return buf;
}

unsigned File::Read(void* buffer, unsigned length)
{
	if (_position + length >= _length)
	{
		length = _length - _position;
	}
	_reader.Read(_address + _position, buffer, length);
	_position += length;
	return length;
}

uint32_t File::GetSize(void) const
{
	return _length;
}

bool File::Seek(SeekMethod method, int32_t offset)
{
	switch (method)
	{
	case Seek_Top:
		break;
	case Seek_Cur:
		offset += _position;
		break;
	default:
		return false;	
	}	
	_position = offset;
	return true;
}	

uint32_t File::GetPosition(void) const
{
	return _position;
}
