#pragma once

#include "IBlockSource.h"
#include "Utilities/ScopedPtr.h"
#include "Collections/Vector.h"

namespace FileSystem
{
	class IFile;

	namespace IndexTable
	{
		class FileBlockSource : public IBlockSource
		{
		public:
			FileBlockSource(FileSystem::IFile* pFile);
			virtual uint32_t GetLength(void) const;
			virtual int Read(void* buffer, uint32_t offset, int length) const;

		private:
			ScopedPtr<FileSystem::IFile> const _pFile;
		};
	}
}

    