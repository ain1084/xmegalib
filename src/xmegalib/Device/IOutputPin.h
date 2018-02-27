/*
 * IOutputPin.h
 *
 * Created: 2017/09/06 22:00:39
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

namespace Device
{
	class IOutputPin
	{
	public:
		virtual ~IOutputPin(void) {}

		virtual void Low(void) = 0;
		virtual void High(void) = 0;
		virtual IOutputPin& operator=(bool isHigh) = 0;
	};
}
