/*
 * IIL9325ControlPort.h
 *
 * Created: 2012/09/28 14:44:21
 *  Author: Seiji Ainoguchi
 */ 

#include "Platform.h"

#pragma once

namespace Device
{
	namespace Display
	{
		namespace IL9325
		{
			class IIL9325ControlPort
			{
			public:
				virtual ~IIL9325ControlPort(void) { }
				
				virtual void WriteRegister(uint8_t registerNumber, uint16_t dataValue) = 0;
				virtual void WriteRegister(uint8_t registerNumber, const uint16_t data[], size_t count) = 0;
				virtual void WriteRegisterRepeat(uint8_t registerNumber, uint16_t data, uint32_t count) = 0;
			};
			
		}
	}	
}
