/*
 * SystemClockCounter.h
 *
 * Created: 2011/12/06 0:19:40
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <avr/io.h>

namespace XMEGA
{
	class SystemClockCounter
	{
	public:
		typedef uint16_t ValueType;

		static void Start(void);
		static void Stop(void);
		static ValueType UStoClock(uint32_t us);
		static ValueType MStoClock(uint32_t ms);

		static ValueType GetCounter(void)
		{
			return RTC_CNT;
		}
	
		static uint32_t GetResolution(void)
		{
			return 1000000/32768;
		}
	
		static ValueType GetMaxValue(void)
		{
			return 65535;
		}

	private:
		static ValueType validValue(uint32_t clock);

	};
}
