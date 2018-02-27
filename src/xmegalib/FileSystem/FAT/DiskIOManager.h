/*
 * DiskIOManager.h
 *
 * Created: 2011/12/09 0:55:36
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <stdint.h>
#include "Collections/Vector.h"
#include "FatFs/ff.h"
#include "DiskVolume.h"

namespace FileSystem
{
	namespace FAT
	{
		class IDiskIO;
		class DiskIOManager
		{
		public:
			static int Add(IDiskIO& disk);

			static void Mount(int driveNumer);

			static uint8_t GetStatus(int driveNumber);

			static IDiskIO& Get(int driveNumber)
			{
				return _volumes[driveNumber]->GetDiskIO();
			}

			static int GetCount(void)
			{
				return _volumes.GetCount();
			}
			
		private:
	
			typedef Collections::Vector<DiskVolume*> Container;
			static Container _volumes;
		};
	}
}
