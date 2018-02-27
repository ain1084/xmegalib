/*
 * CriticalSection.h
 *
 * Created: 2011/12/20 2:00:06
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>

namespace XMEGA
{
	class CriticalSection
	{
	public:
		inline CriticalSection(void)
		: _sreg(SREG) { cli(); }	

		inline ~CriticalSection(void)
		{
			SREG = _sreg;
		}

	private:
		const uint8_t _sreg;
	};
};
