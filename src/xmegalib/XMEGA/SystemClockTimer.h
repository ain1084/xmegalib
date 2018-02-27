/*
 * SystemClockTimer.h
 *
 * Created: 2011/12/10 2:14:03
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "SystemClockCounter.h"

namespace XMEGA
{
	class SystemClockTimer
	{
	public:
		SystemClockTimer(SystemClockCounter::ValueType clock);

		void Reset(void);
		bool IsReached(void);
		void WaitForReached(void);
		static void WaitForReached(SystemClockCounter::ValueType clock);

	private:
		const SystemClockCounter::ValueType _clock;
		bool _isReached;
		SystemClockCounter::ValueType _start;
	};
}
