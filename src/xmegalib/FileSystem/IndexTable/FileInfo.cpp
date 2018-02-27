/*
 * FileInfo.cpp
 *
 * Created: 2012/05/07 2:18:31
 *  Author: Seiji Ainoguchi
 */ 

#include "File.h"
#include "FileInfo.h"
#include "Utilities/BasicString.h"
#include "IndexEntry.h"

using namespace Device;
using namespace FileSystem;
using namespace FileSystem::IndexTable;

namespace
{
	BasicString convertName(const char fileName[11])
	{
		char buf[12];
		uint_fast8_t j = 0;
		uint_fast8_t i = 0;
		char c;
		for (j = 0; j < 8; j++)
		{
			c = fileName[j];
			if (c == ' ')
			{
				break;
			}
			buf[i++] = c;
		}
		if (fileName[8] != ' ')
		{
			buf[i++] = '.';
			for (j = 0; j < 3; j++)
			{
				c = fileName[j + 8];
				if (c == ' ')
				{
					break;
				}
				buf[i++] = c;
			}
		}
		buf[i++] = '\0';
		return buf;
	}
}

FileInfo::FileInfo(const IMemoryReader& reader, const IndexEntry& indexEntry)
 : _reader(reader)
 , _indexEntry(indexEntry)
 , _name(convertName(indexEntry.FileName))
{
	 
}

bool FileInfo::IsDirectory(void) const
{
	return false;
}

bool FileInfo::IsSystem(void) const
{
	return false;
}

bool FileInfo::IsHidden(void) const
{
	return false;
}

BasicString FileInfo::GetName(void) const
{
	return _name;
}

BasicString FileInfo::GetFullName(void) const
{
	return _name;
}

uint32_t FileInfo::GetSize(void) const
{
	return _indexEntry.Length;
}

IFile* FileInfo::OpenRead(void) const
{
	return new File(_reader, _indexEntry.Offset, _indexEntry.Length);
}