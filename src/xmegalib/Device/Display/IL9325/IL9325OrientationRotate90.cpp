/*
 * IL9325OrientationRotate90.cpp
 *
 * Created: 2012/09/28 19:47:55
 *  Author: Seiji Ainoguchi
 */ 

#include "IL9325OrientationRotate90.h"
#include "IIL9325ControlPort.h"

using namespace Device::Display::IL9325;

IL9325OrientationRotate90::IL9325OrientationRotate90(IIL9325ControlPort& port)
 : _port(port)
{
	//BGR, I/D[1:0] = 01, AM
	_port.WriteRegister(0x03, 0x1018);
}

int IL9325OrientationRotate90::GetWidth(void) const
{
	return 320;
}

int IL9325OrientationRotate90::GetHeight(void) const
{
	return 240;
}

void IL9325OrientationRotate90::SetWindow(int x, int y, int width, int height)
{
	x = 319 - x;
	
	_port.WriteRegister(0x50, y);
	_port.WriteRegister(0x20, y);
	_port.WriteRegister(0x51, y + height - 1);
	_port.WriteRegister(0x53, x);
	_port.WriteRegister(0x21, x);
	_port.WriteRegister(0x52, x - width + 1);
}
