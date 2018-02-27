/*
 * IL9325OrientationRotate270.cpp
 *
 * Created: 2012/09/28 19:47:55
 *  Author: Seiji Ainoguchi
 */ 

#include "IL9325OrientationRotate270.h"
#include "IIL9325ControlPort.h"

using namespace Device::Display::IL9325;

IL9325OrientationRotate270::IL9325OrientationRotate270(IIL9325ControlPort& port)
 : _port(port)
{
	//BGR, I/D[1:0] = 10, AM
	_port.WriteRegister(0x03, 0x1028);
}

int IL9325OrientationRotate270::GetWidth(void) const
{
	return 320;
}

int IL9325OrientationRotate270::GetHeight(void) const
{
	return 240;
}

void IL9325OrientationRotate270::SetWindow(int x, int y, int width, int height)
{
	y = 239 - y;

	_port.WriteRegister(0x51, y);
	_port.WriteRegister(0x20, y);
	_port.WriteRegister(0x50, y - height + 1);
	_port.WriteRegister(0x52, x);
	_port.WriteRegister(0x21, x);
	_port.WriteRegister(0x53, x + width - 1);
}
