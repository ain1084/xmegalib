/*
 * Context.cpp
 *
 * Created: 2012/05/15 0:18:13
 *  Author: Seiji Ainoguchi
 */ 

#include "Platform.h"
#include "Device/Display/IGraphicDisplay.h"
#include "IFontData.h"
#include "Font.h"
#include "Context.h"
#include "Utilities/MultiByteString.h"

using namespace Device;
using namespace Device::Display;
using namespace Graphics;

Context::Context(Device::Display::IGraphicDisplay& device)
 : _device(device)
 , _pFont(nullptr)
{
	 
}

IGraphicDisplay& Context::GetDevice(void) const
{
	return _device;
}

void Context::SetFont(Font* pFont)
{
	_pFont = pFont;
}

Font* Context::GetFont(void) const
{
	return _pFont;
}

void Context::Clear(void)
{
	_device.Clear();
}

void Context::DrawLinePattern(uint_fast8_t left, uint_fast8_t top, uint_fast8_t width, uint_fast8_t height, const uint8_t* pData)
{
	_device.DrawLinePattern(left, top, width, height, pData);
}

bool Context::TextOut(int_fast16_t x, int_fast16_t y, const char* pText)
{
	if (_pFont == nullptr)
	{
		return false;
	}

	_buffer.Resize(_pFont->GetDataLength());

	int width = _device.GetWidth();

	char ch;
	uint_fast16_t code;
	while ((ch = *pText++) != '\0')
	{
		if (MultiByteString::IsLeadByte(ch))
		{
			code = ch << 8;
			ch = *pText++;
			if (MultiByteString::Is2ndByte(ch))
			{
				code |= ch;
			}
			else
			{
				pText--;
				continue;
			}
		}
		else
		{
			code = ch;		
		}
		const IFontData* pFontData = _pFont->GetData(code, &_buffer[0], _buffer.GetCount());
		uint_fast8_t fontWidth = pFontData->GetWidth();
		uint_fast8_t fontHeight = pFontData->GetHeight();
		if (pFontData != nullptr)
		{
			if (x + fontWidth > width)
			{
				break;
			}
			_device.DrawMonochromePattern(x, y, fontWidth, fontHeight, &_buffer[0]);
			x += fontWidth;
		}			
	}
	return true;	
}

void Context::SetForegroundColor(uint8_t color)
{
	_device.SetForegroundColor(color);
}

void Context::SetBackgroundColor(uint8_t color)
{
	_device.SetBackgroundColor(color);
}
