/*
 * DirectoryInfoInfo.h
 *
 * Created: 2012/05/07 1:02:17
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "FileSystem/IDirectoryInfo.h"

namespace Device
{
	class IMemoryReader;
};

namespace FileSystem
{
	namespace IndexTable
	{
		class DirectoryInfo : public IDirectoryInfo
		{
		public:
			DirectoryInfo(const Device::IMemoryReader& reader);
			virtual IDirectoryInfo* Clone(void) const;
			virtual bool IsDirectory(void) const;
			virtual bool IsSystem(void) const;
			virtual bool IsHidden(void) const;
			virtual BasicString GetFullName(void) const;
			virtual BasicString GetName(void) const;
			virtual IFileSystemInfo* Next(void);
			
		private:
			const Device::IMemoryReader& _reader;
			uint32_t _offset;
		};
	}
}
