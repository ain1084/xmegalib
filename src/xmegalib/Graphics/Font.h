/*
 * Font.h
 *
 * Created: 2012/05/13 1:57:38
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <stdint.h>
#include "Collections/Vector.h"

namespace Graphics
{
	class IFontData;

	class Font
	{
	public:
		Font(IFontData* pHananku, IFontData* pZenkaku);

		uint_fast8_t GetWidth(void) const
		{
			return _width;
		}

		uint_fast8_t GetHeight(void) const
		{
			return _height;
		}

		unsigned GetDataLength(void) const
		{
			return _length;
		}

		const IFontData* GetData(uint16_t code, uint8_t data[], uint_fast16_t length) const;

	private:
		IFontData* _pHankaku;
		IFontData* _pZenkaku;
		uint_fast8_t _width;
		uint_fast8_t _height;
		unsigned _length;
	};
}
