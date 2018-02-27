/*
 * InputPin.h
 *
 * Created: 2011/12/12 2:03:10
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <avr/io.h>

#include <Device/IInputPin.h>
#include "GPIO.h"

namespace XMEGA
{
	class InputPin : public Device::IInputPin
	{
	public:
		InputPin(volatile PORT_t& port, uint8_t pinNumber, bool isPullup = true);

		virtual bool IsLow(void) const
		{
			return _gpio.IsLow();
		}

		virtual bool IsHigh(void) const
		{
			return _gpio.IsHigh();
		}

	private:
		GPIO _gpio;
	};
}
