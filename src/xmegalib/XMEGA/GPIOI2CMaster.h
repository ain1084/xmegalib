/*
 * GPIOI2CMaster.h
 *
 * Created: 2017/09/07 22:10:08
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <avr/io.h>

#include <Device/II2CMaster.h>

namespace XMEGA
{
	class GPIO;
	
	class GPIOI2CMaster : public Device::II2CMaster
	{
	public:
		GPIOI2CMaster(GPIO& scl, GPIO& sda);
		virtual ~GPIOI2CMaster(void);
		
		virtual bool Write(uint8_t address, const uint8_t buffer[], unsigned length);
		virtual bool Read(uint8_t address, uint8_t buffer[], unsigned length);
		virtual bool WriteRead(uint8_t address, const uint8_t write[], unsigned writeLength, uint8_t read[], unsigned readLength);
		virtual void Stop(void);

	private:
		enum State
		{
			Started,
			Stopped,
		};
	
		GPIO& _scl;
		GPIO& _sda;
		State _state;
		
		void startCondition(void);
		void writeByte(uint8_t data);
		uint8_t readByte(void);
		bool readAcknowledge(void);
		void stopCondition(void);
	};
}