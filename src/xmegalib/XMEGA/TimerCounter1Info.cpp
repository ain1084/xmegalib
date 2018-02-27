/*
 * TimerCounter1Info.cpp
 *
 * Created: 2012/08/10 1:18:00
 *  Author: Seiji Ainoguchi
 */ 

#include "TimerCounter1Info.h"

using namespace XMEGA;

const TimerCounter1Info::TimerInfo TimerCounter1Info::_timerInfos[] =
{
	{ TCC1, PORTC, InterruptController::Cause::TCC1_Overflow, InterruptController::Cause::TCC1_Error,
		InterruptController::Cause::TCC1_CompareCaptureA, InterruptController::Cause::TCC1_CompareCaptureB,
		EVSYS_CHMUX_TCC1_OVF_gc, DMA_CH_TRIGSRC_TCC1_OVF_gc },
	#if defined(TCD1)
	{ TCD1, PORTD, InterruptController::Cause::TCD1_Overflow, InterruptController::Cause::TCD1_Error,
		InterruptController::Cause::TCD1_CompareCaptureA, InterruptController::Cause::TCD1_CompareCaptureB,
		EVSYS_CHMUX_TCD1_OVF_gc, DMA_CH_TRIGSRC_TCD1_OVF_gc },
	#endif
	#if defined(TCE1) && defined(PORTE)
	{ TCE1, PORTE, InterruptController::Cause::TCE1_Overflow, InterruptController::Cause::TCE1_Error,
		InterruptController::Cause::TCE1_CompareCaptureA, InterruptController::Cause::TCE1_CompareCaptureB,
		EVSYS_CHMUX_TCE1_OVF_gc, DMA_CH_TRIGSRC_TCE1_OVF_gc },
	#endif
};

TimerCounter1Info::TimerCounter1Info(TC1_t& timer)
{
	for (const TimerInfo* p = _timerInfos;; p++)
	{
		if (&p->Timer == &timer)
		{
			_pTimerInfo = p;
			break;
		}
	}
}