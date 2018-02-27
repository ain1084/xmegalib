/*
 * IDirectoryInfo.h
 *
 * Created: 2012/05/02 23:52:37
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "IFileSystemInfo.h"

namespace FileSystem
{
	class IDirectoryInfo : public IFileSystemInfo
	{
	public:
		virtual IDirectoryInfo* Clone(void) const = 0;
		virtual IFileSystemInfo* Next(void) = 0;
	};

}
