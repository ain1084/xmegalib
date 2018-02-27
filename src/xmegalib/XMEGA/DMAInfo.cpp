/* 
* DMA.cpp
*
* Created: 2016/01/31 0:32:08
* Author: ain
*/

#include "DMAInfo.h"
#include "InterruptController.h"

using namespace XMEGA;

const DMAInfo::DMADoubleBufferTable DMAInfo::_doubleBufferTable[] =
{
	{ DMADoubleBufferMode::Channel_0_1,
		 { DMA_DBUFMODE_CH01_gc,
			 {
				 { DMA.CH0, InterruptController::Cause::DMAChannel0 },
				 { DMA.CH1, InterruptController::Cause::DMAChannel1 },
			 }
		 }
	},
	#if defined(DMA_CH2_vect) && defined(DMA_CH3_vect)
	{ DMADoubleBufferMode::Channel_2_3,
		{ DMA_DBUFMODE_CH23_gc,
			 {
				 { DMA.CH2, InterruptController::Cause::DMAChannel2 },
				 { DMA.CH3, InterruptController::Cause::DMAChannel3 },
			 }
		}
	},
	#endif
};

const DMADoubleBufferInfo& DMAInfo::GetDoubleBufferInfo(DMADoubleBufferMode doubleBufferMode)
{
	for (auto i = 0;; i++)
	{
		if (_doubleBufferTable[i].doubleBufferMode == doubleBufferMode)	
		{
			return _doubleBufferTable[i].info;
		}
	}
}
