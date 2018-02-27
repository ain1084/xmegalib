/*
 * SystemClockTimer.cpp
 *
 * Created: 2011/12/10 2:17:02
 *  Author: Seiji Ainoguchi
 */ 

#include "SystemClockTimer.h"

using namespace XMEGA;

SystemClockTimer::SystemClockTimer(SystemClockCounter::ValueType clock)
 : _clock(clock)
{
	Reset();
}

void SystemClockTimer::Reset(void)
{
	_start = SystemClockCounter::GetCounter();
	while (_start == SystemClockCounter::GetCounter())
		;
	_isReached = false;
}

bool SystemClockTimer::IsReached(void)
{
	if (!_isReached)
	{
		SystemClockCounter::ValueType count = SystemClockCounter::GetCounter();
		SystemClockCounter::ValueType end = _start + _clock;
		if (_start > end)
		{
			if (count < _start &&  count > end)
			{
				_isReached = true;
			}
		}
		else if (count < _start || count > end)
		{
			_isReached = true;
		}
	}
	return _isReached;	
}

void SystemClockTimer::WaitForReached(void)
{
	while (!IsReached())
		;
}

void SystemClockTimer::WaitForReached(SystemClockCounter::ValueType clock)
{
	SystemClockTimer(clock).WaitForReached();
}
