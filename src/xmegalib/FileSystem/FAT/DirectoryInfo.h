/*
 * DirectoryInfo.h
 *
 * Created: 2011/12/15 3:07:36
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "fatfs/ff.h"
#include "FileSystem/IDirectoryInfo.h"

#include "Utilities/BasicString.h"

namespace FileSystem
{
	class IFileSystemInfo;

	namespace FAT
	{
		class DirectoryInfo : public IDirectoryInfo
		{
		public:
			static DirectoryInfo* Create(const char* pFullName);
		
	
			virtual IDirectoryInfo* Clone(void) const;
			virtual bool IsDirectory(void) const;
			virtual bool IsSystem(void) const;
			virtual bool IsHidden(void) const;
			virtual BasicString GetFullName(void) const;
			virtual BasicString GetName(void) const;
			virtual IFileSystemInfo* Next(void);

		private:
			DirectoryInfo(const DIR& dir, const char* pFullName);

			DIR _dir;
			const BasicString _fullName;
			BYTE _fattrib;
		};
	}			
}
