/*
 * I2CMaster.cpp
 *
 * Created: 2012/04/30 0:24:14
 *  Author: Seiji Ainoguchi
 */ 

#include "Platform.h"
#include "SystemClock.h"
#include "I2CMaster.h"

using namespace XMEGA;

namespace
{
	const uint32_t BUS_FREQUENCY_DEFAULT = 100000;
}

I2CMaster::I2CMaster(TWI_t& twi, uint32_t busFrequency)
 : _twiMaster(twi.MASTER)
 , _busFrequency(busFrequency == 0 ? BUS_FREQUENCY_DEFAULT : busFrequency)
{
	SystemClock::GetInstance().AddNotify(*this);

	_twiMaster.CTRLA = 0;
	_twiMaster.CTRLB = 0;//TWI_MASTER_SMEN_bm;
	_twiMaster.CTRLA = TWI_MASTER_ENABLE_bm;
	_twiMaster.STATUS = TWI_MASTER_BUSSTATE_IDLE_gc;
	applyBaudrate();
}

I2CMaster::~I2CMaster(void)
{
	SystemClock::GetInstance().RemoveNotify(*this);
}

bool I2CMaster::WriteRead(uint8_t address, const uint8_t write[], unsigned writeLength, uint8_t read[], unsigned readLength)
{
	const uint8_t* pWriteBuffer = write;
	_twiMaster.ADDR = (address << 1) | 0;
	while (!(_twiMaster.STATUS & TWI_MASTER_WIF_bm))
		;

	if ((_twiMaster.STATUS & TWI_MASTER_RXACK_bm))
	{
		return false;
	}
	do 
	{
		_twiMaster.DATA = *pWriteBuffer++;
		while (!(_twiMaster.STATUS & TWI_MASTER_WIF_bm))
			;
		if ((_twiMaster.STATUS & TWI_MASTER_RXACK_bm))
		{
			return false;
		}
	} while (--writeLength != 0);

	uint8_t* pReadBuffer = read;
	
	if (readLength == 0)
	{
		return true;
	}

	_twiMaster.ADDR = (address << 1) | 1;
	for (;;) 
	{
		for (;;)
		{
			uint8_t status = _twiMaster.STATUS;
			if (status & TWI_MASTER_RIF_bm)
			{
				break;
			}
			if (status & TWI_MASTER_RXACK_bm)
			{
				return false;
			}
		}
		*pReadBuffer++ = _twiMaster.DATA;
		if (--readLength == 0)
		{
			_twiMaster.CTRLC = TWI_MASTER_CMD_STOP_gc | TWI_MASTER_ACKACT_bm;
			break;
		}
		else
		{
			_twiMaster.CTRLC = TWI_MASTER_CMD_RECVTRANS_gc;
		}
	}

	while ((_twiMaster.STATUS & TWI_MASTER_BUSSTATE_gm) != TWI_MASTER_BUSSTATE_IDLE_gc)
		;

	return true;
}

bool I2CMaster::Write(uint8_t address, const uint8_t buffer[], unsigned length)
{
	const uint8_t* pBuffer = buffer;
	_twiMaster.STATUS = TWI_MASTER_WIF_bm;
	_twiMaster.ADDR = (address << 1) | 0;
	while (!(_twiMaster.STATUS & TWI_MASTER_WIF_bm))
		;

	if ((_twiMaster.STATUS & TWI_MASTER_RXACK_bm))
	{
		return false;
	}
	
	do 
	{
		_twiMaster.DATA = *pBuffer++;
		while (!(_twiMaster.STATUS & TWI_MASTER_WIF_bm))
			;
		if ((_twiMaster.STATUS & TWI_MASTER_RXACK_bm))
		{
			return false;
		}
	} while (--length != 0);

	return true;
}	

bool I2CMaster::Read(uint8_t address, uint8_t buffer[], unsigned length)
{
	uint8_t* pBuffer = buffer;
	
	if (length == 0)
	{
		return true;
	}

	_twiMaster.ADDR = (address << 1) | 1;
	for (;;) 
	{
		for (;;)
		{
			uint8_t status = _twiMaster.STATUS;
			if (status & TWI_MASTER_RIF_bm)
			{
				break;
			}
			if (status & TWI_MASTER_RXACK_bm)
			{
				return false;
			}
		}
		*pBuffer++ = _twiMaster.DATA;
		if (--length == 0)
		{
			_twiMaster.CTRLC = TWI_MASTER_CMD_STOP_gc | TWI_MASTER_ACKACT_bm;
			break;
		}
		else
		{
			_twiMaster.CTRLC = TWI_MASTER_CMD_RECVTRANS_gc;
		}
	}		
	return true;
}
	
void I2CMaster::Stop(void)
{
	_twiMaster.CTRLC = TWI_MASTER_CMD_STOP_gc;
	while ((_twiMaster.STATUS & TWI_MASTER_BUSSTATE_gm) != TWI_MASTER_BUSSTATE_IDLE_gc)
		;
}

void I2CMaster::applyBaudrate(void)
{
	static const uint8_t BAUD_SLOW = 0xFF;
	static const uint8_t BAUD_FAST = 0x00;
	
	int32_t baudL = (SystemClock::GetInstance().GetPeripheralClock() + _busFrequency * 2 - 1) / (_busFrequency * 2) - 5;
	uint8_t baud;
	if (baudL < 0)
	{
		baud = BAUD_FAST;
	}
	else if (baudL > BAUD_SLOW)
	{
		baud = BAUD_SLOW;
	}	
	else
	{
		baud = static_cast<uint8_t>(baudL);
	}
	_twiMaster.BAUD = baud;
}

void I2CMaster::OnPeripheralClockChanged(uint32_t newClock)
{
//	_twiMaster.CTRLA = 0;
	applyBaudrate();
//	_twiMaster.CTRLA = TWI_MASTER_ENABLE_bm;
//	_twiMaster.STATUS = TWI_MASTER_BUSSTATE_IDLE_gc;
}
