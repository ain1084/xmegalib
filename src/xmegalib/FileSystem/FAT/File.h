/*
 * File.h
 *
 * Created: 2011/12/15 2:54:41
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <stdint.h>
#include "FileSystem/IFile.h"
#include "FatFs/ff.h"

namespace FileSystem
{
	namespace FAT
	{
		class File : public IFile
		{
		public:
			static IFile* Create(const char* pFileName);

			File(const FIL& _file, const TCHAR* fileName);
			virtual ~File(void);

			virtual BasicString GetFullName(void) const;
			virtual BasicString GetName(void) const;
			virtual unsigned Read(void* buffer, unsigned length);
			virtual uint32_t GetSize(void) const;
			virtual bool Seek(SeekMethod method, int32_t offset);
			virtual uint32_t GetPosition(void) const;

		private:
			FIL _file;
			const BasicString _fullName;
		};
	}
}
