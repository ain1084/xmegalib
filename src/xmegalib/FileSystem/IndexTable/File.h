/*
 * File.h
 *
 * Created: 2012/05/07 1:02:35
 *  Author: Seiji Ainoguchi
 */ 

#include "FileSystem/IFile.h"

namespace Device
{
	class IMemoryReader;
}

namespace FileSystem
{
	namespace IndexTable
	{
		class File : public IFile
		{
		public:
			File(const Device::IMemoryReader& reader, uint32_t address, uint32_t length);
			File(const Device::IMemoryReader& reader, const char* pFileName);

			const Device::IMemoryReader& GetReader(void) const
			{
				return _reader;
			}

			uint32_t GetAddress(void) const
			{
				return _address;
			}
			
			virtual BasicString GetFullName(void) const;
			virtual BasicString GetName(void) const;
			virtual unsigned Read(void* buffer, unsigned length);
			virtual uint32_t GetSize(void) const;
			virtual bool Seek(SeekMethod method, int32_t offset);
			virtual uint32_t GetPosition(void) const;

		private:
			const Device::IMemoryReader& _reader;
			uint32_t _address;
			uint32_t _length;
			uint32_t _position;
			bool _opened;
		};
	}
}
