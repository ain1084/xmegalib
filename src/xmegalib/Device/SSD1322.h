/*
 * SSD1322.h
 *
 * Created: 2012/04/21 2:50:14
 *  Author: Seiji Ainoguchi
 */ 

#pragma once


#include <stdint.h>

namespace Device
{
	class ISPIChip;
	class IOutputPin;
	
	class SSD1322
	{
	public:
		SSD1322(ISPIChip& chip, IOutputPin& cdPin, IOutputPin& resetPin);
	
		void Initialize(void);
		void DisplayOn(void);
		void DisplayOff(void);
		void SetAddress(uint8_t left, uint8_t top, uint8_t right, uint8_t bottom);
		void WriteRAM(const uint8_t data[], uint16_t count);
	
	private:
		ISPIChip& _chip;
		IOutputPin& _cdPin;
		IOutputPin& _resetPin;

		void beginCommand(uint8_t command);
		void endCommand(void);
		void sendCommand(uint8_t command);
		void sendCommand(uint8_t command, uint8_t arg);
		void sendCommand(uint8_t command, uint8_t arg1, uint8_t arg2);

	};
}
