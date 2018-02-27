/*
 * IFile.h
 *
 * Created: 2012/05/02 23:52:28
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <stdint.h>
#include "Utilities/BasicString.h"

namespace FileSystem
{
	class IFile
	{
	public:
		enum SeekMethod
		{
			Seek_Top,
			Seek_Cur,		
		};

		virtual ~IFile(void) { }

		virtual BasicString GetFullName(void) const = 0;
		virtual BasicString GetName(void) const = 0;
		virtual unsigned Read(void* buffer, unsigned length) = 0;
		virtual uint32_t GetSize(void) const = 0;
		virtual bool Seek(SeekMethod method, int32_t offset) = 0;
		virtual uint32_t GetPosition(void) const = 0;
	};
}
