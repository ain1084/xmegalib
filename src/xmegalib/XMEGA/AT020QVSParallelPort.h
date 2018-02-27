/*
 * AT020QVSParallelPort.h
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
	class AT020QVSParallelPort : public Device::Display::IL9325::IIL9325ControlPort
	{
	public:
		AT020QVSParallelPort(PORT_t& port, OutputPin& csPin, OutputPin& rsPin, PORT_t& wrPort, uint8_t wrPin);
	
		virtual void WriteRegister(uint8_t registerNumber, uint16_t dataValue);
		virtual void WriteRegister(uint8_t registerNumber, const uint16_t data[], size_t count);
		virtual void WriteRegisterRepeat(uint8_t registerNumber, uint16_t data, uint32_t count);
	
	private:
		PORT_t& _port;
		OutputPin& _csPin;
		OutputPin& _rsPin;
		
		PORT_t& _wrPort;
		const uint8_t _wrBm;
			
		void write8bit(uint8_t data)
		{
			PORT_t& wrPort = _wrPort;
			uint8_t wrBm = _wrBm;
			wrPort.OUTCLR = wrBm;
			_port.OUT = data;
			wrPort.OUTSET = wrBm;
		}
			
		void __attribute__((always_inline)) write16bit(uint16_t data)
		{
			write8bit(data >> 8);
			write8bit(data);
		}
	};
}
