/*
 * IL9325OrientationRotate270.h
 *
 * Created: 2012/09/28 19:44:04
 *  Author: Seiji Ainoguchi
 */ 

#include "Platform.h"
#include "IIL9325Orientation.h"

#pragma once

namespace Device
{
	namespace Display
	{
		namespace IL9325
		{
			class IIL9325ControlPort;
				
			class IL9325OrientationRotate270 : public IIL9325Orientation
			{
			public:
				IL9325OrientationRotate270(IIL9325ControlPort& port);
			
				virtual int GetWidth(void) const;
				virtual int GetHeight(void) const;
				virtual void SetWindow(int x, int y, int width, int height);
				
			private:
				IIL9325ControlPort& _port;
			};
		}
	}
}				