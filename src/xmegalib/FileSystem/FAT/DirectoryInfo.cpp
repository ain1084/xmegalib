/*
 * Directory.cpp
 *
 * Created: 2011/12/15 3:09:39
 *  Author: Seiji Ainoguchi
 */ 

#include "FileInfo.h"
#include "Utilities/Path.h"
#include "DirectoryInfo.h"

using namespace FileSystem;
using namespace FileSystem::FAT;

DirectoryInfo* DirectoryInfo::Create(const TCHAR* pFullName)
{
	DIR dir;
	if (f_opendir(&dir, pFullName) != FR_OK)
	{
		return nullptr;
	}
	return new DirectoryInfo(dir, pFullName);
}

DirectoryInfo::DirectoryInfo(const DIR& dir, const char* pFullName)
 : _dir(dir)
 , _fullName(pFullName)
{
	FILINFO fi;
	char lfnBuf[_MAX_LFN + 1];
	fi.lfname = lfnBuf;
	fi.lfsize = sizeof(lfnBuf);
	f_stat(pFullName, &fi);
	_fattrib = fi.fattrib;
}

IDirectoryInfo* DirectoryInfo::Clone(void) const
{
	return Create(_fullName.GetBuffer());
}

bool DirectoryInfo::IsDirectory(void) const
{
	return true;
}

bool DirectoryInfo::IsSystem(void) const
{
	return _fattrib & AM_SYS;
}

bool DirectoryInfo::IsHidden(void) const
{
	return _fattrib & AM_HID;
}

BasicString DirectoryInfo::GetFullName(void) const
{
	return _fullName;
}

BasicString DirectoryInfo::GetName(void) const
{
	return Path::GetName(_fullName.GetBuffer());
}	
	
IFileSystemInfo* DirectoryInfo::Next(void)
{
	FILINFO fileInfo;
	char lfnBuf[_MAX_LFN + 1];
	fileInfo.lfname = lfnBuf;
	fileInfo.lfsize = sizeof(lfnBuf);
	if (f_readdir(&_dir, &fileInfo) != FR_OK)
	{
		return nullptr;
	}

	const char* pName = strlen(fileInfo.lfname) != 0 ? fileInfo.lfname : fileInfo.fname;
	if (strlen(pName) == 0)
	{
		return nullptr;
	}

	if (fileInfo.fattrib & AM_DIR)
	{
		return Create(Path::Combine(_fullName.GetBuffer(), pName).GetBuffer());
	}
	else
	{
		return new FileInfo(_fullName.GetBuffer(), pName, fileInfo.fattrib, fileInfo.fsize);
	}
}
