/*
 * AT020QVSVirtualPort.h
 *
 * Created: 2012/10/25 2:10:55
 *  Author: Seiji Ainoguchi
 */ 

#include "Platform.h"
#include "SPIChip.h"
#include "OutputPin.h"
#include "../Device/Display/IL9325/IIL9325ControlPort.h"

#pragma once

namespace XMEGA
{
	template<uint8_t WRITE_PIN>
	class AT020QVSVirtualPort : public Device::Display::IL9325::IIL9325ControlPort
	{
	public:
		AT020QVSVirtualPort(OutputPin& csPin, OutputPin& rsPin);

		virtual void WriteRegister(uint8_t registerNumber, uint16_t dataValue);
		virtual void WriteRegister(uint8_t registerNumber, const uint16_t data[], size_t count);
		virtual void WriteRegisterRepeat(uint8_t registerNumber, uint16_t data, uint32_t count);
	
	private:
		OutputPin& _csPin;
		OutputPin& _rsPin;
			
		inline void write8bit(uint8_t data)
		{
			VPORT1_OUT &= ~(1 << WRITE_PIN);
			VPORT0_OUT = data;
			VPORT1_OUT |= (1 << WRITE_PIN);
		}
		
		inline void write16bit(uint16_t data)
		{
			write8bit(data >> 8);
			write8bit(data);
		}
	};
}
