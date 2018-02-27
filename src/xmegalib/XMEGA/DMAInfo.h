/* 
* DMAInfo.h
*
* Created: 2016/01/31 0:32:09
* Author: ain
*/

#pragma once

#include <avr/io.h>
#include "InterruptController.h"

namespace XMEGA
{
	enum class DMADoubleBufferMode
	{
		Channel_0_1,
		#if defined(DMA_CH2_vect) && defined(DMA_CH3_vect)
		Channel_2_3
		#endif
	};

	struct DMAChannelInfo
	{
		DMA_CH_t& IO;
		const InterruptController::Cause InterruptCause;
	};

	struct DMADoubleBufferInfo
	{
		const DMA_DBUFMODE_t dbufMode;
		const DMAChannelInfo Channels[2];
	};
		
	class DMAInfo
	{
	public:
		static const DMADoubleBufferInfo& GetDoubleBufferInfo(DMADoubleBufferMode bufferMode);	

	private:
		struct DMADoubleBufferTable
		{
			const DMADoubleBufferMode doubleBufferMode;
			const DMADoubleBufferInfo info;
		};

		static const DMADoubleBufferTable _doubleBufferTable[];
	};
}