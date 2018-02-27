/*
 * IL9325GraphicDisplay.cpp
 *
 * Created: 2012/09/28 14:41:19
 *  Author: Seiji Ainoguchi
 */ 

#include "Platform.h"
#include <string.h>
#include "Utilities/ScopedPtr.h"
#include "IL9325GraphicDisplay.h"
#include "IIL9325ControlPort.h"
#include "IL9325OrientationRotate0.h"
#include "IL9325OrientationRotate90.h"
#include "IL9325OrientationRotate180.h"
#include "IL9325OrientationRotate270.h"

using namespace Device;
using namespace Device::Display::IL9325;

IL9325GraphicDisplay::IL9325GraphicDisplay(IIL9325ControlPort& port)
 : _port(port)
 , _pOrientation(nullptr)
{
	initialize();
	SetRotate(Rotate_0);
	Clear();
}

IL9325GraphicDisplay::~IL9325GraphicDisplay(void)
{
	delete _pOrientation;
}

void IL9325GraphicDisplay::SetRotate(Rotate rotate)
{
	delete _pOrientation;
	_pOrientation = nullptr;
	
	switch (rotate)
	{
	case Rotate_0:
		_pOrientation = new IL9325OrientationRotate0(_port);
		break;
	case Rotate_90:	
		_pOrientation = new IL9325OrientationRotate90(_port);
		break;
	case Rotate_180:
		_pOrientation = new IL9325OrientationRotate180(_port);
	break;
	case Rotate_270:
		_pOrientation = new IL9325OrientationRotate270(_port);
		break;
	}		
}

void IL9325GraphicDisplay::Fill(int x, int y, int width, int height, uint16_t color)
{
	_pOrientation->SetWindow(x, y, width, height);
	_port.WriteRegisterRepeat(0x22, color, (uint32_t)width * height);
}

void IL9325GraphicDisplay::Rectangle(int x, int y, int width, int height, uint16_t color)
{
	_pOrientation->SetWindow(x, y, width, 1);
	_port.WriteRegisterRepeat(0x22, color, width);
	_pOrientation->SetWindow(x, y, 1, height);
	_port.WriteRegisterRepeat(0x22, color, height);
	_pOrientation->SetWindow(x + width - 1, y, 1, height);
	_port.WriteRegisterRepeat(0x22, color, height);
	_pOrientation->SetWindow(x, y + height - 1, width, 1);
	_port.WriteRegisterRepeat(0x22, color, width);
}

void IL9325GraphicDisplay::Blt(int x, int y, int width, int height, const uint16_t data[])
{
	_pOrientation->SetWindow(x, y, width, height);
	_port.WriteRegister(0x22, data, width * height);
}

void IL9325GraphicDisplay::DrawMonochromePattern(int dx, int dy ,int width, int height, const uint8_t data[])
{
	uint16_t foregroundColor = 0xFFFF;
	uint16_t backgroundColor = 0x0000;
	
	int h = height;
	uint16_t* pBuffer = getBuffer<uint16_t>(width * height);

	unsigned i = 0;
	uint16_t* pb = pBuffer;
	do 
	{
		uint8_t widthByteCount = width >> 3;
		for (uint8_t j = 0; j < widthByteCount; ++j)
		{
			uint8_t d = data[i++];
			*pb++ = (d & 0x80) ? foregroundColor : backgroundColor;
			*pb++ = (d & 0x40) ? foregroundColor : backgroundColor;
			*pb++ = (d & 0x20) ? foregroundColor : backgroundColor;
			*pb++ = (d & 0x10) ? foregroundColor : backgroundColor;
			*pb++ = (d & 0x08) ? foregroundColor : backgroundColor;
			*pb++ = (d & 0x04) ? foregroundColor : backgroundColor;
			*pb++ = (d & 0x02) ? foregroundColor : backgroundColor;
			*pb++ = (d & 0x01) ? foregroundColor : backgroundColor;
		}
		uint8_t widthBitCount = width & 0x7;
		if (widthBitCount != 0)
		{
			uint8_t mask = 0x80;
			uint8_t d = data[i++];
			do 
			{
				*pb++ = (d & mask) ? foregroundColor : backgroundColor;
			} while ((mask >>= 1) != 0);
		}			
	} while (--height != 0);
	Blt(dx, dy, width, h, pBuffer);
}

void IL9325GraphicDisplay::DrawTestPattern(int x, int y, int width, int height)
{
	uint16_t* pb = getBuffer<uint16_t>(width);

	unsigned divX = width - 1;
	unsigned divY = height - 1;
	for (int j = 0; j < height; ++j)
	{
		uint16_t g = static_cast<unsigned>(j) * 63 / divY;
		uint16_t gb = static_cast<uint16_t>(g) << 5 | (31 - g / 2);		
		
		for (int i = 0; i < width; ++i)
		{
			uint8_t r = static_cast<unsigned>(i) * 31 / divX;
			*(pb + i) = ((uint16_t)r << 11) | gb;
		}
		Blt(x, y + j, width, 1, pb);
	}
}

void IL9325GraphicDisplay::DisplayOn(void)
{
	
}

void IL9325GraphicDisplay::DisplayOff(void)
{
	
}

void IL9325GraphicDisplay::Clear(void)
{
	Fill(0, 0, GetWidth(), GetHeight(), 0x0000);	
}

void IL9325GraphicDisplay::Refresh(void)
{
	
}

uint_fast16_t IL9325GraphicDisplay::GetWidth() const
{
	return _pOrientation->GetWidth();
}

uint_fast16_t IL9325GraphicDisplay::GetHeight() const
{
	return _pOrientation->GetHeight();
}

void IL9325GraphicDisplay::SetForegroundColor(uint_fast8_t color)
{
	
}

void IL9325GraphicDisplay::SetBackgroundColor(uint_fast8_t color)
{
	
}

void IL9325GraphicDisplay::DrawLinePattern(int left, int top, int width, int height, const uint8_t* pData)
{
	uint16_t foregroundColor = 0x00FF;
	uint16_t backgroundColor = 0x0000;

	uint16_t* pBuffer = getBuffer<uint16_t>(width);

	// extract line pattern
	uint16_t* pb = pBuffer;
	uint8_t widthByteCount = width >> 3;
	for (uint8_t j = 0; j < widthByteCount; ++j)
	{
		uint8_t d = *pData++;
		*pb++ = (d & 0x80) ? foregroundColor : backgroundColor;
		*pb++ = (d & 0x40) ? foregroundColor : backgroundColor;
		*pb++ = (d & 0x20) ? foregroundColor : backgroundColor;
		*pb++ = (d & 0x10) ? foregroundColor : backgroundColor;
		*pb++ = (d & 0x08) ? foregroundColor : backgroundColor;
		*pb++ = (d & 0x04) ? foregroundColor : backgroundColor;
		*pb++ = (d & 0x02) ? foregroundColor : backgroundColor;
		*pb++ = (d & 0x01) ? foregroundColor : backgroundColor;
	}
	uint8_t widthBitCount = width & 0x7;
	if (widthBitCount != 0)
	{
		uint8_t mask = 0x80;
		uint8_t d = *pData++;
		do 
		{
			*pb++ = (d & mask) ? foregroundColor : backgroundColor;
		} while ((mask >>= 1) != 0);
	}
	
	do 
	{
		Blt(left, top++, width, 1, pBuffer);
	} while (--height != 0);
}

void IL9325GraphicDisplay::initialize(void)
{
	static const struct RegisterWriteData
	{
		uint8_t registerIndex;
		uint16_t value;
		uint8_t waitMilliseconds;
	} initData[] =
	{
		{ 0x00, 0x0001,  50 },
		{ 0x01, 0x0100,   0 },
		{ 0x02, 0x0700,   0 },
		{ 0x03, 0x1030,   0 },
		{ 0x04, 0x0000,   0 },
		{ 0x08, 0x0202,   0 },
		{ 0x09, 0x0000,   0 },
		{ 0x0a, 0x0000,   0 },
		{ 0x0c, 0x0000,   0 },
		{ 0x0d, 0x0000,   0 },
		{ 0x0f, 0x0000,   0 },
		{ 0x10, 0x0000,   0 },
		{ 0x11, 0x0007,   0 },
		{ 0x12, 0x0000,   0 },
		{ 0x13, 0x0000, 200 },
		{ 0x10, 0x1690,   0 },
		{ 0x11, 0x0227,  50 },
		{ 0x12, 0x001a,  50 },
		{ 0x13, 0x1800,   0 },
		{ 0x29, 0x002a,  50 },
		{ 0x30, 0x0000,   0 },
		{ 0x31, 0x0000,   0 },
		{ 0x32, 0x0000,   0 },
		{ 0x35, 0x0206,   0 },
		{ 0x36, 0x0808,   0 },
		{ 0x37, 0x0007,   0 },
		{ 0x38, 0x0201,   0 },
		{ 0x39, 0x0000,   0 },
		{ 0x3c, 0x0000,   0 },
		{ 0x3d, 0x0000,   0 },
		{ 0x50, 0x0000,   0 },
		{ 0x51, 0x00ef,   0 },
		{ 0x52, 0x0000,   0 },
		{ 0x53, 0x013f,   0 },
		{ 0x60, 0xa700,   0 },
		{ 0x61, 0x0003,   0 },
		{ 0x6a, 0x0000,   0 },
		{ 0x90, 0x0010,   0 },
		{ 0x92, 0x0000,   0 },
		{ 0x93, 0x0003,   0 },
		{ 0x95, 0x1100,   0 },
		{ 0x97, 0x0000,   0 },
		{ 0x98, 0x0000,   0 },
		{ 0x07, 0x0133,   0 },
	};
	
	SystemClockCounter::ValueType clockCount = SystemClockCounter::MStoClock(1);

	for (unsigned i = 0; i < sizeof(initData) / sizeof(initData[0]); ++i)
	{
		_port.WriteRegister(initData[i].registerIndex, initData[i].value);
		if (initData[i].waitMilliseconds != 0)
		{
			SystemClockTimer(clockCount * initData[i].waitMilliseconds).WaitForReached();
		}
	}
}