/*
 * IFileInfo.h
 *
 * Created: 2012/05/07 0:16:37
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <stdint.h>
#include "IFileSystemInfo.h"

namespace FileSystem
{
	class IFile;
	
	class IFileInfo : public IFileSystemInfo
	{
	public:
		virtual ~IFileInfo(void) { }
		virtual uint32_t GetSize(void) const = 0;
		virtual IFile* OpenRead(void) const = 0;
	};
}
