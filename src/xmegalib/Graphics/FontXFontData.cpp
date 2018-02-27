/*
 * FontXFontData.cpp
 *
 * Created: 2012/05/01 1:12:53
 *  Author: Seiji Ainoguchi
 */ 

#include <string.h>
#include "Utilities/StringUtilities.h"
#include "Device/IMemoryReader.h"
#include "FontXFontData.h"

using namespace Device;
using namespace Graphics;

IFontData* FontXFontData::CreateFont(const Device::IMemoryReader& reader, uint32_t offset)
{
	static const char fontX2[] = { 'F', 'O', 'N', 'T', 'X', '2' };
	char buf[sizeof(fontX2)];
	reader.Read(offset, buf, sizeof(buf));
	if (memcmp(buf, fontX2, sizeof(buf)) != 0)
	{
		return nullptr;
	}
	offset += sizeof(fontX2);
	
	char name[8 + 1];
	name[8] = '\0';
	reader.Read(offset, name, 8);
	offset += 8;
			
	uint_fast8_t width = reader.ReadByte(offset++);
	uint_fast8_t height = reader.ReadByte(offset++);
	CodeType type = static_cast<CodeType>(reader.ReadByte(offset++));
	uint_fast8_t size = (((width + 7) & ~7) >> 3) * height;
	
	PatternExtractor* pExtractor;
	switch (type)
	{
		case Hankaku:
			pExtractor = new HankakuPatternExtractor(reader, offset, size);
			break;
		case Zenkaku:
			pExtractor = new ZenkakuPatternExtractor(reader, offset, size);
			break;
		default:
			return nullptr;
	}			
	return new FontXFontData(pExtractor, type, StringUtilities::StripRightSpace(name), width, height, size);
}

FontXFontData::HankakuPatternExtractor::HankakuPatternExtractor(const Device::IMemoryReader& reader, uint32_t patternOffset, uint_fast8_t patternSize)
 : _reader(reader)
 , _patternOffset(patternOffset)
 , _patternSize(patternSize)
{
}

bool FontXFontData::HankakuPatternExtractor::GetData(uint16_t code, uint8_t* pBuffer) const
{
	if (code >= 0x100)
	{
		return false;
	}
	_reader.Read(_patternOffset + (uint_fast8_t)code * _patternSize, pBuffer, _patternSize);
	return true;
}

FontXFontData::ZenkakuPatternExtractor::ZenkakuPatternExtractor(const Device::IMemoryReader& reader, uint32_t tableCountOffset, uint_fast8_t patternSize)
 : _reader(reader)
 , _patternSize(patternSize)
{
	uint_fast8_t count = _reader.ReadByte(tableCountOffset++);
	_table.Reserve(count);
	do 
	{
		TableItem item;
		_reader.Read(tableCountOffset, &item, sizeof(item));
		tableCountOffset += sizeof(item);
		_table.Add(item);
	} while (--count != 0);
	_patternOffset = tableCountOffset;
}

bool FontXFontData::ZenkakuPatternExtractor::GetData(uint16_t code, uint8_t* pBuffer) const
{
	uint16_t linearCode = 0;
	for (Container::IndexType i = 0; i < _table.GetCount(); ++i)
	{
		const TableItem& item = _table[i];
		if (code >= item.First && code <= item.Last)
		{
			linearCode += code - item.First;
			_reader.Read(_patternOffset + static_cast<uint32_t>(linearCode) * _patternSize, pBuffer, _patternSize);
			return true;			
		}
		else
		{
			linearCode += (item.Last - item.First) + 1;
		}
	}
	return false;
}


FontXFontData::FontXFontData(PatternExtractor* pExtractor, CodeType codeType, const char* pName, uint_fast8_t width, uint_fast8_t height, uint_fast8_t patternSize)
 : _pExtractor(pExtractor)
 , _codeType(codeType)
 , _name(pName)
 , _width(width)
 , _height(height)
 , _patternSize(patternSize)
{

}

BasicString FontXFontData::GetName(void) const
{
	return _name;
}

uint_fast8_t FontXFontData::GetWidth(void) const
{
	return _width;
}

uint_fast8_t FontXFontData::GetHeight(void) const
{
	return _height;
}

uint_fast16_t FontXFontData::GetDataSize(void) const
{
	return _patternSize;
}

bool FontXFontData::GetData(uint16_t code, uint8_t data[], uint_fast16_t length) const
{
	if (_patternSize > length)
	{
		return false;
	}
	return _pExtractor->GetData(code, data);
}

IFontData::CodeType FontXFontData::GetCodeType(void) const
{
	return _codeType;
}