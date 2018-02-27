/*
 * IInputPin.h
 *
 * Created: 2012/10/26 0:46:59
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

namespace Device
{
	class IInputPin
	{
	public:
		virtual ~IInputPin(void) { }

		virtual bool IsLow(void) const = 0;
		virtual bool IsHigh(void) const = 0;	
	};
}

