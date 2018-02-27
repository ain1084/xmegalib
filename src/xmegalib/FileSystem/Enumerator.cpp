/*
 * Enumerator.cpp
 *
 * Created: 2012/02/04 12:28:40
 *  Author: Seiji Ainoguchi
 */ 

#include <string.h>
#include "IFileInfo.h"
#include "Enumerator.h"
#include "Utilities/ScopedPtr.h"
#include "Utilities/BasicString.h"
#include "IDirectoryInfo.h"

using namespace FileSystem;

Enumerator::Enumerator(IDirectoryInfo* pStartDirectory, uint_fast8_t maxHierachyLevel /* = 0 */)
: _maxHierachyLevel(maxHierachyLevel + 1)
, _pStartDirectory(pStartDirectory)
{
	Reset();
}

Enumerator::~Enumerator(void)
{
	clearDirectories();
}

void Enumerator::clearDirectories(void)
{
	while (!_directories.IsEmpty())
	{
		delete _directories.GetTop();
		_directories.Pop();
	}
}

IDirectoryInfo* Enumerator::getTopDirectory(void) const
{
	if (_directories.IsEmpty())
	{
		return nullptr;
	}
	return _directories.GetTop();
}

void Enumerator::popTopDirectory(void)
{
	delete _directories.GetTop();
	_directories.Pop();
}

bool Enumerator::Reset(void)
{
	_index = 0;
	clearDirectories();
	_directories.Push(_pStartDirectory->Clone());
	return true;
}

IFileInfo* Enumerator::FetchIndexOf(unsigned index)
{
	if (index < _index)
	{
		Reset();
	}

	IFileInfo* pResult = nullptr;
	do 
	{
		delete pResult;
		pResult = FetchNext();
		if (pResult == nullptr)
		{
			break;
		}
	} while (index != _index++);
	return pResult;
}

unsigned Enumerator::FetchCount(void)
{
	unsigned int count = 0;
	while (fetchNext(nullptr))
	{
		++count;		
	}
	Reset();
	return count;
}

IFileInfo* Enumerator::FetchNext(void)
{
	IFileInfo* result;
	return fetchNext(&result) ? result : nullptr;
}

bool Enumerator::fetchNext(IFileInfo** ppFileInfo)
{
	for (;;)
	{
		IDirectoryInfo* pDir = getTopDirectory();
		if (pDir == nullptr)
		{
			return false;
		}
		for (;;)
		{
			ScopedPtr<IFileSystemInfo> pFileInfo(pDir->Next());
			if (!pFileInfo)
			{
				break;
			}
			if (!pFileInfo->IsDirectory())
			{
				if (ppFileInfo != nullptr)
				{
					*ppFileInfo = static_cast<IFileInfo*>(pFileInfo.Release());
				}
				return true;
			}			
			BasicString name = pFileInfo->GetName();		
			if (name[0] == '.')
			{
				continue;
			}
			if (_directories.GetCount() <= _maxHierachyLevel)
			{
				pDir = static_cast<IDirectoryInfo*>(pFileInfo.Release());
				_directories.Push(pDir);
			}
		}
		popTopDirectory();
	}		
}
