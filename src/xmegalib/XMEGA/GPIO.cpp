/*
 * GPIO.cpp
 *
 * Created: 2017/09/03 0:21:15
 *  Author: Seiji Ainoguchi
 */ 

#include "Platform.h"
#include "GPIO.h"

using namespace XMEGA;

GPIO::GPIO(volatile PORT_t& port, uint8_t pinNumber, uint8_t pinCtrl)
	: _port(port)
	, _pinNumber(pinNumber)
	, _pinBit(1 << pinNumber)
{
	*(&port.PIN0CTRL + pinNumber) = pinCtrl;
}
