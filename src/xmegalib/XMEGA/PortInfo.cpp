/*
 * PortInfo.cpp
 *
 * Created: 2012/08/11 1:45:32
 *  Author: Seiji Ainoguchi
 */ 

#include "PortInfo.h"

using namespace XMEGA;

const PortInfo::Info PortInfo::_infos[] =
{
#if defined(PORTA)
	{ &PORTA, EVSYS_CHMUX_PORTA_PIN0_gc },
#endif
	{ &PORTB, EVSYS_CHMUX_PORTB_PIN0_gc },
	{ &PORTC, EVSYS_CHMUX_PORTC_PIN0_gc },
	{ &PORTD, EVSYS_CHMUX_PORTD_PIN0_gc },
#if defined(PORTE)
	{ &PORTE, EVSYS_CHMUX_PORTE_PIN0_gc },
#endif
#if defined(PORTF)
	{ &PORTF, EVSYS_CHMUX_PORTF_PIN0_gc },
#endif
};

PortInfo::PortInfo(PORT_t& port, uint8_t bp)
 : _bp(bp)
{
	for (const Info* p = _infos;; p++)
	{
		if (p->pPort == &port)
		{
			_pInfo = p;
			break;
		}
	}
}