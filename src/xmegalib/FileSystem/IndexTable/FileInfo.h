/*
 * FileInfo.h
 *
 * Created: 2012/05/07 2:18:22
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "FileSystem/IFileInfo.h"
#include "IndexEntry.h"
#include "Utilities/BasicString.h"

namespace Device
{
	class IMemoryReader;
}

namespace FileSystem
{
	class IFile;
	
	namespace IndexTable
	{
		class FileInfo : public IFileInfo
		{
		public:
			FileInfo(const Device::IMemoryReader& reader, const IndexEntry& indexEntry);

			uint32_t GetOffset(void) const
			{
				return _indexEntry.Offset;
			}

			virtual bool IsDirectory(void) const;
			virtual bool IsSystem(void) const;
			virtual bool IsHidden(void) const;
			virtual BasicString GetName(void) const;
			virtual BasicString GetFullName(void) const;
			virtual uint32_t GetSize(void) const;
			virtual IFile* OpenRead(void) const;
			 
		private:
			const Device::IMemoryReader& _reader;
			const IndexEntry _indexEntry;
			const BasicString _name;
		};
	}
}
