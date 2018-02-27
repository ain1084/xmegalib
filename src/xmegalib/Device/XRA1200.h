/*
 * XRA1200.h
 *
 * Created: 2012/10/26 0:54:43
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "IIOPort.h"

namespace Device
{
	class II2CMaster;
	class IInputPin;

	class XRA1200 : public IIOPort
	{
	public:
		enum Register
		{
			GPIOStatus,
			OutputControl,
			InputPolarityInversion,
			GPIOConfiguration,
			InputInternalPullupRegister,
			InputInterruptEnable,
			OutputThreeStateControl,
			InputInterruptStatus,
			InputRisingEdgeInterruptEnable,
			InputFallingEdgeInterruptEnable,
			InputFilterEnable,
		};

		XRA1200(II2CMaster& i2cMaster, uint8_t address);
		XRA1200(II2CMaster& i2cMaster, uint8_t address, IInputPin& intPin);

		virtual void SetDirection(uint8_t inputBitMasks);
		virtual uint8_t GetDirection(void) const;
		virtual void SetPullup(uint8_t bitMasks);
		virtual uint8_t GetPullup(void) const;
		virtual void SetData(uint8_t data);
		virtual uint8_t GetData(void) const;
		
	private:
		II2CMaster& _i2cMaster;
		const uint8_t _address;
		mutable uint8_t _lastData;
		IInputPin* _pIntPin;

		uint8_t read(Register regNumber) const;
		void write(Register regNumber, uint8_t data);
	};
}
