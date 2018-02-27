#include "FileBlockSource.h"
#include "FileSystem/IFile.h"

using namespace FileSystem;
using namespace FileSystem::IndexTable;

FileBlockSource::FileBlockSource(FileSystem::IFile* pFile)
 : _pFile(pFile)
{

}

uint32_t FileBlockSource::GetLength(void) const
{
    return _pFile->GetSize();
}

int FileBlockSource::Read(void* buffer, uint32_t offset, int length) const
{
	auto fileLength = GetLength();
    if (offset >= fileLength)
    {
        return 0;
    }        
    if (length + offset > fileLength)
    {
        length = fileLength - offset;
    }
    if (length == 0)
    {
        return 0;
    }
	_pFile->Seek(IFile::Seek_Top, offset);
	return _pFile->Read(buffer, length);
}
