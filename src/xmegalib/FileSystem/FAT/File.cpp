/*
 * File.cpp
 *
 * Created: 2011/12/15 2:56:51
 *  Author: Seiji Ainoguchi
 */ 

#include "File.h"
#include "Utilities/Path.h"

using namespace FileSystem;
using namespace FileSystem::FAT;

IFile* File::Create(const char* pFileName)
{
	FIL file;
	if (f_open(&file, pFileName, FA_READ) != FR_OK)
	{
		return nullptr;
	}		
	return new File(file, pFileName);
}

File::File(const FIL& file, const char* fileName)
 : _file(file)
 , _fullName(fileName)
{

}

File::~File(void)
{
	f_close(&_file);
}

BasicString File::GetName(void) const
{
	return Path::GetName(_fullName.GetBuffer());
}

BasicString File::GetFullName(void) const
{
	return _fullName;
}

unsigned File::Read(void* buffer, unsigned length)
{
	UINT fatfsLength = length;
	UINT fatfsReaded;
	FRESULT fr = f_read(&_file, buffer, fatfsLength, &fatfsReaded);
	if (fr != FR_OK)
	{
		return 0;
	}
	return fatfsReaded;
}

uint32_t File::GetSize(void) const
{
	return f_size(&_file);
}

bool File::Seek(SeekMethod method, int32_t offset)
{
	switch (method)
	{
	case Seek_Top:
		break;
	case Seek_Cur:
		offset += f_tell(&_file);
		break;
	default:
		return false;	
	}	
	if (f_lseek(&_file, offset) != FR_OK)
	{
		return false;
	}
	if (f_tell(&_file) != static_cast<uint32_t>(offset))
	{
		return false;
	}
	return true;
}	

uint32_t File::GetPosition(void) const
{
	return f_tell(&_file);
}
