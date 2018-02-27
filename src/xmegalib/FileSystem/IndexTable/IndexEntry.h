/*
 * IndexEntry.h
 *
 * Created: 2012/05/05 23:00:16
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

namespace FileSystem
{
	namespace IndexTable
	{
		struct IndexEntry
		{
			char FileName[11];
			uint32_t Offset;
			uint32_t Length;
			char align[13];
		};
	}
}

