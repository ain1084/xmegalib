/*
 * IIL9325Orientation.h
 *
 * Created: 2012/09/28 17:25:14
 *  Author: Seiji Ainoguchi
 */ 

#include "Platform.h"

#pragma once

namespace Device
{
	namespace Display
	{
		namespace IL9325
		{
			class IIL9325Orientation
			{
			public:
				virtual ~IIL9325Orientation(void) { }
				
				virtual int GetWidth(void) const = 0;
				virtual int GetHeight(void) const = 0;
				virtual void SetWindow(int x, int y, int width, int height) = 0;
			};
		}			
	}	
}
