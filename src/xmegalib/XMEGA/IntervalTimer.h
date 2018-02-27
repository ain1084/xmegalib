/*
 * IntervalTimer.h
 *
 * Created: 2012/06/09 3:16:08
 *  Author: Seiji Ainoguchi
 */ 

#include <avr/io.h>
#include "CriticalSection.h"
#include "Utilities/ScopedPtr.h"

namespace Device
{
	class IIntervalTimerNotify;
}

namespace XMEGA
{
	class IntervalTimer
	{
	public:
		IntervalTimer(volatile TC0_t& timer, Device::IIntervalTimerNotify& notify, TC_OVFINTLVL_t level = TC_OVFINTLVL_MED_gc);
		IntervalTimer(volatile TC1_t& timer, Device::IIntervalTimerNotify& notify, TC_OVFINTLVL_t level = TC_OVFINTLVL_MED_gc);
		~IntervalTimer(void);
		
		void SetTimeout(uint32_t millisecond);
		void Start(void);
		void Stop(void);

	private:
		class ITimer
		{
		public:
			virtual ~ITimer(void) { }
			virtual void SetPeriod(TC_CLKSEL_t clksel, uint16_t period) = 0;
			virtual void Start(void) = 0;
			virtual void Stop(void) = 0;
		};
	
		template<typename T>
		class Timer : public ITimer
		{
		public:
			Timer(volatile T& timer, TC_OVFINTLVL_t level)
			 : _timer(timer)
			 , _clksel(TC_CLKSEL_OFF_gc)
			{
				CriticalSection cs;
				_timer.CTRLA = TC_CLKSEL_OFF_gc;
				_timer.INTCTRLA = level;
			}
			virtual ~Timer(void)
			{
				CriticalSection cs;
				_timer.CTRLA = TC_CLKSEL_OFF_gc;
				_timer.INTCTRLA = TC_OVFINTLVL_OFF_gc;
			}
			virtual void SetPeriod(TC_CLKSEL_t clksel, uint16_t period)
			{
				CriticalSection cs;
				_clksel = clksel;
				_timer.PER = period;
				_timer.CTRLA = _clksel;
			}
			virtual void Start()
			{
				_timer.CTRLA = _clksel;
			}
			virtual void Stop()
			{
				_timer.CTRLA = TC_CLKSEL_OFF_gc;
			}
		private:
			volatile T& _timer;
			TC_CLKSEL_t _clksel;
		};

		ScopedPtr<ITimer> _pTimer;
		Device::IIntervalTimerNotify& _notify;
		const InterruptController::Cause _cause;
		uint32_t _timeout;
		
		void initialize(void);
		static TC_CLKSEL_t toPeriod(uint16_t& period, uint32_t _frequency);
		static void onTimerOverflow(void*);
	};
}
