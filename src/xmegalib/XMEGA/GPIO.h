/*
 * GPIO.h
 *
 * Created: 2017/09/03 0:09:38
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

namespace XMEGA
{
	class GPIO
	{
	public:
		GPIO(volatile PORT_t& port, uint8_t pinNumber, uint8_t pinCtrl = PORT_OPC_TOTEM_gc);

		void SetCtrl(uint8_t pinCtrl)
		{
			*(&_port.PIN0CTRL + _pinNumber) = pinCtrl;
		}

		void SetOutput(void)
		{
			_port.DIRSET = _pinBit;
		}
		
		void SetInput(void)
		{
			_port.DIRCLR = _pinBit;			
		}
		
		void SetHigh(void)
		{
			_port.OUTSET = _pinBit;
		}

		void SetLow(void)
		{
			_port.OUTCLR = _pinBit;
		}

		bool IsHigh(void) const
		{
			return (_port.IN & _pinBit) != 0;
		}
		
		bool IsLow(void) const
		{
			return (_port.IN & _pinBit) == 0;
		}
		
	private:
		volatile PORT_t& _port;
		const uint8_t _pinNumber;
		const uint8_t _pinBit;
	};
}
