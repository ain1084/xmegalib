/*
 * OutputPin.cpp
 *
 * Created: 2011/11/28 1:32:07
 *  Author: Seiji Ainoguchi
 */ 

#include "Platform.h"
#include <avr/io.h>
#include "OutputPin.h"

using namespace XMEGA;

OutputPin::OutputPin(volatile PORT_t& port, uint8_t pinNumber, bool initialStateIsHigh /*=false*/)
 : _gpio(port, pinNumber, PORT_OPC_TOTEM_gc)
{
	_gpio.SetOutput();
	if (initialStateIsHigh)
	{
		_gpio.SetHigh();
	}
	else
	{
		_gpio.SetLow();
	}
}
