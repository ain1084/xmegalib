/*
 * USARTInfo.h
 *
 * Created: 2012/08/10 0:11:21
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <avr/io.h>

namespace XMEGA
{
	class USARTInfo
	{
	public:
		USARTInfo(USART_t& usart);	
		
		USART_t& GetUSART(void) const
		{
			return *_pPortInfo->pUSART;
		}
		
		PORT_t& GetPort(void) const
		{
			return *_pPortInfo->pPORT;
		}
		
		uint8_t GetXCKBitPosition(void) const
		{
			return _pPortInfo->bpStart + BitPosition_XCK;
		}

		uint8_t GetXCKBitMask(void) const
		{
			return 1 << GetXCKBitPosition();
		}

		uint8_t GetRXDBitPosition(void) const
		{
			return _pPortInfo->bpStart + BitPosition_RXD;
		}

		uint8_t GetRXDBitMask(void) const
		{
			return 1 << GetRXDBitPosition();
		}

		uint8_t GetTXDBitPosition(void) const
		{
			return _pPortInfo->bpStart + BitPosition_TXD;
		}

		uint8_t GetTXDBitMask(void) const
		{
			return 1 << GetTXDBitPosition();
		}

		DMA_CH_TRIGSRC_t GetRXCDMATriggerSource(void) const
		{
			return _pPortInfo->RXCDMATriggerSource;
		}

		DMA_CH_TRIGSRC_t GetDREDMATriggerSource(void) const
		{
			return _pPortInfo->DREDMATriggerSource;
		}

	private:
		enum BitPosition
		{
			BitPosition_XCK = 0,
			BitPosition_RXD = 1,
			BitPosition_TXD = 2,
		};

		struct PortInfo
		{
			USART_t* pUSART;
			PORT_t* pPORT;
			uint8_t bpStart;
			DMA_CH_TRIGSRC_t RXCDMATriggerSource;
			DMA_CH_TRIGSRC_t DREDMATriggerSource;
		} const* _pPortInfo;
		
		static const PortInfo _portInfos[];
	};
}