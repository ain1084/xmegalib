/*
 * XRA1200InputPin.h
 *
 * Created: 2012/10/26 1:01:41
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <stdint.h>
#include "IInputPin.h"

namespace Device
{
	class XRA1200;

	class XRA1200InputPin : public IInputPin
	{
	public:
		XRA1200InputPin(XRA1200& device, uint8_t pinNumber);
		
		virtual bool IsLow(void) const;
		virtual bool IsHigh(void) const;
		
	private:
		XRA1200& _device;
		const uint8_t _pinNumber;
	};
}
