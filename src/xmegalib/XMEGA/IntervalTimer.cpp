/*
 * IntervalTimer.cpp
 *
 * Created: 2012/06/09 3:15:54
 *  Author: Seiji Ainoguchi
 */ 

#include "Platform.h"
#include <avr/pgmspace.h>
#include "SystemClock.h"
#include "InterruptController.h"
#include "Device/IIntervalTimerNotify.h"
#include "IntervalTimer.h"


using namespace XMEGA;
using namespace Device;

namespace
{
	template<typename T>
	struct TimerToInterruptCause
	{
		T* pTimer;
		InterruptController::Cause cause;
	};
	
	template<typename T>
	InterruptController::Cause findCause(const TimerToInterruptCause<T> timerToInterruptVector[], volatile T& timer)
	{
		for (uint_fast8_t i = 0;; ++i)
		{
			TimerToInterruptCause<T> tm;
			memcpy_PF(&tm, reinterpret_cast<uint_farptr_t>(&timerToInterruptVector[i]), sizeof(tm));
			if (tm.pTimer == &timer)
			{
				return tm.cause;
			}
		}
	}

	const PROGMEM TimerToInterruptCause<TC0_t> timerToCause0[] =
	{
		{ &TCC0, InterruptController::Cause::TCC0_Overflow },
		#if defined(TCD0)
		{ &TCD0, InterruptController::Cause::TCD0_Overflow },
		#endif
		#if defined(TCE0)
		{ &TCE0, InterruptController::Cause::TCE0_Overflow },
		#endif
		#if defined(TCF0)
		{ &TCF0, InterruptController::Cause::TCF0_Overflow }
		#endif
	};

	const PROGMEM TimerToInterruptCause<TC1_t> timerToCause1[] =
	{
		{ &TCC1, InterruptController::Cause::TCC1_Overflow },
		#if defined(TCD1)
		{ &TCD1, InterruptController::Cause::TCD1_Overflow },
		#endif
		#if defined(TCE1)
		{ &TCE1, InterruptController::Cause::TCE1_Overflow },
		#endif
	};
}

IntervalTimer::IntervalTimer(volatile TC0_t& timer, IIntervalTimerNotify& notify, TC_OVFINTLVL_t level)
 : _pTimer(new Timer0(timer, level))
 , _notify(notify)
 , _cause(findCause<TC0_t>(timerToCause0, timer))
{
	initialize();
}

IntervalTimer::IntervalTimer(volatile TC1_t& timer, IIntervalTimerNotify& notify, TC_OVFINTLVL_t level)
 : _pTimer(new Timer1(timer, level))
 , _notify(notify)
 , _cause(findCause<TC1_t>(timerToCause1, timer))
{
	initialize();
}

IntervalTimer::~IntervalTimer(void)
{
	_pTimer->Stop();
	InterruptController::ResetService(_cause);
}

void IntervalTimer::SetTimeout(uint32_t timeout)
{
	_timeout = timeout;
	uint16_t period;
	TC_CLKSEL_t clkSel = toPeriod(period, timeout);
	_pTimer->SetPeriod(clkSel, period);
}

void IntervalTimer::Start(void)
{
	_pTimer->Start();
}

void IntervalTimer::Stop(void)
{
	_pTimer->Stop();	
}

void IntervalTimer::initialize(void)
{
	InterruptController::SetService(_cause, onTimerOverflow, this);
	static const uint32_t DEFAULT_TIMEOUT = 1000;	// 1sec
	SetTimeout(DEFAULT_TIMEOUT);
}

TC_CLKSEL_t IntervalTimer::toPeriod(uint16_t& period, uint32_t millisecond)
{
	struct ClockDiv
	{
		TC_CLKSEL_t clkSel;
		uint8_t shift;
	};
	static const PROGMEM ClockDiv clockdivs[] =
	{
		{ TC_CLKSEL_DIV1_gc,     0 },
		{ TC_CLKSEL_DIV2_gc,     1 },
		{ TC_CLKSEL_DIV4_gc,     2 },
		{ TC_CLKSEL_DIV8_gc,     3 },
		{ TC_CLKSEL_DIV64_gc,    6 },
		{ TC_CLKSEL_DIV256_gc,   8 },
		{ TC_CLKSEL_DIV1024_gc, 10 },
		{ TC_CLKSEL_OFF_gc,     16 },
	};
	
	uint32_t temp = (static_cast<uint64_t>(SystemClock::GetInstance().GetPeripheralClock()) * millisecond + 999) / 1000;
	for (uint8_t i = 0;; ++i)
	{
		ClockDiv clockDiv;
		memcpy_PF(&clockDiv, reinterpret_cast<uint_farptr_t>(&clockdivs[i]), sizeof(ClockDiv));
		
		uint32_t period32 = temp >> clockDiv.shift;
		if (period32 < 65536)
		{
			period = static_cast<uint16_t>(period32);
			return clockDiv.clkSel;
		}
	}
}	

void IntervalTimer::onTimerOverflow(void* pContext)
{
	IntervalTimer* pThis = static_cast<IntervalTimer*>(pContext);
	pThis->_notify.OnTimer();
}	
