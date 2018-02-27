/*
 * IL9325GraphicDisplay.h
 *
 * Created: 2012/09/28 14:41:00
 *  Author: Seiji Ainoguchi
 */ 

#include "Platform.h"
#include "Device/Display/IGraphicDisplay.h"
#include "Collections/Vector.h"

#pragma once

namespace Device
{
	namespace Display
	{
		namespace IL9325
		{
			class IIL9325ControlPort;
			class IIL9325Orientation;
			
			class IL9325GraphicDisplay : public IGraphicDisplay
			{
			public:
				IL9325GraphicDisplay(IIL9325ControlPort& port);
				virtual void DisplayOn(void);
				virtual void DisplayOff(void);
				virtual void Clear(void);
				virtual void Refresh(void);
				virtual uint_fast16_t GetWidth() const;
				virtual uint_fast16_t GetHeight() const;
				virtual void SetForegroundColor(uint_fast8_t color);
				virtual void SetBackgroundColor(uint_fast8_t color);
				virtual void DrawMonochromePattern(int dx, int dy ,int width, int height, const uint8_t data[]);
				virtual void DrawLinePattern(int left, int top, int width, int height, const uint8_t* pData);

				enum Rotate
				{
					Rotate_0,
					Rotate_90,
					Rotate_180,
					Rotate_270,
				};
			
				virtual ~IL9325GraphicDisplay(void);

				
				void SetRotate(Rotate rotate);
				
				void Fill(int x, int y, int width, int height, uint16_t color);
				void Rectangle(int x, int y, int width, int height, uint16_t color);
				void Blt(int x, int y, int width, int height, const uint16_t data[]);
				void DrawTestPattern(int x, int y, int width, int height);
		
			private:
				IIL9325ControlPort& _port;
				IIL9325Orientation* _pOrientation;
				Collections::Vector<uint8_t> _buffer;

				template<typename T> T* getBuffer(int count)
				{
					_buffer.Resize(count * sizeof(T));
					return reinterpret_cast<T*>(&_buffer[0]);
				}
											
				void initialize(void);
			};
		}
	}
}
