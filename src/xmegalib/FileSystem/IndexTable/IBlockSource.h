/*
 * IBlockSource.h
 *
 * Created: 2012/05/05 02:00:00
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <stdint.h>

namespace FileSystem
{
	namespace IndexTable
	{
		class IBlockSource
		{
		public:
			virtual uint32_t GetLength(void) const = 0;
			virtual int Read(void* buffer, uint32_t offset, int length) const = 0;
			virtual ~IBlockSource(void) { }
		};
	}
}
