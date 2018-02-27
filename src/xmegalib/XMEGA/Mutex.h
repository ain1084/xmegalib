/*
 * Mutex.h
 *
 * Created: 2012/05/11 1:40:57
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "CriticalSection.h"

namespace XMEGA
{
	class Mutex
	{
	public:
		inline void Enter(void)
		{
			for (;;)
			{
				CriticalSection cs;
				if (!_inUse)
				{
					_inUse = true;
					break;
				}
			}				
		}
		inline void Leave(void)
		{
			CriticalSection cs;
			_inUse = false;
		}
		inline bool TryEnter(void)
		{
			CriticalSection cs;
			if (!_inUse)
			{
				_inUse = true;
			}
			return _inUse;
		}
		
		
	private:
		volatile bool _inUse;
	};
}