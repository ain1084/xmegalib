#pragma once

#include <stdint.h>
#include "IndexEntry.h"
#include "IBlockSource.h"
#include "Collections/Vector.h"

namespace FileSystem
{
	namespace IndexTable
	{
		class IndexBlockSource : public IBlockSource
		{
		public:
			IndexBlockSource(int indexCount);

			void Add(const char fileName[], uint32_t offset, uint32_t length);

			virtual uint32_t GetLength(void) const;
			virtual int Read(void* buffer, uint32_t offset, int length) const;
 
		private:
			const int _indexCount;
			typedef Collections::Vector<IndexEntry> Container;
			Container _container;

			void appendEndMark(void);
		};
	}
}
  