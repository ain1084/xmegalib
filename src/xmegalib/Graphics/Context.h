/*
 * Context.h
 *
 * Created: 2012/05/15 0:15:16
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "Collections/Vector.h"

namespace Device
{
	namespace Display
	{
		class IGraphicDisplay;
	}
}

namespace Graphics
{
	class Font;

	class Context
	{
	public:
		Context(Device::Display::IGraphicDisplay& device);
		
		void Clear(void);
		void SetForegroundColor(uint8_t color);
		void SetBackgroundColor(uint8_t color);
		void SetFont(Font* pFont);
		Font* GetFont(void) const;
		Device::Display::IGraphicDisplay& GetDevice(void) const;
		bool TextOut(int_fast16_t x, int_fast16_t y, const char* pText);
		void DrawLinePattern(uint_fast8_t left, uint_fast8_t top, uint_fast8_t width, uint_fast8_t height, const uint8_t* pData);

	private:
		Device::Display::IGraphicDisplay& _device;
		Font* _pFont;
		Collections::Vector<uint8_t> _buffer;
	};
}
