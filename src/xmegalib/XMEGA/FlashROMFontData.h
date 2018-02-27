/*
 * FlashROMFontData.h
 *
 * Created: 2012/05/01 0:36:18
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "Platform.h"
#include "Graphics/IFontData.h"

namespace XMEGA
{
	class FlashROMFontData : public Graphics::IFontData
	{
	public:
		FlashROMFontData(const uint8_t data[], uint8_t width, uint8_t height);
		virtual CodeType GetCodeType(void) const;
		virtual uint_fast8_t GetWidth(void) const;
		virtual uint_fast8_t GetHeight(void) const;
		virtual uint_fast16_t GetDataSize(void) const;
		virtual bool GetData(uint16_t code, uint8_t data[], uint_fast16_t length) const;
		
	private:
		const uint8_t* _pData;
		const uint8_t _width;
		const uint8_t _height;
		const uint8_t _fontSize;
	};
}
