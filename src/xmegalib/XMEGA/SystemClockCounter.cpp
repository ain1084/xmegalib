/*
 * SystemClockCounter.cpp
 *
 * Created: 2011/12/06 0:21:54
 *  Author: Seiji Ainoguchi
 */ 

#include "Platform.h"
#include "SystemClock.h"
#include "SystemClockCounter.h"

using namespace XMEGA;

void SystemClockCounter::Start(void)
{
	SystemClock::GetInstance().EnableOscillator(SystemClock::Oscillator_Internal_32KHz);
	CLK_RTCCTRL = CLK_RTCSRC_RCOSC32_gc | CLK_RTCEN_bm;
	RTC_CTRL = RTC_PRESCALER_DIV1_gc;
}

void SystemClockCounter::Stop(void)
{
	RTC_CTRL = 0;
	CLK_RTCCTRL = 0;
}

SystemClockCounter::ValueType SystemClockCounter::UStoClock(uint32_t us)
{
	return validValue(us * 32768 / 1000000);
}

SystemClockCounter::ValueType SystemClockCounter::MStoClock(uint32_t ms)
{
	return validValue(ms * 32768 / 1000);
}

SystemClockCounter::ValueType SystemClockCounter::validValue(uint32_t clock)
{
	if (clock == 0)
	{
		return 1;
	}
	else if (clock > GetMaxValue())
	{
		return GetMaxValue();
	}
	return static_cast<SystemClockCounter::ValueType>(clock);
}
