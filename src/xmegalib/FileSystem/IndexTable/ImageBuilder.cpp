/*
 * ImageBuilder.cpp
 *
 * Created: 2012/05/05 23:25:11
 *  Author: Seiji Ainoguchi
 */ 

#include "FileSystem/IFile.h"
#include "FileSystem/IFileInfo.h"
#include "FileSystem/IEnumerator.h"
#include "FileSystem/Enumerator.h"
#include "Utilities/ScopedPtr.h"
#include "IndexBlockSource.h"
#include "FileBlockSource.h"
#include "BlockSource.h"
#include "ImageBuilder.h"

using namespace FileSystem;
using namespace FileSystem::IndexTable;

int ImageBuilder::Build(BlockSource& blockSource, Enumerator& enumerator)
{
	enumerator.Reset();
	auto count = enumerator.FetchCount();
	auto pIndexBlockSource = new IndexBlockSource(count);
	blockSource.Add(pIndexBlockSource);
	
	for (;;)
	{
		ScopedPtr<IFileInfo> pFileInfo(enumerator.FetchNext());
		if (!pFileInfo)
		{
			break;
		}		
		auto pFile = pFileInfo->OpenRead();
		if (pFile != nullptr)
		{
			auto pFileBlockSource = new FileBlockSource(pFile);
			auto offset = blockSource.GetLength();
			blockSource.Add(pFileBlockSource);
			auto length = pFileInfo->GetSize();
			pIndexBlockSource->Add(pFileInfo->GetName().GetBuffer(), offset, length);
		}			
	}
	return count;
}
