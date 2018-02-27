/*
 * IL9325GraphicDisplayImpl.h
 *
 * Created: 2012/09/28 10:18:22
 *  Author: Seiji Ainoguchi
 */ 

#include "Platform.h"
#include "IIL9325ControlPort.h"

#pragma once

namespace Device
{
	namespace Display
	{
		namespace IL9325
		{
			class AT020QVS164ControlPort : public IIL9325ControlPort
			{
			public:
				AT020QVS164ControlPort(SPIChip& chip, OutputPin& rsPin, OutputPin& wrPin, OutputPin& backlightPin);
				AT020QVS164ControlPort(SPIChip& chip, OutputPin& rsPin, OutputPin& wrPin);
	
				virtual void WriteRegister(uint8_t registerNumber, uint16_t dataValue);
				virtual void WriteRegister(uint8_t registerNumber, const uint16_t data[], size_t count);
				virtual void WriteRegisterRepeat(uint8_t registerNumber, uint16_t data, uint32_t count);
	
			private:
				SPIChip& _chip;
				OutputPin& _rsPin;
				OutputPin& _wrPin;
				OutputPin* _pBacklightPin;
			
				void __attribute__((always_inline)) write8bit(uint8_t data)
				{
					_chip.Write(data);
					_wrPin.Low();
					_wrPin.High();
				}
			
				void __attribute__((always_inline)) write16bit(uint16_t data)
				{
					write8bit(data >> 8);
					write8bit(data);
				}
			};
		}			
	}
}
