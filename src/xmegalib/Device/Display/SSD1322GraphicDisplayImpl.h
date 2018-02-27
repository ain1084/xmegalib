/*
 * SSD1322GraphicDisplayImpl.h
 *
 * Created: 2012/04/28 0:38:06
 *  Author: Seiji Ainoguchi
 */ 

#include "Platform.h"

#pragma once

namespace Device
{
	namespace Display
	{
		class SSD1322GraphicDisplayImpl
		{
		public:
			SSD1322GraphicDisplayImpl(SPIChip& chip, OutputPin& cdPin, OutputPin& resetPin);
	
			void DisplayOn(void);
			void DisplayOff(void);
			void Clear(void);
			uint_fast16_t GetWidth(void) const
			{
				return 256;
			}
			uint_fast16_t GetHeight(void) const
			{
				return 64;
			}
			
			void DrawPattern(uint_fast8_t left, uint_fast8_t top, uint_fast8_t right, uint_fast8_t bottom, const uint8_t data[], unsigned length);
			void DrawPatternBegin(uint_fast8_t left, uint_fast8_t top, uint_fast8_t right, uint_fast8_t bottom);
			void DrawPatternSend(const uint8_t data[], unsigned length);
			void DrawPatternEnd(void);

			enum DisplayMode
			{
				Mode_Normal,
				Mode_Invert
			};

			void SetDisplayMode(DisplayMode mode);
		private:
			SPIChip& _chip;
			OutputPin& _cdPin;
			OutputPin& _resetPin;

			void initialize(void);
			void beginCommand(uint8_t command);
			void endCommand(void);
			void sendCommand(uint8_t command);
			void sendCommand(uint8_t command, uint8_t arg);
			void sendCommand(uint8_t command, uint8_t arg1, uint8_t arg2);
		};
	}
}
