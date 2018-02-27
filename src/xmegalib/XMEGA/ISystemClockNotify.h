/*
 * ISystemClockNotify.h
 *
 * Created: 2012/04/30 1:02:26
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "SystemClock.h"

namespace XMEGA
{
	class ISystemClockNotify
	{
	public:
		virtual ~ISystemClockNotify(void) { }
		virtual void OnPeripheralClockChanged(uint32_t newClock) = 0;
	};
}
