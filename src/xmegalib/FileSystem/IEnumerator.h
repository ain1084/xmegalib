/*
 * IEnumerator.h
 *
 * Created: 2012/05/05 23:20:35
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

namespace FileSystem
{
	class IFileInfo;
	
	class IEnumerator
	{
	public:
		virtual bool Reset(void) = 0;
		virtual unsigned FetchCount(void) = 0;
		virtual IFileInfo* FetchIndexOf(unsigned index) = 0;
		virtual IFileInfo* FetchNext(void) = 0;
	};
}
