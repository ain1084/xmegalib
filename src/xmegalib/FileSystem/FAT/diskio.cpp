/*
 * diskio.cpp
 *
 * Created: 2011/12/09 0:47:11
 *  Author: Seiji Ainoguchi
 */ 

#include "DiskIOManager.h"
#include "IDiskIO.h"
#include "diskio.h"

using namespace FileSystem::FAT;

extern "C" DSTATUS disk_initialize(BYTE disk)
{
	if (DiskIOManager::GetCount() < disk)
	{
		return STA_NODISK;
	}

	IDiskIO& diskio = DiskIOManager::Get(disk);
	diskio.Pool();
	return static_cast<DSTATUS>(diskio.Initialize());
}

extern "C" DSTATUS disk_status(BYTE disk)
{
	return static_cast<DSTATUS>(DiskIOManager::Get(disk).Status());
}

extern "C" DRESULT disk_read(BYTE disk, BYTE* buf, DWORD sector, BYTE count)
{
	return static_cast<DRESULT>(DiskIOManager::Get(disk).Read(buf, sector, count));
}

extern "C" DRESULT disk_ioctl(BYTE disk, BYTE index, void* buf)
{
	return static_cast<DRESULT>(DiskIOManager::Get(disk).IoCtl(index, buf));
}
