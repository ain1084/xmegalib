/*
 * IFileSystemInfo.h
 *
 * Created: 2012/05/07 10:01:33
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

class BasicString;

namespace FileSystem
{
	class IFileSystemInfo
	{
	public:
		virtual ~IFileSystemInfo(void) { }

		virtual bool IsDirectory(void) const = 0;
		virtual bool IsSystem(void) const = 0;
		virtual bool IsHidden(void) const = 0;
		virtual BasicString GetFullName(void) const = 0;
		virtual BasicString GetName(void) const = 0;
	};
}
