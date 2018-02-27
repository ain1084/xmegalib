/*
 * IIOPort.h
 *
 * Created: 2012/11/22 1:49:38
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

namespace Device
{
	class IIOPort
	{
	public:
		virtual void SetDirection(uint8_t inputBitMasks) = 0;
		virtual uint8_t GetDirection(void) const = 0;
		virtual void SetPullup(uint8_t bitMasks) = 0;
		virtual uint8_t GetPullup(void) const = 0;
		virtual void SetData(uint8_t data) = 0;
		virtual uint8_t GetData(void) const = 0;
	};
}	
