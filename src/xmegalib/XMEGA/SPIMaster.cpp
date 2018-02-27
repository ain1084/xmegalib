/*
 * SPIMaster.cpp
 *
 * Created: 2011/11/27 2:12:29
 *  Author: Seiji Ainoguchi
 */ 

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <string.h>
#include "SystemClock.h"
#include "SPIMaster.h"

namespace
{
	const uint32_t DEFAULT_FREQUENCY = 10000000L;
}

using namespace XMEGA;

SPIMaster::SPIMaster(USART_t& usart)
 : _usart(usart)
{
	static const PROGMEM PortConfigInfo portConfigInfos[] =
	{
		{ &USARTC0, &PORTC, PIN1_bm | PIN3_bm, PIN2_bm },
		#if defined(USARTC1) && defined(PORTC)
		{ &USARTC1, &PORTC, PIN5_bm | PIN7_bm, PIN6_bm },
		#endif
		#if defined(USARTD0) && defined(PORTD)
		{ &USARTD0, &PORTD, PIN1_bm | PIN3_bm, PIN2_bm },
		#endif
		#if defined(USARTD1) && defined(PORTD)
		{ &USARTD1, &PORTD, PIN5_bm | PIN7_bm, PIN6_bm },
		#endif
		#if defined(USARTE0) && defined(PORTE)
		{ &USARTE0, &PORTE, PIN1_bm | PIN3_bm, PIN2_bm },
		#endif
		#if defined(USARTE1) && defined(PORTE)
		{ &USARTE1, &PORTE, PIN5_bm | PIN7_bm, PIN6_bm },
		#endif
		#if defined(USARTF0) && defined(PORTF)
		{ &USARTF0, &PORTF, PIN1_bm | PIN3_bm, PIN2_bm },
		#endif
	};

	for (const PortConfigInfo* p = portConfigInfos;; p++)
	{
		memcpy_P(&_portConfigInfo, p, sizeof(PortConfigInfo));
		if (_portConfigInfo.usart == &_usart)
		{
			_portConfigInfo.port->DIRSET = _portConfigInfo.dirset;
			_portConfigInfo.port->DIRCLR = _portConfigInfo.dirclr;

			usart.CTRLA = 0;
			usart.CTRLB = 0;
			usart.CTRLC = USART_CMODE_MSPI_gc;
			SetFrequency(DEFAULT_FREQUENCY);
			return;	
		}
	}
}

void SPIMaster::SetBaudrateValue(uint16_t baudrateValue)
{
	_usart.BAUDCTRLA = baudrateValue & 0xff;
	_usart.BAUDCTRLB = baudrateValue >> 8;
}

uint16_t SPIMaster::CalcBaudrateValue(uint32_t frequency) const
{
	uint16_t baud;
	if (frequency == 0 || SystemClock::GetInstance().GetPeripheralClock() <= frequency * 2)
	{
		baud = 0;
	}
	else
	{
		baud = ((SystemClock::GetInstance().GetPeripheralClock() + frequency * 2 - 1) / (frequency * 2)) - 1;
	}
	return baud;
}

void SPIMaster::SetFrequency(uint32_t frequency)
{
	SetBaudrateValue(CalcBaudrateValue(frequency));
}

uint8_t SPIMaster::Read(void)
{
	volatile USART_t& usart = _usart;

	usart.CTRLB  = USART_TXEN_bm | USART_RXEN_bm;

	usart.DATA = 0xFF;

	while (!(usart.STATUS & USART_RXCIF_bm))
		;
		
	return usart.DATA;
}

void SPIMaster::SendClock(unsigned clockByteCount)
{
	volatile USART_t& usart = _usart;

	usart.CTRLB  = USART_TXEN_bm | USART_RXEN_bm;

	do 
	{
		usart.DATA = 0xFF;
		while (!(usart.STATUS & USART_RXCIF_bm))
			;
		volatile uint8_t dummy = usart.DATA;
	} while (--clockByteCount != 0);
}

void SPIMaster::Read(void* dest, unsigned length)
{
	volatile USART_t& usart = _usart;
	if (length == 0)
	{
		return;
	}
	

	usart.CTRLB  = USART_TXEN_bm | USART_RXEN_bm;

	uint8_t* buf = static_cast<uint8_t*>(dest);
	uint8_t* bufend = buf + length;
		
	for (;;) 
	{
		uint8_t status = usart.STATUS;
		if (status & USART_DREIF_bm)
		{
			if (status & USART_RXCIF_bm)
			{
				*buf++ = usart.DATA;
			}
			usart.DATA = 0xFF;
			if (--length == 0)
			{
				break;
			}
		}
	}
	while (bufend != buf)
	{
		while (!(usart.STATUS & USART_RXCIF_bm))
			;
		*buf++ = usart.DATA;
	}
}

void SPIMaster::Write(uint8_t data)
{
	volatile USART_t& usart = _usart;

	usart.CTRLB = USART_TXEN_bm;
	usart.STATUS = USART_TXCIF_bm;
	usart.DATA = data;

	while (!(usart.STATUS & USART_TXCIF_bm))
		;
}

void SPIMaster::Write(const void* src, unsigned length)
{
	volatile USART_t& usart = _usart;

	if (length == 0)
	{
		return;
	}
	
	usart.CTRLB  = USART_TXEN_bm;
	const uint8_t* buf = static_cast<const uint8_t*>(src);
	do 
	{
		uint8_t nextData = *buf++;
		while (!(usart.STATUS & USART_DREIF_bm))
			;
		usart.STATUS = USART_TXCIF_bm;
		usart.DATA = nextData;
	} while (--length != 0);

	while (!(usart.STATUS & USART_TXCIF_bm))
		;
}
