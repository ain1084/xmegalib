/*
 * EventChannelMultiplexerInput.cpp
 *
 * Created: 2012/08/10 0:50:55
 *  Author: Seiji Ainoguchi
 */ 

#include "EventChannelMultiplexerInput.h"

using namespace XMEGA;

EVSYS_CHMUX_t EventChannelMultiplexerInput::FromPortBP(PORT_t& port, uint8_t bp)
{
	auto base = EVSYS_CHMUX_PORTA_PIN0_gc;
	if (&port == &PORTB)
	{
		base = EVSYS_CHMUX_PORTB_PIN0_gc;
	}
	else if (&port == &PORTC)
	{
		base = EVSYS_CHMUX_PORTC_PIN0_gc;
	}
	else if (&port == &PORTD)
	{
		base = EVSYS_CHMUX_PORTD_PIN0_gc;
	}
	#if defined(PORTE)
	else if (&port == &PORTE)
	{
		base = EVSYS_CHMUX_PORTE_PIN0_gc;
	}
	#endif
	#if defined(PORTF)
	else if (&port == &PORTF)
	{
		base = EVSYS_CHMUX_PORTF_PIN0_gc;
	}
	#endif
	return static_cast<EVSYS_CHMUX_t>(base + bp);
}

EVSYS_CHMUX_t EventChannelMultiplexerInput::FromPortBM(PORT_t& port, uint8_t bm)
{
	if (bm == 0x00)
	{
		return EVSYS_CHMUX_OFF_gc;
	}
	uint8_t bp = 0;
	while ((bp & 0x1) == 0b0)
	{
		++bp;
	}
	return FromPortBP(port, bp);		
}