/*
 * SSD1322GraphicDisplay.h
 *
 * Created: 2012/04/28 0:46:37
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "Device/Display/IGraphicDisplay.h"
#include "SSD1322GraphicDisplayImpl.h"

namespace Device
{
	namespace Display
	{
		class SSD1322GraphicDisplay : public IGraphicDisplay
		{
		public:
			SSD1322GraphicDisplay(SPIChip& chip, OutputPin& cdPin, OutputPin& resetPin);
			virtual void DisplayOn(void);
			virtual void DisplayOff(void);
			virtual void Clear(void);
			virtual void Refresh(void);
			virtual uint_fast16_t GetWidth() const;
			virtual uint_fast16_t GetHeight() const;
			virtual void SetForegroundColor(uint_fast8_t color);
			virtual void SetBackgroundColor(uint_fast8_t color);
			virtual void DrawMonochromePattern(int_fast16_t left, int top, int width, int height, const uint8_t* pData);
			virtual void DrawLinePattern(int left, int top, int width, int height, const uint8_t* pData);

		private:
			SSD1322GraphicDisplayImpl _impl;
			uint8_t _foregounrdColor;
			uint8_t _backgroundColor;
			SystemClockTimer _invertTimer;
			SSD1322GraphicDisplayImpl::DisplayMode _displayMode;
			int _invert100MilliSeconds;

			static void extractMonoPattern(uint8_t* pBuffer, uint_fast8_t width, uint_fast8_t height, uint_fast8_t foregroundColor, uint_fast8_t backgroundColor, const uint8_t* pSrc);
		};
	}
}