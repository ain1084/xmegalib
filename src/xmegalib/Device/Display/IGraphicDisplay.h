/*
 * IGraphicDisplay.h
 *
 * Created: 2012/04/28 0:33:33
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "Platform.h"

namespace Device
{
	namespace Display
	{
		class IGraphicDisplay
		{
		public:
			virtual ~IGraphicDisplay(void) { }

			virtual void DisplayOn(void) = 0;
			virtual void DisplayOff(void) = 0;
			virtual void Clear(void) = 0;
			virtual void Refresh(void) = 0;
			virtual uint_fast16_t GetWidth() const = 0;
			virtual uint_fast16_t GetHeight() const = 0;
			virtual void SetForegroundColor(uint_fast8_t color) = 0;
			virtual void SetBackgroundColor(uint_fast8_t color) = 0;
			virtual void DrawMonochromePattern(int left, int top, int width, int height, const uint8_t data[]) = 0;
			virtual void DrawLinePattern(int left, int top, int width, int height, const uint8_t* pData) = 0;
		};
		
	}
}
