/*
 * EventChannelMultiplexerInput.h
 *
 * Created: 2012/08/10 0:47:33
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <avr/io.h>

namespace XMEGA
{
	class EventChannelMultiplexerInput
	{
	public:
		static EVSYS_CHMUX_t FromPortBP(PORT_t& port, uint8_t bp);
		static EVSYS_CHMUX_t FromPortBM(PORT_t& port, uint8_t bm);
	};
}
