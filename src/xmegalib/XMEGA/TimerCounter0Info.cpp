/*
 * TimerCounter0Info.cpp
 *
 * Created: 2012/08/10 1:08:43
 *  Author: Seiji Ainoguchi
 */ 

#include "TimerCounter0Info.h"

using namespace XMEGA;

const TimerCounter0Info::TimerInfo TimerCounter0Info::_timerInfos[] =
{
	{ TCC0, PORTC, InterruptController::Cause::TCC0_Overflow, InterruptController::Cause::TCC0_Error,
		InterruptController::Cause::TCC0_CompareCaptureA, InterruptController::Cause::TCC0_CompareCaptureB,
		InterruptController::Cause::TCC0_CompareCaptureC, InterruptController::Cause::TCC0_CompareCaptureD,
		EVSYS_CHMUX_TCC0_OVF_gc, DMA_CH_TRIGSRC_TCC0_OVF_gc },
	#if defined(TCD0)
	{ TCD0, PORTD, InterruptController::Cause::TCD0_Overflow, InterruptController::Cause::TCD0_Error, 
		InterruptController::Cause::TCD0_CompareCaptureA, InterruptController::Cause::TCD0_CompareCaptureB,
		InterruptController::Cause::TCD0_CompareCaptureC, InterruptController::Cause::TCD0_CompareCaptureD,
		EVSYS_CHMUX_TCD0_OVF_gc, DMA_CH_TRIGSRC_TCD0_OVF_gc },
	#endif
	#if defined(TCE0)
	{ TCE0, PORTD, InterruptController::Cause::TCE0_Overflow, InterruptController::Cause::TCE0_Error, 
		InterruptController::Cause::TCE0_CompareCaptureA, InterruptController::Cause::TCE0_CompareCaptureB,
		InterruptController::Cause::TCE0_CompareCaptureC, InterruptController::Cause::TCE0_CompareCaptureD,
		EVSYS_CHMUX_TCE0_OVF_gc, DMA_CH_TRIGSRC_TCE0_OVF_gc },
	#endif
#if defined(TCF0) && defined(PORTF)
	{ TCF0, PORTF, InterruptController::Cause::TCF0_Overflow, InterruptController::Cause::TCF0_Error, 
		InterruptController::Cause::TCF0_CompareCaptureA, InterruptController::Cause::TCF0_CompareCaptureB,
		InterruptController::Cause::TCF0_CompareCaptureC, InterruptController::Cause::TCF0_CompareCaptureD,
		EVSYS_CHMUX_TCF0_OVF_gc, DMA_CH_TRIGSRC_TCF0_OVF_gc },
#endif
};

TimerCounter0Info::TimerCounter0Info(volatile TC0_t& timer)
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