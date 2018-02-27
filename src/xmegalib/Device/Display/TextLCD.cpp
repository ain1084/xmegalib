/*
 * TextLCD.cpp
 *
 * Created: 2012/01/10 1:01:22
 *  Author: Seiji Ainoguchi
 */ 

#include "Platform.h"
#include "TextLCD.h"
#include "Device/IOutputPin.h"

using namespace Device;

TextLCD::TextLCD(IOutputPin& db7, IOutputPin& db6, IOutputPin& db5, IOutputPin& db4, IOutputPin& e, IOutputPin& rs)
 : _e(e)
 , _rs(rs)
 , _clock40us(SystemClockCounter::UStoClock(40))
{
	_pdb[0] = &db7;
	_pdb[1] = &db6;
	_pdb[2] = &db5;
	_pdb[3] = &db4;

	_e.Low();
	_rs.Low();

	SystemClockCounter::ValueType clock1msc = SystemClockCounter::MStoClock(1);
	for (uint8_t c = 0; c < 20; c++)
	{
		SystemClockTimer::WaitForReached(clock1msc);
	}					

	write4Bit(0x30);
	SystemClockTimer::WaitForReached(clock1msc);
	write4Bit(0x30);
	SystemClockTimer::WaitForReached(clock1msc);
	write4Bit(0x30);
	SystemClockTimer::WaitForReached(clock1msc);
	write4Bit(0x20);

	writeCommand(0x28);
	writeCommand(0x0c);
	writeCommand(0x06);
	writeCommand(0x01);

	Clear();
}

void TextLCD::Clear(void)
{
	writeCommand(0x01);
	SystemClockTimer::WaitForReached(SystemClockCounter::UStoClock(1800));
	_column = 0;
	_row = 0;
}

void TextLCD::Locate(uint8_t col, uint8_t row)
{
	_column = col;
	_row = row;
}

void TextLCD::Put(const char* p)
{
	char ch;
	while ((ch = *p++) != '\0')
	{
		Put(ch);
	}
}

void TextLCD::Put(char ch)
{
	if (ch == '\n')
	{
		_column = 0;
		++_row;
	}
	else
	{
		character(ch);
		if (++_column >= 16)
		{
			_column = 0;
			+_row++;
		}
	}	
	if (_row >= 2)
	{
		_row = 0;
	}
}

void TextLCD::character(int ch)
{
	writeCommand(0x80 | ((_row * 0x40) + _column));
	writeData(ch);
}

void TextLCD::write4Bit(uint8_t data)
{
	for (uint8_t i = 0; i < 4; ++i)
	{
		if (data & 0x80)
		{
			_pdb[i]->High();		
		}
		else
		{
			_pdb[i]->Low();
		}
		data <<= 1;
	}

	_e.High();
	SystemClockTimer::WaitForReached(_clock40us);
	_e.Low();
	SystemClockTimer::WaitForReached(_clock40us);
}

void TextLCD::write8Bit(uint8_t data)
{
	write4Bit(data);
	write4Bit(data << 4);
}

void TextLCD::writeCommand(uint8_t command)
{
	_rs.Low();
	write8Bit(command);
}

void TextLCD::writeData(uint8_t data)
{
	_rs.High();
	write8Bit(data);
}
