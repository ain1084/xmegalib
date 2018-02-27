/*
 * IL9325OrientationRotate180.cpp
 *
 * Created: 2012/09/28 19:47:55
 *  Author: Seiji Ainoguchi
 */ 

#include "IL9325OrientationRotate180.h"
#include "IIL9325ControlPort.h"

using namespace Device::Display::IL9325;

IL9325OrientationRotate180::IL9325OrientationRotate180(IIL9325ControlPort& port)
 : _port(port)
{
	//BGR, I/D[1:0] = 00
	_port.WriteRegister(0x03, 0x1000);
}

int IL9325OrientationRotate180::GetWidth(void) const
{
	return 240;
}

int IL9325OrientationRotate180::GetHeight(void) const
{
	return 320;
}

void IL9325OrientationRotate180::SetWindow(int x, int y, int width, int height)
{
	y = 319 - y;
	x = 239 - x;
	
	_port.WriteRegister(0x50, x - width + 1);
	_port.WriteRegister(0x51, x);
	_port.WriteRegister(0x20, x);
	_port.WriteRegister(0x52, y - height + 1);
	_port.WriteRegister(0x53, y);
	_port.WriteRegister(0x21, y);
}
