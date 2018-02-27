/*
 * PushButton.cpp
 *
 * Created: 2012/01/11 0:29:11
 *  Author: Seiji Ainoguchi
 */ 

#include "PushButton.h"

using namespace Device;

PushButton::PushButton(const IInputPin& pin)
 : _pin(pin)
 , _prev(0)
 , _decided(0)
 , _edgePrev(0)
 , _current(0)
{

}

void PushButton::Update(void)
{
	uint8_t now = _pin.IsLow() ? 1 : 0;
	uint8_t exor = ~(now ^ _prev);
	uint8_t f = exor & now;
	uint8_t g = exor & _decided;
	_current = f | g;
	_prev = now;
	_decided = _current;

	_edge = _current & ~_edgePrev;
	_edgePrev = _current;
}
