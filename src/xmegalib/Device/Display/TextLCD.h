/*
 * TextLCD.h
 *
 * Created: 2012/01/10 1:01:12
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <stdint.h>

namespace Device
{
	class IOutputPin;

	class TextLCD
	{
	public:
		TextLCD(IOutputPin& db7, IOutputPin& db6, IOutputPin& db5, IOutputPin& db4, IOutputPin& e, IOutputPin& rs);	
		void Clear(void);
		void Put(char ch);
		void Put(const char* p);
		void Locate(uint8_t col, uint8_t row);

	private:
		IOutputPin& _e;
		IOutputPin& _rs;
		const SystemClockCounter::ValueType _clock40us;

		uint8_t _column;
		uint8_t _row;
		IOutputPin* _pdb[4];

		void write4Bit(uint8_t data);
		void write8Bit(uint8_t data);
		void writeCommand(uint8_t command);
		void writeData(uint8_t data);
		void character(int ch);
	};
}
