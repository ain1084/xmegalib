/*
 * MMCDiskIO.h
 *
 * Created: 2011/12/09 0:48:30
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "Platform.h"
#include "IDiskIO.h"
#include "Device/IIntervalTimerNotify.h"

namespace Device
{
	class ISPIChip;
	class IInputPin;
}

namespace FileSystem
{
	namespace FAT
	{
		class MMCDiskIO : public IDiskIO, public Device::IIntervalTimerNotify
		{
		public:
			MMCDiskIO(Device::ISPIChip& spiChip, Device::IInputPin& cardDetect);

			virtual uint8_t /* DSTATUS */ Initialize(void);
			virtual uint8_t /* DSTATUS */ Status(void);
			virtual int /* DRESULT */ Read(uint8_t* buffer, uint32_t address, uint8_t count);
			virtual int /* DRESULT */ IoCtl(uint_fast8_t ctrl, void* buffer);	
			virtual void Pool(void);

			virtual void OnTimer(void);


		private:
			enum CardType
			{
				CardType_Unknown = 0x00,
				CardType_MMC = 0x01,
				CardType_SD1 = 0x02,
				CardType_SD2 = 0x04,
				CardType_SDC = CardType_SD1 | CardType_SD2,
				CardType_BLOCK = 0x08
			};
			CardType _cardType;

			Device::ISPIChip& _spiChip;
			Device::IInputPin& _cardDetect;
			SystemClockCounter::ValueType _timeout10ms;
			bool _isNoInit;
			bool _isNoDisk;

			uint8_t sendCommand(uint8_t cmd, uint32_t argument);
			uint8_t receiveBlock(void* buffer, uint16_t length);
			void select(void);
			void deselect(void);
		};
	}
}
