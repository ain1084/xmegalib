/*
 * USARTInfo.cpp
 *
 * Created: 2012/08/10 0:13:32
 *  Author: Seiji Ainoguchi
 */ 

#include "USARTInfo.h"

using namespace XMEGA;

const USARTInfo::PortInfo USARTInfo::_portInfos[] =
{
	{ &USARTC0, &PORTC, PIN1_bp, DMA_CH_TRIGSRC_USARTC0_RXC_gc, DMA_CH_TRIGSRC_USARTC0_DRE_gc },
	#if defined(USARTC1) && defined(PORTC)
	{ &USARTC1, &PORTC, PIN5_bp, DMA_CH_TRIGSRC_USARTC1_RXC_gc, DMA_CH_TRIGSRC_USARTC1_DRE_gc },
	#endif
	#if defined(USARTD0) && defined(PORTD)
	{ &USARTD0, &PORTD, PIN1_bp, DMA_CH_TRIGSRC_USARTD0_RXC_gc, DMA_CH_TRIGSRC_USARTD0_DRE_gc },
	#endif
	#if defined(USARTD1) && defined(PORTD)
	{ &USARTD1, &PORTD, PIN5_bp, DMA_CH_TRIGSRC_USARTD1_RXC_gc, DMA_CH_TRIGSRC_USARTD1_DRE_gc },
	#endif
	#if defined(USARTE0) && defined(PORTE)
	{ &USARTE0, &PORTE, PIN1_bp, DMA_CH_TRIGSRC_USARTE0_RXC_gc, DMA_CH_TRIGSRC_USARTE0_DRE_gc },
	#endif
	#if defined(USARTE1) && defined(PORTE)
	{ &USARTE1, &PORTE, PIN5_bp, DMA_CH_TRIGSRC_USARTE1_RXC_gc, DMA_CH_TRIGSRC_USARTE1_DRE_gc },
	#endif
	#if defined(USARTF0) && defined(PORTF)
	{ &USARTF0, &PORTF, PIN1_bp, DMA_CH_TRIGSRC_USARTF0_RXC_gc, DMA_CH_TRIGSRC_USARTF0_DRE_gc },
	#endif
};

USARTInfo::USARTInfo(volatile USART_t& usart)
{
	for (const PortInfo* p = _portInfos;; p++)
	{
		if (p->pUSART == &usart)
		{
			_pPortInfo = p;
			break;
		}
	}
}