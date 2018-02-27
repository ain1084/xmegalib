/*
 * SSD1322.cpp
 *
 * Created: 2012/04/21 2:50:03
 *  Author: Seiji Ainoguchi
 */ 

#include "SSD1322.h"
#include "Platform.h"
#include "ISPIChip.h"
#include "IOutputPin.h"

using namespace Device;

SSD1322::SSD1322(ISPIChip& chip, IOutputPin& cdPin, IOutputPin& resetPin)
: _chip(chip)
, _cdPin(cdPin)
, _resetPin(resetPin)
{
	Initialize();
}

void SSD1322::SetAddress(uint8_t left, uint8_t top, uint8_t right, uint8_t bottom)
{
	sendCommand(0x15, left, right);
	sendCommand(0x75, top, bottom);
}

void SSD1322::WriteRAM(const uint8_t data[], uint16_t count)
{
	beginCommand(0x5C);
	_chip.Write(data, count);
	endCommand();
}

void SSD1322::DisplayOn(void)
{
	sendCommand(0xAF);
}

void SSD1322::DisplayOff(void)
{
	sendCommand(0xAE);
}

void SSD1322::Initialize(void)
{
	SystemClockTimer wait2msec(SystemClockCounter::MStoClock(2));
	_resetPin.Low();
	wait2msec.WaitForReached();
	_resetPin.High();
	wait2msec.WaitForReached();

	static const uint8_t commands[] =
	{
		//unlock basic commands
		1, 0xFD, 0x12,
		
		//display off
		0, 0xAE,
		
		//display clock
		1, 0xB3, 0x91,

		//multiplex ratio
		1, 0xCA, 0x3F,

		//start line
		1, 0xA1, 0x00,
	
		//set remap
		2, 0xA0, 0x14, 0x11,

		//set GPIO
		1, 0xB5, 0x00,

		//enable internal VDD regulator
		1, 0xAB, 0x01,

		//enable external VSL
		2, 0xB4, 0xA0, 0xFD,

		//set contrast
		1, 0xC1, 0x7f,
			
		//master current
		1, 0xC7, 0x9,

		//Set default Gray scale table
		0, 0xB9,
			
		//Enable Gray scale
		0, 0x00,

		//phase length
		1, 0xB1, 0xE2,

		//display enhancement B
		2, 0xD1, 0x82, 0x20,

		//Pre charge voltage
		1, 0xBB, 0x1F,

		//Pre charge period
		1, 0xB6, 0x08,
					
		//VCOMH
		1, 0xBE, 0x07,
		
		//display mode (all pixel turn on)
		0, 0xA6,
		
		//end mark
		0xff,
	};

	const uint8_t* pCommands = commands;
	while (*pCommands != 0xff)
	{
		uint8_t length = *pCommands++;
		uint8_t cmd    = *pCommands++;
		beginCommand(cmd);
		_chip.Write(pCommands, length);
		pCommands += length;
		endCommand();
	}
 	//display on
	DisplayOn();
}

void SSD1322::beginCommand(uint8_t command)
{
	_cdPin.Low();
	_chip.Active();
	_chip.Write(command);
	_cdPin.High();
}
		
void SSD1322::endCommand(void)
{
	_chip.Inactive();
}

void SSD1322::sendCommand(uint8_t command)
{
	beginCommand(command);
	endCommand();
}

void SSD1322::sendCommand(uint8_t command, uint8_t arg)
{
	beginCommand(command);
	_chip.Write(arg);
	endCommand();
}

void SSD1322::sendCommand(uint8_t command, uint8_t arg1, uint8_t arg2)
{
	beginCommand(command);
	_chip.Write(arg1);
	_chip.Write(arg2);
	endCommand();
}
