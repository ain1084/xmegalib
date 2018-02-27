/*
 * FlashROMFontData.cpp
 *
 * Created: 2012/05/01 0:44:37
 *  Author: Seiji Ainoguchi
 */ 

#include <avr/pgmspace.h>
#include "FlashROMFontData.h"

using namespace Graphics;
using namespace XMEGA;

FlashROMFontData::FlashROMFontData(const uint8_t data[], uint8_t width, uint8_t height)
: _pData(data)
, _width(width)
, _height(height)
, _fontSize((width >> 3) * height)
{
	
}

uint_fast8_t FlashROMFontData::GetWidth(void) const
{
	return _width;
}

uint_fast8_t FlashROMFontData::GetHeight(void) const
{
	return _height;
}

uint_fast16_t FlashROMFontData::GetDataSize(void) const
{
	return _fontSize;
}

IFontData::CodeType FlashROMFontData::GetCodeType(void) const
{
	return Hankaku;
}

bool FlashROMFontData::GetData(uint16_t code, uint8_t data[], uint_fast16_t length) const
{
	if (code > 0xFF)
	{
		return false;
	}
	if (_fontSize > length)
	{
		return false;
	}
	memcpy_P(data, _pData + code * _fontSize, _fontSize);
	return true;
}
