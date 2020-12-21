/*
 * TimerCounter1Info.h
 *
 * Created: 2012/08/10 1:16:36
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <avr/io.h>
#include "InterruptController.h"

namespace XMEGA
{
	class TimerCounter1Info
	{
	public:
		TimerCounter1Info(volatile TC1_t& timer);	

		TC1_t& GetTimer(void) const
		{
			return _pTimerInfo->Timer;
		}
		
		PORT_t& GetPort(void) const
		{
			return _pTimerInfo->Port;
		}

		InterruptController::Cause GetOverflowInterruptCause(void) const
		{
			return _pTimerInfo->overflowCause;
		}

		InterruptController::Cause GetErrorInterruptCause(void) const
		{
			return _pTimerInfo->errorCause;
		}

		InterruptController::Cause GetCompareCaptureAInterruptCause(void) const
		{
			return _pTimerInfo->compareCaptureACause;
		}

		InterruptController::Cause GetCompareCaptureBInterruptCause(void) const
		{
			return _pTimerInfo->compareCaptureBCause;
		}
		
		EVSYS_CHMUX_t GetOverflowEvent(void) const
		{
			return static_cast<EVSYS_CHMUX_t>(_pTimerInfo->eventBase + 0b000);
		}

		EVSYS_CHMUX_t GetErrorEvent(void) const
		{
			return static_cast<EVSYS_CHMUX_t>(_pTimerInfo->eventBase + 0b001);
		}

		EVSYS_CHMUX_t GetCompareCaptureAEvent(void) const
		{
			return static_cast<EVSYS_CHMUX_t>(_pTimerInfo->eventBase + 0b100);
		}

		EVSYS_CHMUX_t GetCompareCaptureBEvent(void) const
		{
			return static_cast<EVSYS_CHMUX_t>(_pTimerInfo->eventBase + 0b101);
		}

		DMA_CH_TRIGSRC_t GetOverflowTriggerSource(void) const
		{
			return static_cast<DMA_CH_TRIGSRC_t>(_pTimerInfo->DMATriggerBase + 0x00);
		}

		DMA_CH_TRIGSRC_t GetErrorTriggerSource(void) const
		{
			return static_cast<DMA_CH_TRIGSRC_t>(_pTimerInfo->DMATriggerBase + 0x01);
		}

		DMA_CH_TRIGSRC_t GetCompareCaptureATriggerSource(void) const
		{
			return static_cast<DMA_CH_TRIGSRC_t>(_pTimerInfo->DMATriggerBase + 0x02);
		}

		DMA_CH_TRIGSRC_t GetCompareCaptureBTriggerSource(void) const
		{
			return static_cast<DMA_CH_TRIGSRC_t>(_pTimerInfo->DMATriggerBase + 0x03);
		}

		uint8_t GetOC1ABitPosition(void) const
		{
			return PIN4_bp;
		}

		uint8_t GetOC1ABitMask(void) const
		{
			return PIN4_bm;
		}
		
		uint8_t GetOC1BBitPosition(void) const
		{
			return PIN5_bp;
		}

		uint8_t GetOC1BBitMask(void) const
		{
			return PIN5_bm;
		}

	private:
		struct TimerInfo
		{
			TC1_t& Timer;
			PORT_t& Port;
			InterruptController::Cause overflowCause;
			InterruptController::Cause errorCause;
			InterruptController::Cause compareCaptureACause;
			InterruptController::Cause compareCaptureBCause;
			EVSYS_CHMUX_t eventBase;
			DMA_CH_TRIGSRC_t DMATriggerBase;
		} const* _pTimerInfo;
		
		static const TimerInfo _timerInfos[];
	};
}
