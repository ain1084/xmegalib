/*
 * XRA1200InputPin.cpp
 *
 * Created: 2012/10/26 1:03:48
 *  Author: Seiji Ainoguchi
 */ 

#include "XRA1200InputPin.h"
#include "XRA1200.h"

using namespace Device;

XRA1200InputPin::XRA1200InputPin(XRA1200& device, uint8_t pinNumber)
 : _device(device)
 , _pinNumber(pinNumber)
{
	
}

bool XRA1200InputPin::IsLow(void) const
{
	return (((_device.GetData() >> _pinNumber) & 0x01) == 0) ? true : false;
}

bool XRA1200InputPin::IsHigh(void) const
{
	return (((_device.GetData() >> _pinNumber) & 0x01) == 1) ? true : false;
}