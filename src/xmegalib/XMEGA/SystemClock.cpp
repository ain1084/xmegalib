/*
 * SystemClock.cpp
 *
 * Created: 2011/12/08 2:25:05
 *  Author: Seiji Ainoguchi
 */

#include "Platform.h"
#include <stddef.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "ISystemClockNotify.h"
#include "SystemClock.h"

using namespace XMEGA;

SystemClock SystemClock::_instance;

SystemClock::SystemClock(void)
{
	update();
}

bool SystemClock::SetPLL(PLLSource source, uint8_t multiplicationFactor)
{
	if ((CLK_CTRL & CLK_SCLKSEL_gm) == CLK_SCLKSEL_PLL_gc)
	{
		//PLL running
		return false;
	}
	if (multiplicationFactor > (OSC_PLLFAC_gm >> OSC_PLLFAC_gp))
	{
		//multiplication factor is out of range
		return false;
	}

	Oscillator sourceOscillator;
	switch (source)
	{
	case PLLSource_External:
		sourceOscillator = Oscillator_External;
		break;
	case PLLSource_Internal_2MHz:
		sourceOscillator = Oscillator_Internal_2MHz;
		break;
	case PLLSource_Internal_32MHz:
		sourceOscillator = Oscillator_Internal_32MHz;
		break;
	default:
		return false;
	}			
	const OscillatorParameter& param = getOscillatorParameter(sourceOscillator);
	if (!(OSC_STATUS & param.oscStatusMask))
	{
		//not running
		return false;
	}

	OSC_PLLCTRL = param.pllSourceValue | (multiplicationFactor << OSC_PLLFAC_gp);

	_dirty = true;
	return true;
}

void SystemClock::SetPrescaler(CLK_PSADIV_t pacdiv, CLK_PSBCDIV_t pbcdiv)
{
	writeCPP(&CLK_PSCTRL, pacdiv | pbcdiv);
	_dirty = true;
}

bool SystemClock::SetClockSource(Oscillator sourceOscillator)
{
	const OscillatorParameter& param = getOscillatorParameter(sourceOscillator);
	while (!(OSC_STATUS & param.oscStatusMask))
		;
	writeCPP(&CLK_CTRL, param.clockSelectValue);
		
	_dirty = true;
	return true;
}

bool SystemClock::SetExternalOscillatorFrequency(uint32_t frequency)
{
	OSC_FRQRANGE_enum freqRange;
	if (frequency > 12000000)
	{
		freqRange = OSC_FRQRANGE_12TO16_gc;
	}
	else if (frequency > 9000000)
	{
		freqRange = OSC_FRQRANGE_9TO12_gc;
	}
	else if (frequency > 2000000)
	{
		freqRange = OSC_FRQRANGE_2TO9_gc;
	}
	else
	{
		freqRange = OSC_FRQRANGE_04TO2_gc;
	}		

	//Crystal = XTAL_16CLK_gc
	//External Clock (CS2200) = EXTCLK_gc
	OSC_XOSCCTRL = freqRange | OSC_XOSCSEL_EXTCLK_gc;// XTAL_16KCLK_gc;
	_crystalFrequency = frequency;
	_dirty = true;
	return true;
}

void SystemClock::EnableOscillator(Oscillator oscillator)
{
	auto& param = getOscillatorParameter(oscillator);

	OSC_CTRL |= param.oscCtrlMask;
	while (!(OSC_STATUS & param.oscStatusMask))
		;
	_dirty = true;
}

bool SystemClock::DisableOscillator(Oscillator oscillator)
{
	auto& param = getOscillatorParameter(oscillator);
	if ((CLK_CTRL & CLK_SCLKSEL_gm) == param.clockSelectValue)
	{
		//running
		return false;
	}
	
	if ((OSC_STATUS & OSC_PLLEN_bm) && CLK_CTRL == CLK_SCLKSEL_PLL_gc)
	{
		if ((OSC_PLLCTRL & OSC_PLLSRC_gm) == param.pllSourceValue)
		{
			//running
			return false;
		}
	}
	OSC_CTRL &= ~param.oscCtrlMask;
	return true;
}


uint32_t SystemClock::GetSystemClock(void)
{
	update();
	return _systemClock;
}

uint32_t SystemClock::GetCPUClock(void)
{
	update();
	return _peripheralClock;
}

uint32_t SystemClock::GetPeripheralClock(void)
{
	return GetCPUClock();
}

uint32_t SystemClock::GetPeripheral2xClock(void)
{
	update();
	return _peripheral2xClock;
}

uint32_t SystemClock::GetPeripheral4xClock(void)
{
	update();
	return _peripheral4xClock;
}

void SystemClock::AddNotify(ISystemClockNotify& notify)
{
	_notifyContainer.Add(&notify);
}

void SystemClock::RemoveNotify(ISystemClockNotify& notify)
{
	_notifyContainer.Remove(&notify);
}

void SystemClock::NotifyClockUpdated(void)
{
	for (NotifyContainer::IndexType i = 0; i < _notifyContainer.GetCount(); ++i)
	{
		_notifyContainer[i]->OnPeripheralClockChanged(_peripheralClock);
	}
}

void SystemClock::update(void)
{
	if (!_dirty)
	{
		return;
	}
	_dirty = false;
	
	uint8_t sclkSel = CLK_CTRL & CLK_SCLKSEL_gm;
	uint32_t systemClock = 0;
	if (sclkSel == CLK_SCLKSEL_RC2M_gc)
	{
		systemClock = 2000000;
	}
	else if (sclkSel == CLK_SCLKSEL_RC32K_gc)
	{
		systemClock = 32000;
	}
	else if (sclkSel == CLK_SCLKSEL_XOSC_gc)
	{
		systemClock = _crystalFrequency;
	}
	else if (sclkSel == CLK_SCLKSEL_PLL_gc)
	{
		if (OSC_CTRL & OSC_PLLEN_bm)
		{
			uint32_t pllFreq = 0;
			uint8_t pllsrc = OSC_PLLCTRL & OSC_PLLSRC_gm;
			if (pllsrc == OSC_PLLSRC_XOSC_gc)
			{
				pllFreq = _crystalFrequency;
			}
			else if (pllsrc ==	OSC_PLLSRC_RC2M_gc)
			{
				pllFreq = 2000000;
			}
			else if (pllsrc == OSC_PLLSRC_RC32M_gc)
			{
				pllFreq = 32000000;
			}				
			systemClock = pllFreq * (OSC_PLLCTRL & OSC_PLLFAC_gm);
		}
	}
	_systemClock = systemClock;

	uint32_t peripheral4xClock = systemClock;
	uint8_t psadiv = CLK_PSCTRL & CLK_PSADIV_gm;
	static const uint8_t PROGMEM padivToShift[] =
	{
		CLK_PSADIV_1_gc, CLK_PSADIV_2_gc, CLK_PSADIV_4_gc, CLK_PSADIV_8_gc,
		CLK_PSADIV_16_gc, CLK_PSADIV_32_gc, CLK_PSADIV_64_gc, CLK_PSADIV_128_gc,
		CLK_PSADIV_256_gc, CLK_PSADIV_512_gc
	};
	for (uint8_t i = 0; pgm_read_byte_far(&padivToShift[i]) != psadiv; ++i)
	{
		peripheral4xClock >>= 1;
	}
	_peripheral4xClock = peripheral4xClock;

	uint32_t peripheral2xClock = peripheral4xClock;
	uint32_t peripheralClock = peripheral4xClock;
	uint8_t psbcdiv = CLK_PSCTRL & CLK_PSBCDIV_gm;
	if (psbcdiv == CLK_PSBCDIV_4_1_gc)
	{
		peripheral2xClock >>= 2;
	}	
	else if (psbcdiv == CLK_PSBCDIV_1_2_gc)
	{
		peripheralClock >>= 1;
	}
	else if (psbcdiv == CLK_PSBCDIV_2_2_gc)
	{
		peripheral2xClock >>= 1;
		peripheralClock >>= 2;
	}
	_peripheral2xClock = peripheral2xClock;
	_peripheralClock = peripheralClock;
}

void SystemClock::writeCPP(volatile uint8_t* address, uint8_t value)
{
#ifdef RAMPZ
	RAMPZ = 0;
#endif
	asm volatile(
		"movw r30, %0" "\n\t"
		"ldi  r16, %2" "\n\t"
		"out  %3, r16" "\n\t"
		"st    Z,  %1" "\n\t"
		:
		: "r" (address), "r" (value), "M" ((int)CCP_IOREG_gc), "i" (&CCP)
		: "r16", "r30", "r31"
		);
}

const SystemClock::OscillatorParameter& SystemClock::getOscillatorParameter(Oscillator oscillator)
{
	static const OscillatorParameter oscillatorParameters[] =
	{
		{ Oscillator_External, OSC_XOSCEN_bm, OSC_XOSCRDY_bm, CLK_SCLKSEL_XOSC_gc, OSC_PLLSRC_XOSC_gc },
		{ Oscillator_Internal_2MHz, OSC_RC2MEN_bm, OSC_RC2MRDY_bm, CLK_SCLKSEL_RC2M_gc, OSC_PLLSRC_RC2M_gc },
		{ Oscillator_Internal_32MHz, OSC_RC32MEN_bm, OSC_RC32MRDY_bm, CLK_SCLKSEL_RC32M_gc, OSC_PLLSRC_RC32M_gc },
		{ Oscillator_Internal_32KHz, OSC_RC32KEN_bm, OSC_RC32KRDY_bm, CLK_SCLKSEL_RC32K_gc, 0 },
		{ Oscillator_PLL, OSC_PLLEN_bm, OSC_PLLRDY_bm, CLK_SCLKSEL_PLL_gc, 0 },
	};
	const OscillatorParameter* pParameters = oscillatorParameters;
	for (;;)
	{
		if (pParameters->oscillator == oscillator)
		{
			return *pParameters;
		}
		++pParameters;
	}
}
