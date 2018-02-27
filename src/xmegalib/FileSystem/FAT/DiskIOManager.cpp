/*
 * DiskIOManager.cpp
 *
 * Created: 2011/12/09 0:57:25
 *  Author: Seiji Ainoguchi
 */ 

#include <string.h>
#include "diskio.h"
#include "IDiskIO.h"
#include "DiskIOManager.h"

using namespace FileSystem::FAT;

DiskIOManager::Container DiskIOManager::_volumes;

int DiskIOManager::Add(IDiskIO& disk)
{
	_volumes.Add(new DiskVolume(disk));
	uint_fast8_t r = _volumes.GetCount() - 1;
	Mount(r);
	return r;
}	

void DiskIOManager::Mount(int diskNumber)
{
	f_mount(diskNumber, &_volumes[diskNumber]->GetFATFS());
}		

uint8_t DiskIOManager::GetStatus(int driveNumber)
{
	return _volumes[driveNumber]->GetDiskIO().Status();
	
}