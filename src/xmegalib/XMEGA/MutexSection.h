/*
 * Mutex.h
 *
 * Created: 2012/05/11 1:31:05
 *  Author: Seiji Ainoguchi
 */ 

#pragma once

#include "Mutex.h"

namespace XMEGA
{
	class Mutex;
	class MutexSection
	{
	public:
		inline MutexSection(Mutex& mutex)
		 : _mutex(mutex)
		{
			_mutex.Enter();
		}
		inline ~MutexSection(void)
		{
			_mutex.Leave();
		}
	private:
		Mutex& _mutex;
	};
}
