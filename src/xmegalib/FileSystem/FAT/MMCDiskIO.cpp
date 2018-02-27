/*
 * MMCDiskIO.cpp
 *
 * Created: 2011/12/09 0:50:14
 *  Author: Seiji Ainoguchi
 */ 

#include "diskio.h"
#include "Device/ISPIChip.h"
#include "Device/IInputPin.h"
#include "MMCDiskIO.h"

using namespace Device;
using namespace FileSystem::FAT;

namespace
{
	/* Definitions for MMC/SDC command */
	const uint8_t CMD0		= 0;		/* GO_IDLE_STATE */
	const uint8_t CMD1		= 1;		/* SEND_OP_COND (MMC) */
	const uint8_t ACMD41	= 0x80+41;	/* SEND_OP_COND (SDC) */
	const uint8_t CMD8		= 8;		/* SEND_IF_COND */
	const uint8_t CMD9		= 9;		/* SEND_CSD */
	const uint8_t CMD10		= 10;		/* SEND_CID */
	const uint8_t CMD12		= 12;		/* STOP_TRANSMISSION */
	const uint8_t ACMD13	= 0x80+13;	/* SD_STATUS (SDC) */
	const uint8_t CMD16		= 16;		/* SET_BLOCKLEN */
	const uint8_t CMD17		= 17;		/* READ_SINGLE_BLOCK */
	const uint8_t CMD18		= 18;		/* READ_MULTIPLE_BLOCK */
	const uint8_t CMD23		= 23;		/* SET_BLOCK_COUNT (MMC) */
	const uint8_t ACMD23	= 0x80+23;	/* SET_WR_BLK_ERASE_COUNT (SDC) */
	const uint8_t CMD24		= 24;		/* WRITE_BLOCK */
	const uint8_t CMD25		= 25;		/* WRITE_MULTIPLE_BLOCK */
	const uint8_t CMD55		= 55;		/* APP_CMD */
	const uint8_t CMD58		= 58;		/* READ_OCR */

	/* Card type flags (CardType) */
	const uint8_t CT_MMC	= 0x01;		/* MMC ver 3 */
	const uint8_t CT_SD1	= 0x02;		/* SD ver 1 */
	const uint8_t CT_SD2	= 0x04;		/* SD ver 2 */
	const uint8_t CT_SDC	= CT_SD1|CT_SD2;	/* SD */
	const uint8_t CT_BLOCK	= 0x08;		/* Block addressing */
}

void MMCDiskIO::OnTimer(void)
{
	Pool();
}

MMCDiskIO::MMCDiskIO(ISPIChip& spiChip, IInputPin& cardDetect)
 : _spiChip(spiChip)
 , _cardDetect(cardDetect)
 , _timeout10ms(SystemClockCounter::MStoClock(1))
 , _isNoInit(true)
 , _isNoDisk(true)
{
	Pool();
}

uint_fast8_t MMCDiskIO::Initialize(void)
{
	CriticalSection cs;

	if (_isNoDisk)
	{
		return STA_NODISK;
	}
	_cardType = CardType_Unknown;

	_spiChip.Inactive();
	_spiChip.SetFrequency(400000);
	SystemClockTimer timer(_timeout10ms);
	SystemClockTimer::WaitForReached(_timeout10ms);

	_spiChip.SendClock(512 * 2);
	select();

	bool cmd0succeeded = false;
	for (unsigned n = 0; n < 1000; ++n)
	{
		if (sendCommand(CMD0, 0) == 1)
		{
			cmd0succeeded = true;
			break;
		}			
	}
	
	if (cmd0succeeded)
	{
		if (sendCommand(CMD8, 0x1AA) == 1)
		{
			uint8_t ocr[4];
			_spiChip.Read(ocr, 4);
			if (ocr[2] == 0x01 && ocr[3] == 0xAA)
			{
				bool acmd41accepted = false;			
				for (unsigned n = 0; n < 1000 && !acmd41accepted; n++)
				{
					timer.Reset();
					do
					{
						if (sendCommand(ACMD41, 1UL << 30) == 0)
						{
							acmd41accepted = true;
							if (sendCommand(CMD58, 0) == 0)
							{
								_spiChip.Read(ocr, 4);
								_cardType = static_cast<CardType>((ocr[0] & 0x40) ? CardType_SD2|CardType_BLOCK : CardType_SD2);
							}
							break;
						}
					} while (!timer.IsReached());
				}
			}
		}
		else
		{
			uint8_t cmd;
			if (sendCommand(ACMD41, 0) <= 1)
			{
				_cardType = CardType_SD1;
				cmd = ACMD41;
			}
			else
			{
				_cardType = CardType_MMC;
				cmd = CMD1;
			}
			
			for (unsigned n = 0; n < 1000; n++)
			{
				timer.Reset();
				do 
				{
					if (sendCommand(cmd, 0) == 0)
					{
						break;
					}					
				} while (!timer.IsReached());
			}
			if (sendCommand(CMD16, 512) != 0)
			{
				_cardType = CardType_Unknown;
			}
		}				
	}
	deselect();
	if (_cardType != CardType_Unknown)
	{
		_isNoInit = false;		
		_spiChip.SetFrequency(25000000);
		return 0;
	}
	return STA_NOINIT;
}	

uint_fast8_t MMCDiskIO::Status(void)
{
	CriticalSection cs;
	uint_fast8_t result = 0;
	if (_isNoInit)
	{
		result |= STA_NOINIT;
	}
	if (_isNoDisk)
	{
		result |= STA_NODISK;
	}
	return result;
}	
	
int MMCDiskIO::Read(uint8_t* buffer, uint32_t sector, uint_fast8_t count)
{
	if (count == 0)
	{
		return RES_PARERR;
	}
	
	if (!(_cardType & CardType_BLOCK))
	{
		sector *= 512;
	}	
	if (count == 1)
	{
		if (sendCommand(CMD17, sector) == 0)
		{
			if (receiveBlock(buffer, 512) != 0)
			{
				count = 0;
			}
		}
	}
	else
	{
		if (sendCommand(CMD18, sector) == 0)
		{
			do
			{
				if (receiveBlock(buffer, 512) == 0)
				{
					break;
				}
				buffer += 512;
			} while (--count != 0);
			sendCommand(CMD12, 0);
		}
	}
	deselect();
	return count != 0 ? RES_ERROR : RES_OK;
}

void MMCDiskIO::Pool(void)
{
	CriticalSection cs;
	if (_cardDetect.IsLow())
	{
		_isNoDisk = false;
	}
	else
	{
		_isNoDisk = true;
		_isNoInit = true;
	}
}

void MMCDiskIO::deselect(void)
{
	_spiChip.Inactive();
	_spiChip.Read();
}

void MMCDiskIO::select(void)
{
	_spiChip.Active();
}

uint8_t MMCDiskIO::sendCommand(uint8_t cmd, uint32_t argument)
{
	if (cmd & 0x80)
	{
		cmd &= 0x7F;
		uint8_t res = sendCommand(CMD55, 0);
		if (res > 1)
		{
			return res;
		}
	}
	deselect();
	select();

	uint8_t data[6];
	data[0] = 0x040 | cmd;
	data[1] = static_cast<uint8_t>(argument >> 24);
	data[2] = static_cast<uint8_t>(argument >> 16);
	data[3] = static_cast<uint8_t>(argument >>  8);
	data[4] = static_cast<uint8_t>(argument);
	if (cmd == CMD0)
	{
		data[5] =0x95;		
	}
	else if (cmd == CMD8)
	{
		data[5] = 0x87;
	}
	else
	{
		data[5] = 0x01;
	}

	_spiChip.Write(data, sizeof(data));

	if (cmd == CMD12)
	{
		_spiChip.Read();
	}

	uint8_t res = 0xFF;
//	SystemClockTimer timer(_timeout10ms);
	for (uint_fast8_t i = 0; i < 200; ++i)
	{
//		timer.Reset();
//		do 
//		{
				res = _spiChip.Read();
				if (!(res & 0x80))
				{
					return res;
				}
//		} while (!timer.IsReached());
	}			
	return res;
}

uint8_t MMCDiskIO::receiveBlock(void* buffer, uint16_t length)
{
	uint8_t* pBuf = static_cast<uint8_t*>(buffer);
	
	for (unsigned u = 0; u < 65535; u++)
	{
		uint8_t token = _spiChip.Read();
		if (token == 0xFE)
		{
			_spiChip.Read(pBuf, length);
			_spiChip.SendClock(2);
			return 1;
		}
	}		
	return 0;
}

int MMCDiskIO::IoCtl(uint_fast8_t ctrl, void* buffer)
{
	return RES_PARERR;
}
