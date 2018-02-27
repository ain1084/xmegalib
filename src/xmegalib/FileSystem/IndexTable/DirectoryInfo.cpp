/*
 * Directory.cpp
 *
 * Created: 2012/05/07 1:02:09
 *  Author: Seiji Ainoguchi
 */ 

#include "Platform.h"
#include "Device/IMemoryReader.h"
#include "FileInfo.h"
#include "Utilities/BasicString.h"
#include "DirectoryInfo.h"

using namespace Device;
using namespace FileSystem;
using namespace FileSystem::IndexTable;

DirectoryInfo::DirectoryInfo(const IMemoryReader& reader)
 : _reader(reader)
 , _offset(0)
// , _opened(reader.ReadByte(_offset) != 0xFF)
{
}	

IDirectoryInfo* DirectoryInfo::Clone(void) const
{
	return new DirectoryInfo(_reader);
}

bool DirectoryInfo::IsDirectory(void) const
{
	return true;
}

bool DirectoryInfo::IsSystem(void) const
{
	return false;
}

bool DirectoryInfo::IsHidden(void) const
{
	return false;
}

BasicString DirectoryInfo::GetFullName(void) const
{
	return "";
}

BasicString DirectoryInfo::GetName(void) const
{
	return "";
}	

IFileSystemInfo* DirectoryInfo::Next(void)
{
	if (_reader.ReadByte(_offset) == 0xFF)
	{
		return nullptr;
	}
	IndexEntry entry;
	_reader.Read(_offset, &entry, sizeof(entry));
	_offset += sizeof(entry);
	return new FileInfo(_reader, entry);
}
