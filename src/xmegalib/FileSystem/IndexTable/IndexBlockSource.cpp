#include <string.h>
#include "IndexBlockSource.h"

using namespace FileSystem::IndexTable;

namespace
{
	const char* findFileName(const char* pFullPath)
	{
		const char* p = strlen(pFullPath) + pFullPath;
		while (p != pFullPath && *p != '\\')
		{
			--p;
		}
		return p;
	}
	
	void set83Filename(char buffer[11], const char* pSrc)
	{
		uint_fast8_t i = 0;
		memset(buffer, ' ', 11);
		
		pSrc = findFileName(pSrc);
		
		while (*pSrc != '.' && *pSrc != '\0')
		{
			buffer[i++] = *pSrc++;		
		}
		if (*pSrc++ == '\0')
		{
			return;
		}
		for (uint_fast8_t i = 0; i < 3; ++i)
		{
			char ch = *pSrc++;
			if (ch == '\0')
			{
				break;
			}
			buffer[8 + i] = ch;
		}
	}
}


IndexBlockSource::IndexBlockSource(int indexCount)
 : _indexCount(indexCount)
{
	appendEndMark();
} 

void IndexBlockSource::Add(const char fileName[], uint32_t offset, uint32_t length)
{
	IndexEntry index;
	memset(&index, 0, sizeof(index));
	set83Filename(index.FileName, fileName);
	index.Offset = offset;
	index.Length = length;
	_container[_container.GetCount() - 1] = index;
	appendEndMark();
}

uint32_t IndexBlockSource::GetLength(void) const
{
	return _indexCount * sizeof(IndexEntry);
}

int IndexBlockSource::Read(void* buffer, uint32_t offset, int length) const
{
    auto thisLength = GetLength();
    if (offset >= thisLength)
    {
        return 0;
    }        
    if (length + offset > thisLength)
    {
        length = thisLength - offset;
    }
    if (length == 0)
    {
        return 0;
    }
    auto psrc = reinterpret_cast<const uint8_t*>(&_container[0]);
    memcpy(buffer, psrc + offset, length);
    return length;
}

void IndexBlockSource::appendEndMark(void)
{
	IndexEntry index;
	memset(&index, 0xFF, sizeof(IndexEntry));
	_container.Add(index);
}
