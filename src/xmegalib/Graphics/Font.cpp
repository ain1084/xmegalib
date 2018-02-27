/*
 * Font.cpp
 *
 * Created: 2012/05/13 2:12:57
 *  Author: Seiji Ainoguchi
 */ 

#include "IFontData.h"
#include "Utilities/MultiByteString.h"
#include "Font.h"

using namespace Graphics;


Font::Font(IFontData* pHankaku, IFontData* pZenkaku)
 : _pHankaku(pHankaku)
 , _pZenkaku(pZenkaku)
 , _width(0)
 , _height(0)
 , _length(0)
{
	if (_pHankaku != nullptr)
	{
		_length = pHankaku->GetDataSize();
		_width = pHankaku->GetWidth();
		_height = pHankaku->GetHeight();
	}
	if (pZenkaku != nullptr)
	{
		if (_length < pZenkaku->GetDataSize())
		{
			_length = pZenkaku->GetDataSize();
		}
		_width = pZenkaku->GetWidth() >> 1;
		_height = pZenkaku->GetHeight();
	}
}

const IFontData* Font::GetData(uint16_t code, uint8_t data[], uint_fast16_t length) const
{
	IFontData* pFontData;
	if (MultiByteString::IsLeadByte(code >> 8))
	{
		if (!MultiByteString::Is2ndByte(code))
		{
			return nullptr;
		}
		pFontData = _pZenkaku;
	}
	else
	{
		pFontData = _pHankaku;
	}
	if (pFontData == nullptr || !pFontData->GetData(code, data, length))
	{
		return nullptr;
	}
	return pFontData;
}