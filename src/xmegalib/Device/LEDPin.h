/*
 * LEDPin.h
 *
 * Created: 2012/04/25 0:58:27
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "IOutputPin.h"

namespace Device
{
	class LEDPin
	{
	public:
		LEDPin(IOutputPin& pin, bool onIsLow = true)
		 : _pin(pin)
		 , _onIsLow(onIsLow)
		{
		}
		
		void On(void)
		{
			_pin = !_onIsLow;
		}
		
		void Off(void)
		{
			_pin = _onIsLow;
		}
				
	private:
		IOutputPin& _pin;
		const bool _onIsLow;
	};
}
