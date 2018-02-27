/*
 * InterruptController.cpp
 *
 * Created: 2011/12/26 0:24:49
 *  Author: Seiji Ainoguchi
 */ 

#include "Platform.h"
#include <string.h>
#include <avr/io.h>
#include "CriticalSection.h"
#include "InterruptController.h"

using namespace XMEGA;

InterruptController::CauseInfo InterruptController::_causeInfos[static_cast<int>(InterruptController::Cause::Count)];

void InterruptController::Start(void)
{
	PMIC_CTRL |= PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm;
	sei();
}

void InterruptController::Stop(void)
{
	PMIC_CTRL = 0;
	cli();
}

void InterruptController::SetService(Cause cause, ServiceRoutine routine, void* pContext)
{
	CriticalSection cs;
	auto index = static_cast<int>(cause);
	_causeInfos[index].routine = routine;
	_causeInfos[index].pContext = pContext;
}

void InterruptController::ResetService(Cause cause)
{
	CriticalSection cs;
	auto index = static_cast<int>(cause);
	_causeInfos[index].routine = nullptr;
	_causeInfos[index].pContext = nullptr;
}

#if defined(DMA_CH0_vect)
ISR(DMA_CH0_vect)
{
	InterruptController::execute(InterruptController::Cause::DMAChannel0);
}
#endif

#if defined(DMA_CH1_vect)
ISR(DMA_CH1_vect)
{
	InterruptController::execute(InterruptController::Cause::DMAChannel1);
}
#endif

#if defined(DMA_CH2_vect)
ISR(DMA_CH2_vect)
{
	InterruptController::execute(InterruptController::Cause::DMAChannel2);
}
#endif

#if defined(DMA_CH3_vect)
ISR(DMA_CH3_vect)
{
	InterruptController::execute(InterruptController::Cause::DMAChannel3);
}
#endif

ISR(TCC0_OVF_vect)
{
	InterruptController::execute(InterruptController::Cause::TCC0_Overflow);
}

ISR(TCC0_ERR_vect)
{
	InterruptController::execute(InterruptController::Cause::TCC0_Error);
}

ISR(TCC0_CCA_vect)
{
	InterruptController::execute(InterruptController::Cause::TCC0_CompareCaptureA);
}

ISR(TCC0_CCB_vect)
{
	InterruptController::execute(InterruptController::Cause::TCC0_CompareCaptureB);
}

ISR(TCC0_CCC_vect)
{
	InterruptController::execute(InterruptController::Cause::TCC0_CompareCaptureC);
}

ISR(TCC0_CCD_vect)
{
	InterruptController::execute(InterruptController::Cause::TCC0_CompareCaptureD);
}

ISR(TCC1_OVF_vect)
{
	InterruptController::execute(InterruptController::Cause::TCC1_Overflow);
}

ISR(TCC1_ERR_vect)
{
	InterruptController::execute(InterruptController::Cause::TCC1_Error);
}

ISR(TCC1_CCA_vect)
{
	InterruptController::execute(InterruptController::Cause::TCC1_CompareCaptureA);
}

ISR(TCC1_CCB_vect)
{
	InterruptController::execute(InterruptController::Cause::TCC1_CompareCaptureB);
}

ISR(TCD0_OVF_vect)
{
	InterruptController::execute(InterruptController::Cause::TCD0_Overflow);
}

ISR(TCD0_ERR_vect)
{
	InterruptController::execute(InterruptController::Cause::TCD0_Error);
}

ISR(TCD0_CCA_vect)
{
	InterruptController::execute(InterruptController::Cause::TCD0_CompareCaptureA);
}

ISR(TCD0_CCB_vect)
{
	InterruptController::execute(InterruptController::Cause::TCD0_CompareCaptureB);
}

ISR(TCD0_CCC_vect)
{
	InterruptController::execute(InterruptController::Cause::TCD0_CompareCaptureC);
}

ISR(TCD0_CCD_vect)
{
	InterruptController::execute(InterruptController::Cause::TCD0_CompareCaptureD);
}

#if defined(TCD1_OVF_vect)
ISR(TCD1_OVF_vect)
{
	InterruptController::execute(InterruptController::Cause::TCD1_Overflow);
}
#endif

#if defined(TCD1_ERR_vect)
ISR(TCD1_ERR_vect)
{
	InterruptController::execute(InterruptController::Cause::TCD1_Error);
}
#endif

#if defined(TCD1_CCA_vect)
ISR(TCD1_CCA_vect)
{
	InterruptController::execute(InterruptController::Cause::TCD1_CompareCaptureA);
}
#endif

#if defined(TCD1_CCB_vect)
ISR(TCD1_CCB_vect)
{
	InterruptController::execute(InterruptController::Cause::TCD1_CompareCaptureB);
}
#endif


ISR(TCE0_OVF_vect)
{
	InterruptController::execute(InterruptController::Cause::TCE0_Overflow);
}

ISR(TCE0_ERR_vect)
{
	InterruptController::execute(InterruptController::Cause::TCE0_Error);
}

ISR(TCE0_CCA_vect)
{
	InterruptController::execute(InterruptController::Cause::TCE0_CompareCaptureA);
}

ISR(TCE0_CCB_vect)
{
	InterruptController::execute(InterruptController::Cause::TCE0_CompareCaptureB);
}

ISR(TCE0_CCC_vect)
{
	InterruptController::execute(InterruptController::Cause::TCE0_CompareCaptureC);
}

ISR(TCE0_CCD_vect)
{
	InterruptController::execute(InterruptController::Cause::TCE0_CompareCaptureD);
}

#if defined(TCE1_OVF_vect)
ISR(TCE1_OVF_vect)
{
	InterruptController::execute(InterruptController::Cause::TCE1_Overflow);
}
#endif

#if defined(TCE1_ERR_vect)
ISR(TCE1_ERR_vect)
{
	InterruptController::execute(InterruptController::Cause::TCE1_Error);
}
#endif

#if defined(TCE1_CCA_vect)
ISR(TCE1_CCA_vect)
{
	InterruptController::execute(InterruptController::Cause::TCE1_CompareCaptureA);
}
#endif

#if defined(TCE1_CCB_vect)
ISR(TCE1_CCB_vect)
{
	InterruptController::execute(InterruptController::Cause::TCE1_CompareCaptureB);
}
#endif

#if defined(TCF0_OVF_vect)
ISR(TCF0_OVF_vect)
{
	InterruptController::execute(InterruptController::Cause::TCF0_Overflow);
}
#endif

#if defined(TCF0_ERR_vect)
ISR(TCF0_ERR_vect)
{
	InterruptController::execute(InterruptController::Cause::TCF0_Error);
}
#endif

#if defined(TCF0_CCA_vect)
ISR(TCF0_CCA_vect)
{
	InterruptController::execute(InterruptController::Cause::TCF0_CompareCaptureA);
}
#endif

#if defined(TCF0_CCB_vect)
ISR(TCF0_CCB_vect)
{
	InterruptController::execute(InterruptController::Cause::TCF0_CompareCaptureB);
}
#endif

#if defined(TCF0_CCC_vect)
ISR(TCF0_CCC_vect)
{
	InterruptController::execute(InterruptController::Cause::TCF0_CompareCaptureC);
}
#endif

#if defined(TCF0_CCD_vect)
ISR(TCF0_CCD_vect)
{
	InterruptController::execute(InterruptController::Cause::TCF0_CompareCaptureD);
}
#endif
