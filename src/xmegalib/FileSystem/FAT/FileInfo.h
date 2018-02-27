/*
 * FileInfo.h
 *
 * Created: 2011/12/15 3:15:35
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "fatfs/ff.h"
#include "FileSystem/IFileInfo.h"
#include "Utilities/BasicString.h"

namespace FileSystem
{
	class IFile;
	
	namespace FAT
	{
		class FileInfo : public IFileInfo
		{
		public:
			FileInfo(const char* pParentName, const char* pName, BYTE fattrib, uint32_t fsize);
			virtual bool IsDirectory(void) const;
			virtual bool IsSystem(void) const;
			virtual bool IsHidden(void) const;
			virtual BasicString GetName(void) const;
			virtual BasicString GetFullName(void) const;
			virtual uint32_t GetSize(void) const;
			virtual IFile* OpenRead(void) const;
			 
		private:
			const BasicString _fullName;
			const BYTE _fattrib;
			const uint32_t _fsize;
		};
	}
}
