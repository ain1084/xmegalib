/*
 * PortInfo.h
 *
 * Created: 2012/08/11 0:25:46
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <avr/io.h>

namespace XMEGA
{
	class PortInfo
	{
	public:
		PortInfo(PORT_t& port, uint8_t bp);

		operator PORT_t&() const
		{
			return *_pInfo->pPort;
		}
		
		uint8_t GetPinNumber(void) const
		{
			return _bp;
		}
		
		EVSYS_CHMUX_t GetEvent(void) const
		{
			return static_cast<EVSYS_CHMUX_t>(_pInfo->eventBase + _bp);
		}
		
		volatile register8_t& PinCtrl(void) const
		{
			return *(&_pInfo->pPort->PIN0CTRL + _bp);
		}

	private:
		uint8_t _bp;
		struct Info
		{
			PORT_t* pPort;
			EVSYS_CHMUX_t eventBase;
		};
		const Info* _pInfo;
		static const Info _infos[];
	};
}
