/*
 * Endian.h
 *
 * Created: 2016/02/18 0:23:21
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <stdint.h>

#pragma pack(1, push)

struct LittleUint16
{
	LittleUint16(void)
	{

	}

	LittleUint16(uint16_t value)
	: _value(value)
	{
		
	}

	LittleUint16(const LittleUint16& rhs)
	: _value(rhs._value)
	{
		
	}

	LittleUint16& operator=(uint16_t value)
	{
		_value = value;
		return *this;
	}

	LittleUint16& operator=(const LittleUint16& rhs)
	{
		_value = rhs._value;
		return *this;
	}

	operator uint16_t () const
	{
		return _value;
	}

private:
	uint16_t _value;
};

struct LittleUint32
{		
	LittleUint32(void)
	{

	}
	
	LittleUint32(const LittleUint32& rhs)
	: _value(rhs._value)
	{

	}

	LittleUint32(uint32_t value)
	: _value(value)
	{

	}	

	LittleUint32& operator=(const LittleUint32& rhs)
	{
		_value = rhs._value;
		return *this;
	}

	LittleUint32& operator=(uint32_t value)
	{
		_value = value;
		return *this;
	}

	operator uint32_t () const
	{
		return _value;
	}

private:
	uint32_t _value;
};

#pragma pack(pop)