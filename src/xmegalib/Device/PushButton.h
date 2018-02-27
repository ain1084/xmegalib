/*
 * PushButton.h
 *
 * Created: 2012/01/11 0:26:32
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include <stdint.h>
#include "IInputPin.h"

namespace Device
{
	class PushButton
	{
	public:

		PushButton(const IInputPin& pin);
		void Update(void);
		bool GetCurrentState(void) const
		{
			return _current & 0x1;
		}

		bool GetEdgeState(void) const
		{
			return _edge & 0x1;
		}

	private:
		const IInputPin& _pin;
		uint8_t _prev;
		uint8_t _decided;
		uint8_t _edgePrev;
		uint8_t _current;
		uint8_t _edge;
	};
}
