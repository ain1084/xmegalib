/*
 * SSD1322GraphicDisplay.cpp
 *
 * Created: 2012/04/28 0:47:09
 *  Author: Seiji Ainoguchi
 */ 

#include "Platform.h"
#include "SSD1322GraphicDisplay.h"


using namespace Device;
using namespace Device::Display;


namespace
{
	const int INVERT_TIMER_PERIOD_MS = 500;
}

SSD1322GraphicDisplay::SSD1322GraphicDisplay(SPIChip& chip, OutputPin& cdPin, OutputPin& resetPin)
 : _impl(chip, cdPin, resetPin)
 , _foregounrdColor(0xFF)
 , _backgroundColor(0x00)
 , _invertTimer(SystemClockCounter::MStoClock(INVERT_TIMER_PERIOD_MS))
 , _displayMode(SSD1322GraphicDisplayImpl::Mode_Normal)
 , _invert100MilliSeconds(0)
{
	
}

void SSD1322GraphicDisplay::Refresh(void)
{
	if (_invertTimer.IsReached())
	{
		_invertTimer.Reset();
		if (++_invert100MilliSeconds >= 30000 / INVERT_TIMER_PERIOD_MS)
		{
			_invert100MilliSeconds = 0;
			_displayMode = (_displayMode == SSD1322GraphicDisplayImpl::Mode_Normal)
				 ? SSD1322GraphicDisplayImpl::Mode_Invert : SSD1322GraphicDisplayImpl::Mode_Normal;
			_impl.SetDisplayMode(_displayMode);
		}
	}	
}

void SSD1322GraphicDisplay::DisplayOn(void)
{
	_impl.DisplayOn();
}
	
void SSD1322GraphicDisplay::DisplayOff(void)
{
	_impl.DisplayOff();
}	

uint_fast16_t SSD1322GraphicDisplay::GetWidth() const
{
	return _impl.GetWidth();
}	

uint_fast16_t SSD1322GraphicDisplay::GetHeight() const
{
	return _impl.GetHeight();
}

void SSD1322GraphicDisplay::DrawLinePattern(int left, int top, int width, int height, const uint8_t* pData)
{
	uint_fast8_t displayPatternDataSizePerLine = static_cast<uint_fast8_t>(width) >> 1;
	uint_fast8_t srcPatternDataSizePerLine = displayPatternDataSizePerLine >> 2;

	uint_fast8_t displayHeight = static_cast<uint_fast8_t>(height);

	uint_fast8_t displayLeft = static_cast<uint_fast8_t>(left) >> 2;
	uint_fast8_t displayRight = (displayPatternDataSizePerLine >> 1) + displayLeft - 1;
	uint_fast8_t displayTop = static_cast<uint_fast8_t>(top);
	uint_fast8_t displayBottom = displayHeight + displayTop - 1;
	unsigned length = displayPatternDataSizePerLine * displayHeight;
	
	uint8_t buf[128];
	extractMonoPattern(buf, srcPatternDataSizePerLine, 1, _foregounrdColor, _backgroundColor, pData);
	
	_impl.DrawPatternBegin(displayLeft, displayTop, displayRight, displayBottom);
	while (displayTop++ <= displayBottom)
	{
		_impl.DrawPatternSend(buf, displayPatternDataSizePerLine);
	}
	_impl.DrawPatternEnd();
}

void SSD1322GraphicDisplay::DrawMonochromePattern(int left, int top, int width, int height, const uint8_t* pData)
{
	uint_fast8_t displayPatternDataSizePerLine = static_cast<uint_fast8_t>(width) >> 1;
	uint_fast8_t srcPatternDataSizePerLine = displayPatternDataSizePerLine >> 2;

	uint_fast8_t displayHeight = static_cast<uint_fast8_t>(height);

	uint_fast8_t displayLeft = static_cast<uint_fast8_t>(left) >> 2;
	uint_fast8_t displayRight = (displayPatternDataSizePerLine >> 1) + displayLeft - 1;
	uint_fast8_t displayTop = static_cast<uint_fast8_t>(top);
	uint_fast8_t displayBottom = displayHeight + displayTop - 1;
	unsigned length = displayPatternDataSizePerLine * displayHeight;
	
	uint8_t buf[128];
	if (length <= sizeof(buf))
	{
		extractMonoPattern(buf, srcPatternDataSizePerLine, displayHeight, _foregounrdColor, _backgroundColor, pData);
		_impl.DrawPattern(displayLeft, displayTop, displayRight, displayBottom, static_cast<const uint8_t*>(buf), length);
	}
	else
	{
		_impl.DrawPatternBegin(displayLeft, displayTop, displayRight, displayBottom);
		while (displayTop++ <= displayBottom)
		{
			extractMonoPattern(buf, srcPatternDataSizePerLine, 1, _foregounrdColor, _backgroundColor, pData);
			pData += srcPatternDataSizePerLine;
			_impl.DrawPatternSend(buf, displayPatternDataSizePerLine);
		}
		_impl.DrawPatternEnd();
	}
}

void SSD1322GraphicDisplay::Clear(void)
{
	_impl.Clear();
}

void SSD1322GraphicDisplay::SetForegroundColor(uint_fast8_t color)
{
	_foregounrdColor = color | color << 4;
}

void SSD1322GraphicDisplay::SetBackgroundColor(uint_fast8_t color)
{
	_backgroundColor = color | color << 4;
}

void SSD1322GraphicDisplay::extractMonoPattern(uint8_t* pBuffer, uint_fast8_t widthCount, uint_fast8_t height, uint_fast8_t foregroundColor, uint_fast8_t backgroundColor, const uint8_t* pSrc)
{
	do 
	{
		uint_fast8_t cx = widthCount;
		do 
		{
			uint_fast8_t data = *pSrc++;
			uint8_t vramData;

			vramData = ((data & 0x80) ? foregroundColor : backgroundColor) & 0xf0;
			vramData |= ((data & 0x40) ? foregroundColor : backgroundColor) & 0x0f;
			*pBuffer++ = vramData;
			vramData = ((data & 0x20) ? foregroundColor : backgroundColor) & 0xf0;
			vramData |= ((data & 0x10) ? foregroundColor : backgroundColor) & 0x0f;
			*pBuffer++ = vramData;
			vramData = ((data & 0x08) ? foregroundColor : backgroundColor) & 0xf0;
			vramData |= ((data & 0x04) ? foregroundColor : backgroundColor) & 0x0f;
			*pBuffer++ = vramData;
			vramData = ((data & 0x02) ? foregroundColor : backgroundColor) & 0xf0;
			vramData |= ((data & 0x01) ? foregroundColor : backgroundColor) & 0x0f;
			*pBuffer++ = vramData;

		} while (--cx != 0);
	} while (--height != 0);
}
