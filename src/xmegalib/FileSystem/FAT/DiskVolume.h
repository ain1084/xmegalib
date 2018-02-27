/*
 * DiskVolume.h
 *
 * Created: 2012/06/12 2:23:44
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "FatFs/ff.h"

namespace FileSystem
{
	namespace FAT
	{
		class IDiskIO;
		
		class DiskVolume
		{
		public:
			DiskVolume(IDiskIO& diskIO)
				: _diskIO(diskIO) { }
					
			FATFS& GetFATFS(void)
			{
				return _fatFS;
			}

			IDiskIO& GetDiskIO(void)
			{
				return _diskIO;
			}

		private:
			IDiskIO& _diskIO;
			FATFS _fatFS;
		};
	}
}

