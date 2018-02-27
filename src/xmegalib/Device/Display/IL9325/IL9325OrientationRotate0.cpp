/*
 * IL9325OrientationRotate0.cpp
 *
 * Created: 2012/09/28 19:47:55
 *  Author: Seiji Ainoguchi
 */ 

#include "IL9325OrientationRotate0.h"
#include "IIL9325ControlPort.h"

using namespace Device::Display::IL9325;

IL9325OrientationRotate0::IL9325OrientationRotate0(IIL9325ControlPort& port)
 : _port(port)
{
	//BGR, I/D[1:0] = 11
	_port.WriteRegister(0x03, 0x1030);
}

int IL9325OrientationRotate0::GetWidth(void) const
{
	return 240;
}

int IL9325OrientationRotate0::GetHeight(void) const
{
	return 320;
}

void IL9325OrientationRotate0::SetWindow(int x, int y, int width, int height)
{
	_port.WriteRegister(0x50, x);
	_port.WriteRegister(0x51, x + width - 1);
	_port.WriteRegister(0x20, x);
	_port.WriteRegister(0x52, y);
	_port.WriteRegister(0x53, y + height - 1);
	_port.WriteRegister(0x21, y);
}
