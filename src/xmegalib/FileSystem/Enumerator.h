/*
 * Enumerator.h
 *
 * Created: 2012/02/04 12:25:58
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <stdint.h>
#include "Collections/Stack.h"
#include "Utilities/ScopedPtr.h"

namespace FileSystem
{
	class IDirectoryInfo;
	class IFileInfo;

	class Enumerator
	{
	public:
		Enumerator(IDirectoryInfo* pStartDirectory, uint_fast8_t maxHierachyLevel = 0);
		~Enumerator(void);
		
		bool Reset(void);
		unsigned FetchCount(void);
		IFileInfo* FetchIndexOf(unsigned index);
		IFileInfo* FetchNext(void);

	private:
		const uint_fast8_t _maxHierachyLevel;
		ScopedPtr<IDirectoryInfo> _pStartDirectory;
		Collections::Stack<IDirectoryInfo*, uint_fast8_t> _directories;
		unsigned _index;

		IDirectoryInfo* getTopDirectory(void) const;
		bool fetchNext(IFileInfo** ppFileInfo);
		void popTopDirectory(void);
		bool isDirectory(const char* name);
		void clearDirectories(void);
	};
}
