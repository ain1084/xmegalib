/*
 * InterruptController.h
 *
 * Created: 2011/12/26 0:24:37
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <stddef.h>

namespace XMEGA
{
	class InterruptController
	{
	public:
		static void Start(void);
		static void Stop(void);
		
		enum class Cause
		{
#if defined(DMA_CH0_vect)
			DMAChannel0,
#endif
#if defined(DMA_CH1_vect)
			DMAChannel1,
#endif
#if defined(DMA_CH2_vect)
			DMAChannel2,
#endif
#if defined(DMA_CH3_vect)
			DMAChannel3,
#endif
			TCC0_Overflow,
			TCC0_Error,
			TCC0_CompareCaptureA,
			TCC0_CompareCaptureB,
			TCC0_CompareCaptureC,
			TCC0_CompareCaptureD,
			TCC1_Overflow,
			TCC1_Error,
			TCC1_CompareCaptureA,
			TCC1_CompareCaptureB,
			TCD0_Overflow,
			TCD0_Error,
			TCD0_CompareCaptureA,
			TCD0_CompareCaptureB,
			TCD0_CompareCaptureC,
			TCD0_CompareCaptureD,
#if defined(TCD1_OVF_vect)
			TCD1_Overflow,
#endif
#if defined(TCD1_ERR_vect)
			TCD1_Error,
#endif
#if defined(TCD1_CCA_vect)
			TCD1_CompareCaptureA,
#endif
#if defined(TCD1_CCB_vect)
			TCD1_CompareCaptureB,
#endif
			TCE0_Overflow,
			TCE0_Error,
			TCE0_CompareCaptureA,
			TCE0_CompareCaptureB,
			TCE0_CompareCaptureC,
			TCE0_CompareCaptureD,
#if defined(TCE1_OVF_vect)
			TCE1_Overflow,
#endif
#if defined(TCE1_ERR_vect)
			TCE1_Error,
#endif
#if defined(TCE1_CCA_vect)
			TCE1_CompareCaptureA,
#endif
#if defined(TCE1_CCB_vect)
			TCE1_CompareCaptureB,
#endif
#if defined(TCF0_OVF_vect)
			TCF0_Overflow,
#endif
#if defined(TCF0_ERR_vect)
			TCF0_Error,
#endif
#if defined(TCF0_CCA_vect)
			TCF0_CompareCaptureA,
#endif
#if defined(TCF0_CCB_vect)
			TCF0_CompareCaptureB,
#endif
#if defined(TCF0_CCC_vect)
			TCF0_CompareCaptureC,
#endif
#if defined(TCF0_CCD_vect)
			TCF0_CompareCaptureD,
#endif
			Count,
		};
		typedef void (*ServiceRoutine)(void* pContext);

		static void SetService(Cause cause, ServiceRoutine routine, void* pContext);
		static void ResetService(Cause cause);

		//internal only
		static void execute(Cause cause)
		{
			const auto& causeInfo = _causeInfos[static_cast<int>(cause)];
			if (causeInfo.routine != nullptr)
			{
				causeInfo.routine(causeInfo.pContext);				
			}
		}

	private:
		static struct CauseInfo
		{
			ServiceRoutine routine;
			void* pContext;
		} _causeInfos[static_cast<int>(Cause::Count)];
	};
}
