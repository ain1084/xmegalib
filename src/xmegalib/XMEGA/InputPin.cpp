/*
 * Program1.cpp
 *
 * Created: 2011/12/12 2:06:43
 *  Author: Seiji Ainoguchi
 */ 

#include <avr/io.h>
#include "InputPin.h"

using namespace XMEGA;

InputPin::InputPin(volatile PORT_t& port, uint8_t pinNumber, bool isPullup /* = true */)
 : _gpio(port, pinNumber, isPullup ? PORT_OPC_PULLUP_gc : PORT_OPC_TOTEM_gc)
{
	_gpio.SetInput();
}