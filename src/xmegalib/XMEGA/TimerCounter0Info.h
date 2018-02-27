/*
 * TimerCounter0Info.h
 *
 * Created: 2012/08/10 1:01:31
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <avr/io.h>
#include "InterruptController.h"

namespace XMEGA
{
	class TimerCounter0Info
	{
	public:
		TimerCounter0Info(TC0_t& timer);	

		TC0_t& GetTimer(void) const
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

		InterruptController::Cause GetCompareCaptureCInterruptCause(void) const
		{
			return _pTimerInfo->compareCaptureCCause;
		}

		InterruptController::Cause GetCompareCaptureDInterruptCause(void) const
		{
			return _pTimerInfo->compareCaptureDCause;
		}
		
		EVSYS_CHMUX_t GetOverflowEvent(void) const
		{
			return static_cast<EVSYS_CHMUX_t>(_pTimerInfo->eventBase + 0b000);
		}

		EVSYS_CHMUX_t GetErrorEvent(void) const
		{
			return static_cast<EVSYS_CHMUX_t>(_pTimerInfo->eventBase + 0b001);
		}

		EVSYS_CHMUX_t GetCompareCaptureA(void) const
		{
			return static_cast<EVSYS_CHMUX_t>(_pTimerInfo->eventBase + 0b100);
		}

		EVSYS_CHMUX_t GetCompareCaptureB(void) const
		{
			return static_cast<EVSYS_CHMUX_t>(_pTimerInfo->eventBase + 0b101);
		}
		
		EVSYS_CHMUX_t GetCompareCaptureC(void) const
		{
			return static_cast<EVSYS_CHMUX_t>(_pTimerInfo->eventBase + 0b110);
		}

		EVSYS_CHMUX_t GetCompareCaptureD(void) const
		{
			return static_cast<EVSYS_CHMUX_t>(_pTimerInfo->eventBase + 0b111);
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

		DMA_CH_TRIGSRC_t GetCompareCaptureCTriggerSource(void) const
		{
			return static_cast<DMA_CH_TRIGSRC_t>(_pTimerInfo->DMATriggerBase + 0x04);
		}

		DMA_CH_TRIGSRC_t GetCompareCaptureDTriggerSource(void) const
		{
			return static_cast<DMA_CH_TRIGSRC_t>(_pTimerInfo->DMATriggerBase + 0x05);
		}

		uint8_t GetOC0ABitPosition(void) const
		{
			return PIN0_bp;
		}

		uint8_t GetOC0ABitMask(void) const
		{
			return PIN0_bm;
		}
		
		uint8_t GetOC0BBitPosition(void) const
		{
			return PIN1_bp;
		}

		uint8_t GetOC0BBitMask(void) const
		{
			return PIN1_bm;
		}

		uint8_t GetOC0CBitPosition(void) const
		{
			return PIN2_bp;
		}

		uint8_t GetOC0CBitMask(void) const
		{
			return PIN2_bm;
		}

		uint8_t GetOC0DBitPosition(void) const
		{
			return PIN3_bp;
		}

		uint8_t GetOC0DBitMask(void) const
		{
			return PIN3_bm;
		}

	private:
		struct TimerInfo
		{
			TC0_t& Timer;
			PORT_t& Port;
			InterruptController::Cause overflowCause;
			InterruptController::Cause errorCause;
			InterruptController::Cause compareCaptureACause;
			InterruptController::Cause compareCaptureBCause;
			InterruptController::Cause compareCaptureCCause;
			InterruptController::Cause compareCaptureDCause;
			EVSYS_CHMUX_t eventBase;
			DMA_CH_TRIGSRC_t DMATriggerBase;
		} const* _pTimerInfo;
		
		static const TimerInfo _timerInfos[];
	};
}
