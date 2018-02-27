/*
 * IFontData.h
 *
 * Created: 2012/05/01 0:32:07
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <stdint.h>
#include "Utilities/BasicString.h"

namespace Graphics
{
	class IFontData
	{
	public:
		enum CodeType
		{
			Hankaku,
			Zenkaku
		};

		virtual ~IFontData(void) { }
		
		virtual BasicString GetName(void) const = 0;
		virtual CodeType GetCodeType(void) const = 0;
		virtual uint_fast8_t GetWidth(void) const = 0;
		virtual uint_fast8_t GetHeight(void) const = 0;
		virtual uint_fast16_t GetDataSize(void) const = 0;
		virtual bool GetData(uint16_t code, uint8_t data[], uint_fast16_t length) const = 0;
	};
}
