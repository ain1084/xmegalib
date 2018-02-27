/*
 * SystemClock.h
 *
 * Created: 2011/12/08 2:20:05
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <avr/io.h>
#include "Collections/Vector.h"

namespace XMEGA
{
	class ISystemClockNotify;

	class SystemClock
	{
	public:
		inline static SystemClock& GetInstance(void)
		{
			return _instance;
		}
	
		enum Oscillator
		{
			Oscillator_PLL,
			Oscillator_External,
			Oscillator_Internal_32KHz,
			Oscillator_Internal_32MHz,
			Oscillator_Internal_2MHz
		};
	
		enum PLLSource
		{
			PLLSource_Internal_2MHz,
			PLLSource_Internal_32MHz,
			PLLSource_External
		};
	
		bool SetExternalOscillatorFrequency(uint32_t crystalFrequency);
		void EnableOscillator(Oscillator oscillator);
		bool DisableOscillator(Oscillator oscillator);
		bool SetPLL(PLLSource source, uint8_t multiplicationFactor);
		void SetPrescaler(CLK_PSADIV_t pacdiv, CLK_PSBCDIV_t pbcdiv);
		bool SetClockSource(Oscillator sourceOscillator);

		void AddNotify(ISystemClockNotify& notify);
		void RemoveNotify(ISystemClockNotify& notify);
		void NotifyClockUpdated(void);

		uint32_t GetSystemClock(void);
		uint32_t GetCPUClock(void);
		uint32_t GetPeripheralClock(void);
		uint32_t GetPeripheral2xClock(void);
		uint32_t GetPeripheral4xClock(void);
		
	private:
		SystemClock(void);
		static SystemClock _instance;

		bool _dirty;
		uint32_t _crystalFrequency;
		uint32_t _systemClock;
		uint32_t _peripheral4xClock;
		uint32_t _peripheral2xClock;
		uint32_t _peripheralClock;
		
		typedef Collections::Vector<ISystemClockNotify*> NotifyContainer;
		NotifyContainer _notifyContainer;

		void update(void);

		void writeCPP(volatile uint8_t* address, uint8_t value);

		struct OscillatorParameter
		{
			Oscillator oscillator;
			uint8_t oscCtrlMask;
			uint8_t oscStatusMask;
			uint8_t clockSelectValue;
			uint8_t pllSourceValue;
		};
		
		static const OscillatorParameter& getOscillatorParameter(Oscillator oscillator);
	};
}
