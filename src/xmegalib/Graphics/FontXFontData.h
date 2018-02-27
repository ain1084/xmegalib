/*
 * FontXFontData.h
 *
 * Created: 2012/05/01 1:11:18
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "Platform.h"
#include "Utilities/ScopedPtr.h"
#include "Collections/Vector.h"
#include "IFontData.h"

namespace Device
{
	class IMemoryReader;
}

namespace Graphics
{
	class FontXFontData : public IFontData
	{
	public:
		static IFontData* CreateFont(const Device::IMemoryReader& reader, uint32_t startAddress);
	
		virtual BasicString GetName(void) const;
		virtual CodeType GetCodeType(void) const;
		virtual uint_fast8_t GetWidth(void) const;
		virtual uint_fast8_t GetHeight(void) const;
		virtual uint_fast16_t GetDataSize(void) const;
		virtual bool GetData(uint16_t code, uint8_t data[], uint_fast16_t length) const;
		
	private:
		class PatternExtractor
		{
		public:
			virtual ~PatternExtractor(void) { }
			
			virtual bool GetData(uint16_t code, uint8_t* pBuffer) const = 0;	
		};

		class HankakuPatternExtractor : public PatternExtractor
		{
		public:
			HankakuPatternExtractor(const Device::IMemoryReader& reader, uint32_t patternOffset, uint_fast8_t patternSize);	
			virtual bool GetData(uint16_t code, uint8_t* pBuffer) const;
		private:
			const Device::IMemoryReader& _reader;
			const uint32_t _patternOffset;
			const uint_fast8_t _patternSize;
		};

		class ZenkakuPatternExtractor : public PatternExtractor
		{
		public:
			ZenkakuPatternExtractor(const Device::IMemoryReader& reader, uint32_t tableCountOffset, uint_fast8_t patternSize);
			virtual bool GetData(uint16_t code, uint8_t* pBuffer) const;	
		private:
			const Device::IMemoryReader& _reader;
			const uint_fast8_t _patternSize;

			#pragma pack(push, 1)
			struct TableItem
			{
				LittleUint16 First;
				LittleUint16 Last;
			};
			
			#pragma pack(pop)

			typedef Collections::Vector<TableItem, uint_fast8_t> Container;
			Container _table;
			uint32_t _patternOffset;
		};

		FontXFontData(PatternExtractor* pExtractor, CodeType codeType, const char* pName, uint_fast8_t width, uint_fast8_t height, uint_fast8_t patternSize);

		ScopedPtr<PatternExtractor> _pExtractor;
		const CodeType _codeType;
		const BasicString _name; 
		const uint_fast8_t _width;
		const uint_fast8_t _height;
		const uint_fast8_t _patternSize;
	};
}
