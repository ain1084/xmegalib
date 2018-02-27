/*
 * IDiskIO.h
 *
 * Created: 2011/12/09 0:41:44
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <stdint.h>

namespace FileSystem
{
	namespace FAT
	{
		class IDiskIO
		{
		public:
			virtual ~IDiskIO(void) { }

			virtual uint8_t /* DSTATUS */ Initialize(void) = 0;
			virtual uint8_t /* DSTATUS */ Status(void) = 0;
			virtual int /* DRESULT */ Read(uint8_t* buffer, uint32_t address, uint8_t count) = 0;
			virtual int /* DRESULT */ IoCtl(uint_fast8_t ctrl, void* buffer) = 0;	
			virtual void Pool(void) = 0;
		};
	}
}
