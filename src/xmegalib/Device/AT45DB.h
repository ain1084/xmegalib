/*
 * AT45DB.h
 *
 * Created: 2012/04/22 23:24:58
 *  Author: Seiji Ainoguchi
 */ 

#include "IEEPROM.h"

namespace Device
{
	class ISPIChip;
	
	class AT45DB : public IEEPROM
	{
	public:
		static IEEPROM* Create(ISPIChip& chip);
	
		virtual uint_fast32_t GetCapacity(void) const;
		virtual uint_fast8_t  ReadByte(uint_fast32_t address) const;
		virtual void Read(uint_fast32_t address, void* dest, unsigned length) const;

		virtual const char* GetName(void) const;
		virtual uint16_t GetPageSize(void) const;
		virtual uint32_t GetBlockSize(void) const;
		virtual bool IsReady(void) const;
		virtual void WaitReady(void) const;
		virtual void EraseChip(void);
		virtual void EraseBlock(uint32_t address);
		virtual void WritePageBuffer(uint_fast16_t offset, const void* buffer, unsigned length);
		virtual void WritePage(uint32_t address);
		
	private:
		struct DeviceInfo
		{
			uint32_t Capacity;
			uint16_t PageSize;
			uint16_t BlockSize;
			const char* pName;
		};

		static const struct SupportedDevice
		{
			uint8_t deviceId1;
			uint8_t deviceId2;
			const DeviceInfo deviceInfo;
		} _supportedDevices[];
		
		ISPIChip& _chip;
		const DeviceInfo& _deviceInfo;
		
		static uint8_t readDeviceID(ISPIChip& chip, uint8_t& deviceId1, uint8_t& deviceId2);
		void writeCommand(const uint8_t data[], unsigned length);
		void beginRead(uint32_t address) const;
		
		AT45DB(ISPIChip& chip, const DeviceInfo& deviceInfo);
	};
}