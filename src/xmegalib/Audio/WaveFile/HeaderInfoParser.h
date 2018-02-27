/*
 * WaveFileParser.h
 *
 * Created: 2011/12/30 2:12:30
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <stdint.h>

namespace FileSystem
{
	class IFile;
}

namespace Audio
{
	struct Metadata;

	namespace WaveFile
	{
		struct HeaderInfo;

		class HeaderInfoParser
		{
		public:
			static bool Parse(FileSystem::IFile& file, HeaderInfo& waveFileInfo, Metadata& metadata);

		private:
			#pragma pack(1, push)

			union FourCC
			{
			private:
				struct
				{
					char _c1;
					char _c2;
					char _c3;
					char _c4;
				};
				char _data[4];

			public:
				FourCC(char c1, char c2, char c3, char c4)
				: _c1(c1), _c2(c2), _c3(c3), _c4(c4) { }
		
				//		FourCC(const char fourCC[4])
				//		: _data(fourCC) { }

				FourCC()
				{ }

				operator const char*() const
				{
					return _data;
				}

				operator char*()
				{
					return _data;
				}
		
				bool operator==(const FourCC& rhs) const {
					return memcmp(_data, rhs._data, sizeof(_data)) == 0;
				}

				bool operator!=(const FourCC& rhs) const {
					return memcmp(_data, rhs._data, sizeof(_data)) != 0;
				}
			};
	
			struct Chunk
			{
				FourCC fourCC;
				LittleUint32 size;
			};

	#pragma pack(pop)
				
			static bool parseFmtChunk(FileSystem::IFile& file, uint32_t chunkSize, HeaderInfo& waveFileInfo);
			static bool parseListChunk(FileSystem::IFile& file, uint32_t chunkSize, Metadata& metadata);
			static bool readFourCC(FileSystem::IFile& file, FourCC& fourCC);
			static bool readChunk(FileSystem::IFile& file, FourCC& fourCC, uint32_t& size);
			static bool checkFourCC(FileSystem::IFile& file, const FourCC& fourCC);
			static bool checkFourCC(FileSystem::IFile& file, const FourCC& fourCC, uint32_t& size);
			static bool readString(FileSystem::IFile& file, BasicString& result, uint32_t size);

			static bool readWord(FileSystem::IFile& file, uint16_t& result);
			static bool readDoubleWord(FileSystem::IFile& file, uint32_t& result);
		};
	}
}
