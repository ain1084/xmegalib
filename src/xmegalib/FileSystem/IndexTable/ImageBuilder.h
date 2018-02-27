/*
 * ImageBuilder.h
 *
 * Created: 2012/05/05 23:18:41
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

namespace FileSystem
{
	class Enumerator;

	namespace IndexTable
	{
		class BlockSource;
		class ImageBuilder
		{
		public:
			static int Build(BlockSource& blockSource, FileSystem::Enumerator& enumerator);
		};
	}
}
