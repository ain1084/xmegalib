/*
 * FileInfo.cpp
 *
 * Created: 2012/05/07 0:34:22
 *  Author: Seiji Ainoguchi
 */ 

#include <string.h>
#include "File.h"
#include "Utilities/BasicString.h"
#include "Utilities/Path.h"
#include "FileInfo.h"

using namespace FileSystem;
using namespace FileSystem::FAT;

FileInfo::FileInfo(const char* pParentName, const char* pName, BYTE fattrib, uint32_t fsize)
 : _fullName(Path::Combine(pParentName, pName))
 , _fattrib(fattrib)
 , _fsize(fsize)
{
	 
}

bool FileInfo::IsDirectory(void) const
{
	return false;
}

bool FileInfo::IsSystem(void) const
{
	return _fattrib & AM_SYS;
}

bool FileInfo::IsHidden(void) const
{
	return _fattrib & AM_HID;
}

BasicString FileInfo::GetName(void) const
{
	return Path::GetName(_fullName.GetBuffer());
}

BasicString FileInfo::GetFullName(void) const
{
	return _fullName;
}

uint32_t FileInfo::GetSize(void) const
{
	return _fsize;
}

IFile* FileInfo::OpenRead(void) const
{
	return File::Create(_fullName.GetBuffer());
}
