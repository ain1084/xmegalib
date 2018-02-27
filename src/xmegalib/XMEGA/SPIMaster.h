/*
 * SPIMaster.h
 *
 * Created: 2011/11/27 2:12:19
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <avr/io.h>

namespace XMEGA
{
	class SPIMaster
	{
	public:
		SPIMaster(USART_t& usart);

		void SetFrequency(uint32_t frequency);
		void Write(uint8_t data);
		uint8_t Read(void);
		void SendClock(unsigned clockByteCount);
		void Read(void* dest, unsigned length);
		void Write(const void* src, unsigned length);

		//for internal
		void SetBaudrateValue(uint16_t baudrateValue);
		uint16_t CalcBaudrateValue(uint32_t frequency) const;

	private:
		volatile USART_t& _usart;

		struct PortConfigInfo
		{
			USART_t* usart;
			PORT_t* port;
			uint8_t dirset;
			uint8_t dirclr;
		} _portConfigInfo;
	};
}
