#pragma once

#include <stdint.h>
#include "IBlockSource.h"
#include "Collections/Vector.h"
#include "Utilities/ScopedPtr.h"

namespace FileSystem
{
	namespace IndexTable
	{
		class BlockSource : public IBlockSource
		{
		public:
			BlockSource(uint32_t blockSize);
			virtual ~BlockSource(void);

			void Add(IBlockSource* pSource);

			virtual uint32_t GetLength(void) const;
			virtual int Read(void* buffer, uint32_t offset, int length) const;
    
		private:
			uint32_t _blockSize;
			struct ChildItem
			{
				const ScopedPtr<IBlockSource> pSource;
				const uint32_t AlignedOffset;
				const uint32_t AlignedLength;

				ChildItem(IBlockSource* pSource, uint32_t alignedOffset, uint32_t alignedLength)
				 : pSource(pSource)
				 , AlignedOffset(alignedOffset)
				 , AlignedLength(alignedLength)
				 {
			 
				 }
			};
	
			typedef Collections::Vector<ChildItem*> Container;
			Container _container;
			uint32_t _nextOffset;

			Container::IndexType findBlockSourceByOffset(uint32_t offset) const;
		};
	}
}
