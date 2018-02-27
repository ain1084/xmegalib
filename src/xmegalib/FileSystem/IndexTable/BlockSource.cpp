#include <string.h>
#include "IBlockSource.h"
#include "BlockSource.h"

using namespace FileSystem::IndexTable;

BlockSource::BlockSource(uint32_t blockSize)
 : _blockSize(blockSize)
 , _nextOffset(0)
{
}

BlockSource::~BlockSource(void)
{
	for (Container::IndexType i = 0; i < _container.GetCount(); ++i)
	{
		delete _container[i];
	}
}

void BlockSource::Add(IBlockSource* pSource)
{
	auto blockAlignedSourceSize = pSource->GetLength();
	blockAlignedSourceSize = (blockAlignedSourceSize + (_blockSize - 1)) & ~(_blockSize - 1);
	_container.Add(new ChildItem(pSource, _nextOffset, blockAlignedSourceSize));
	_nextOffset += blockAlignedSourceSize;
}

uint32_t BlockSource::GetLength(void) const
{
	return _nextOffset;
}

int BlockSource::Read(void* buffer, uint32_t offset, int length) const
{
	auto blockLength = GetLength();
    if (offset >= blockLength)
    {
        return 0;
    }        
    if (length + offset > blockLength)
    {
        length = blockLength - offset;
    }
    if (length == 0)
    {
        return 0;
    }
    int result = length;
    
    auto pDest = static_cast<uint8_t*>(buffer);
	auto blockIndex = findBlockSourceByOffset(offset);
	while (length != 0)
	{
		const auto& item = *_container[blockIndex++];
		auto offsetInBlock = offset - item.AlignedOffset;
		unsigned readLength;
		if (offsetInBlock + length > item.AlignedLength)
		{
			readLength = item.AlignedOffset + item.AlignedLength - offsetInBlock;
		}
		else
		{
			readLength = length;
		}
		auto readed = item.pSource->Read(pDest, offsetInBlock, readLength);
		auto remainLength = readLength - readed;
		memset(pDest + readed, 0xFF, remainLength);
		offset += readLength;
		length -= readLength;
	}
    return result;
}

BlockSource::Container::IndexType BlockSource::findBlockSourceByOffset(uint32_t offset) const
{
	for (Container::IndexType i = 0;; ++i)
	{
		const auto& item = *_container[i];
        if (item.AlignedOffset <= offset && item.AlignedOffset + item.AlignedLength > offset)
        {
            return i;
        }
    }
}
